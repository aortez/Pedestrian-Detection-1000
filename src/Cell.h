#pragma once
#include <cv.h>
#include <vector>

class Cell
{
public:
    explicit Cell( int numBins );

    void addPixel( double gradientAngle, double weight );

    static double angleToBinIndex( double angle, int numBins );

    static double vecToAngle( const cv::Vec2d& vec );

private:
    std::vector< int > mBins;
};
