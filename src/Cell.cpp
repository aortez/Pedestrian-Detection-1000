#include <cstdio>
#include "Cell.h"

Cell::Cell( int numBins )
{
    assert( numBins > 0 );
    mBins.resize( numBins );
}

void Cell::addPixel( const double angle, const double weight )
{
    const int binIndex = angleToBinIndex( angle, mBins.size() );
    mBins[ binIndex ] += weight;
}

double Cell::angleToBinIndex( const double angle, const int numBins )
{
    // normalize to [0,1]
    const double normalizedAngle = angle / CV_PI;

    // compute bin index
    const int binIndex = static_cast< int >( round( normalizedAngle * ( numBins - 1 ) ) );
    printf( "angle: %f, binIndex: %d\n", angle, binIndex );
    assert( binIndex < numBins && binIndex >= 0 );

    return binIndex;
}

double Cell::vecToAngle( const cv::Vec2d& vec )
{
    // compute angle in [ -pi, +pi ]
    double angle = atan2( vec[ 0 ], vec[ 1 ] );

    // convert into [ 0, pi ], mapping negative angles into positive range
    ( angle < 0 ) ? angle += CV_PI : angle;
    assert( angle >= 0 && angle <= CV_PI );

    return angle;
}

