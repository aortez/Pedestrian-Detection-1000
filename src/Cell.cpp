#include <cstdio>
#include "Cell.h"

Cell::Cell( const int numBins, const bool shouldIgnoreSign ) :
        mSampleSum( 0 ),
        mShouldIgnoreSign( shouldIgnoreSign )
{
    assert( numBins > 0 );
    mBins.resize( numBins );
}

void Cell::addPixel( const double angle, const double weight )
{
    const int binIndex = angleToBinIndex( angle, mBins.size(), mShouldIgnoreSign );
    mBins[ binIndex ] += weight;
    mSampleSum += weight;
}

int Cell::angleToBinIndex( const double angle, const int numBins, const bool shouldIgnoreSign )
{
    // normalize to [0,1]
    double normalizedAngle;
    if( shouldIgnoreSign )
    {
        normalizedAngle = angle / CV_PI;
    }
    else
    {
        normalizedAngle = angle / ( CV_PI * 2 );
    }

    // compute bin index
    const int binIndex = static_cast< int >( round( normalizedAngle * ( numBins - 1 ) ) );
    printf( "angle: %f, binIndex: %d\n", angle, binIndex );
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
    // convert to normalized angle [ 0, 1 ]
    const double normalizedAngle = binIndex / ( numBins - 1.0 );

    double angle;
    if( shouldIgnoreSign )
    {
        // map into [ 0, pi ]
        angle = normalizedAngle * CV_PI;
    }
    else
    {
        // map into [ -pi, +pi ]
        angle = ( normalizedAngle - 0.5 ) * CV_PI;
    }

    return angle;
}

double Cell::binNormalized( const int binIndex ) const
{
    const int votes = bin( binIndex );
    return votes / static_cast< double >( mSampleSum );
}

int Cell::numBins( void ) const
{
    return mBins.size();
}

double Cell::vecToAngle( const cv::Vec2d& vec, const bool shouldIgnoreSign )
{
    // compute angle in [ -pi, +pi ]
    double angle = atan2( vec[ 0 ], vec[ 1 ] );

    if( shouldIgnoreSign )
    {
        // convert into [ 0, pi ], mapping negative angles into positive range
        ( angle < 0 ) ? angle += CV_PI : angle;
        assert( angle >= 0 && angle <= CV_PI );
    }
    else
    {
        // translate into [ 0, 2 * pi ]
        angle += CV_PI;
    }

    return angle;
}

