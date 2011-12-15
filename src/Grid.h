#pragma once
#include <cv.h>
#include <vector>
#include "Cell.h"

//! A grid of HOG(histogram of oriented gradient) cells.  The
//! Grid class is used to convert an image into a collection of
//! descriptor vectors.
class Grid
{
public:
    //! Constructor.
    Grid(
            cv::Mat src, //!< Input image.
            cv::Size cellDims, //! The cols X rows cell dimensions.
            int numBins, //! The number of orientation bins.
            bool shouldIgnoreSign );

    //! Destructor.
    ~Grid( void );

    //! Creates an visual depiction of the HOGs overlayed on the input image.
    //! The scale parameter optionally enlarges the output image.
    cv::Mat createHogImage( int scale );

    //! Gets the descriptor vectors computed from the input image.
    const std::vector< cv::Mat >& getDescriptorVectors( void ) const;

private:
    Cell& cell( int x, int y );
    const Cell& cell( int x, int y ) const;

    std::vector< cv::Mat > createDescriptorVectors( int blockWidth ) const;

    int dimX( void ) const;

    int dimY( void ) const;

    void normalizeDescriptorVectors( std::vector< cv::Mat >& descriptorVectors );

    void populateCells( void );

    std::vector< Cell > mCell;
    cv::Size mCellDims;
    std::vector< cv::Mat > mDescriptorVector;
    cv::Size mGridDims;
    int mNumBins;
    const bool mShouldIgnoreSign;
    cv::Mat mSource;
};
