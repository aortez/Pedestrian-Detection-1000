#include <cstdio>
#include <cv.h>
#include <highgui.h>
#include "Cell.h"
#include "util.h"
using namespace cv;

Cell::Cell( const int numBins, const bool shouldIgnoreSign ) :
        mSampleSum( 0 ), mShouldIgnoreSign( shouldIgnoreSign )
{
    assert( numBins > 0 );
    mBins.resize( numBins );
}

void Cell::addImage( const cv::Mat& src )
{
    mSource = src.clone();

    Mat gradient_horz( mSource.size(), CV_32F );
    Mat gradient_vert( mSource.size(), CV_32F );
    {
        Mat kernel_horz = ( Mat_< float >( 1, 3 ) << -1, 0, 1 );
        Mat kernel_vert = ( Mat_< float >( 3, 1 ) << -1, 0, 1 );
        filter2D( mSource, gradient_horz, -1, kernel_horz );
        filter2D( mSource, gradient_vert, -1, kernel_vert );
    }

    for( int y = 0; y < src.rows; y++ )
    {
        for( int x = 0; x < src.cols; x++ )
        {
            const Vec2d vec( gradient_horz.at< float >( y, x ), gradient_vert.at< float >( y, x ) );
            const float mag = norm( vec );
            const double angle = atan2( static_cast<double>( vec[ 1 ] ), static_cast<double>( vec[ 0 ] ) );
//            printf( "@(%d,%d): (h,v) = (%f, %f), angle = %f, magnitude = %f\n", x, y, vec[ 0 ], vec[ 1 ], angle, mag );

            addPixel( angle, mag );
        }
    }
}

void Cell::addPixel( const double angle, const float weight )
{
    const int binIndex = angleToBinIndex( angle, mBins.size(), mShouldIgnoreSign );
    mBins[ binIndex ] += weight;
    mSampleSum += weight;
}

int Cell::angleToBinIndex( double angle, const int numBins, const bool shouldIgnoreSign )
{
    assert( angle >= -CV_PI && angle <= CV_PI );
    // input angle is [-pi,+pi]

    // normalize to [0,1]
    double normalizedAngle;
    if( shouldIgnoreSign )
    {
        // if negative, then map into positive range [0,pi]
        if( angle < 0 ) angle += CV_PI;
        // scale from [0,pi] -> [0,1]
    }
    else
    {
        // scale to [-0.5,0.5] and shift into [0,1]
        normalizedAngle = angle / CV_PI;
        normalizedAngle = angle / ( 2 * CV_PI ) + 0.5;
    }
//    printf( "normlized angle: %f\n", normalizedAngle );
    assert( normalizedAngle >= 0 && normalizedAngle <= 1 );

    // compute bin index
    const int binIndex = static_cast< int >( floor( normalizedAngle * ( numBins - 1 ) + 0.5 ) );
//    printf( "angle: %f, binIndex: %d\n", angle, binIndex );
    assert( binIndex < numBins && binIndex >= 0 );

    return binIndex;
}

double Cell::bin( const int binIndex ) const
{
    assert( binIndex >= 0 && binIndex < static_cast<int>( mBins.size() ) );
    return mBins[ binIndex ];
}

double Cell::binAngle( int binIndex ) const
{
    return binIndexToAngle( binIndex, numBins(), mShouldIgnoreSign );
}

double Cell::binIndexToAngle( const int binIndex, const int numBins, const bool shouldIgnoreSign )
{
    // convert to normalized angle [0, 1]
    const double normalizedAngle = binIndex / ( numBins - 1.0 );
    assert( normalizedAngle >= 0 && normalizedAngle <= 1.0 );

    double angle;
    if( shouldIgnoreSign )
    {
        // map into [0, pi]
        angle = normalizedAngle * CV_PI;
    }
    else
    {
        // map into [-pi, +pi]
        angle = ( normalizedAngle - 0.5 ) * CV_PI * 2;
    }

    return angle;
}

float Cell::binNormalized( const int binIndex ) const
{
    const float votes = bin( binIndex );
    return votes / mSampleSum;
}

Mat Cell::drawHOG( int cellScale ) const
{
    const int scale = cellScale;
    const float magnitudeScalar = scale * 5.0;
    Mat out;
    resize( mSource, out, mSource.size() * scale );
    Point center( mSource.cols / 2 * scale, mSource.rows / 2 * scale );
    Scalar color( 1 );

    for( int binIndex = 0; binIndex < numBins(); binIndex++ )
    {
        const float magnitude = binNormalized( binIndex ) * magnitudeScalar;
        const double angle = binAngle( binIndex );
        Point end( center.x + cos( angle ) * magnitude, center.y + sin( angle ) * magnitude );
        line( out, center, end, color );
    }

    return out;
}

const vector< float >& Cell::getHog( void ) const
{
    return mBins;
}

float Cell::getSum( void ) const
{
    return mSampleSum;
}

int Cell::numBins( void ) const
{
    return mBins.size();
}
