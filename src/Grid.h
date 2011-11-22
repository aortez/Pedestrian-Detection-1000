#pragma once
#include <cv.h>
#include <vector>
#include "Cell.h"

class Grid
{
public:
    Grid( cv::Mat src, cv::Size cellDims );
    ~Grid( void );

    Cell& cell( int x, int y );

    int dimX(void) const;

    int dimY(void) const;

private:
    std::vector< Cell > mCell;
    cv::Size mCellDims;
    cv::Size mGridDims;
};
