#pragma once
#include <cv.h>
#include <vector>
#include "Cell.h"

class Grid
{
public:
    Grid( cv::Mat src, cv::Size cellDims, int numBins );
    ~Grid( void );

    Cell& cell( int x, int y );

    std::vector< cv::Mat > createDescriptorVectors( int blockWidth );

    cv::Mat createHogImage( int scale );

    int dimX( void ) const;

    int dimY( void ) const;

    const std::vector< cv::Mat >& getDescriptorVectors( void ) const;

    void normalizeDescriptorVectors( std::vector< cv::Mat >& descriptorVectors );

private:
    void populateCells( int numBins );

    std::vector< Cell > mCell;
    cv::Size mCellDims;
    std::vector< cv::Mat > mDescriptorVector;
    cv::Size mGridDims;
    int mNumBins;
    cv::Mat mSource;
};
