#pragma once
#include <cv.h>
#include <vector>
#include "Cell.h"

class Grid
{
public:
    Grid( cv::Mat src, cv::Size cellDims, int numBins, bool shouldIgnoreSign );
    ~Grid( void );

    Cell& cell( int x, int y );
    const Cell& cell( int x, int y ) const;

    std::vector< cv::Mat > createDescriptorVectors( int blockWidth ) const;

    cv::Mat createHogImage( int scale );

    int dimX( void ) const;

    int dimY( void ) const;

    const std::vector< cv::Mat >& getDescriptorVectors( void ) const;

    void normalizeDescriptorVectors( std::vector< cv::Mat >& descriptorVectors );

private:
    void populateCells( void );

    std::vector< Cell > mCell;
    cv::Size mCellDims;
    std::vector< cv::Mat > mDescriptorVector;
    cv::Size mGridDims;
    int mNumBins;
    const bool mShouldIgnoreSign;
    cv::Mat mSource;
};
