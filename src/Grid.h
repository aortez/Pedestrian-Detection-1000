#pragma once
#include <cv.h>
#include <vector>
#include "Cell.h"

class Grid
{
public:
    Grid( cv::Mat src, cv::Vec2i cellDims );
    ~Grid();

    Cell& cell( int x, int y );

    int dimX(void) const;

    int dimY(void) const;

private:
    std::vector< Cell > mCell;
    cv::Vec2i mDims;
};
