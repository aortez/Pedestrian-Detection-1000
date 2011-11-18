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

int Cell::angleToBinIndex( double angle, const int numBins, const bool shouldIgnoreSign )
{
    printf( "angle: %f\n", angle );
    assert( angle >= -CV_PI && angle <= CV_PI );
    // input angle is [-pi,+pi]

    // normalize to [0,1]
    double normalizedAngle;
    if( shouldIgnoreSign )
    {
        // if negative, then map into positive range [0,pi]
        if ( angle < 0 ) angle += CV_PI;
        // scale from [0,pi] -> [0,1]
        normalizedAngle = angle / CV_PI;
    }
    else
    {
        // scale to [-0.5,0.5] and shift into [0,1]
        normalizedAngle = angle / ( 2 * CV_PI ) + 0.5;
    }
    printf( "normlized angle: %f\n", normalizedAngle );
    assert( normalizedAngle >= 0 && normalizedAngle <= 1 );

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

double Cell::binNormalized( const int binIndex ) const
{
    const int votes = bin( binIndex );
    return votes / static_cast< double >( mSampleSum );
}

int Cell::numBins( void ) const
{
    return mBins.size();
}
