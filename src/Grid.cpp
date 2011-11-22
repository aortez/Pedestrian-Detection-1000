#include <cv.h>
#include <highgui.h>
#include <cstdio>
#include "Grid.h"
#include "util.h"
using namespace cv;
using namespace std;

const bool shouldIgnoreSign = false;

Grid::Grid( Mat src, Size cellDims ) :
        mCellDims( cellDims )
{
    // TODO: only perform these conversions if the image is not of the correct type(grayscale 64bit double)
    // convert to greyscale
    Mat bwSrc;
    cvtColor( src, bwSrc, CV_RGB2GRAY );

    // convert to floating point
    bwSrc.convertTo( bwSrc, CV_64F, 1.0 / 255 );

    // allocate cells
    mGridDims.width = src.cols / mCellDims.width;
    mGridDims.height = src.rows / mCellDims.height;
    mCell.resize( mGridDims.width * mGridDims.height, Cell( 9, shouldIgnoreSign ) );

    // populate cells
    printf( "(gridWidth, gridHeight) = (%d,%d)\n", dimX(), dimY() );
    for( int gridY = 0; gridY < dimY(); gridY++ )
    {
        for( int gridX = 0; gridX < dimX(); gridX++ )
        {
            // construct cell's ROI
            Rect roi( gridX * cellDims.width, gridY * cellDims.height, cellDims.width, cellDims.height );
            Mat cellSrc = bwSrc( roi );
            cell( gridX, gridY ).addImage( cellSrc );
        }
    }
    show( bwSrc, "bwSrc" );

    // create HOG
    int scale = 5;
    Size scaledCellDims( cellDims.width * scale, cellDims.height * scale );
    Mat hog( mGridDims.height * scaledCellDims.height, mGridDims.width * scaledCellDims.width, CV_64FC1 );
    for( int gridY = 0; gridY < dimY(); gridY++ )
    {
        for( int gridX = 0; gridX < dimX(); gridX++ )
        {
            // construct cell's ROI
            Rect roi( gridX * scaledCellDims.width, gridY * scaledCellDims.height, scaledCellDims.width, scaledCellDims.height );
            Mat cellOutputRegion = hog( roi );

            // output cell's hog to ROI
            Mat cellHog = cell( gridX, gridY ).drawHOG( scale );
            cellHog.copyTo( cellOutputRegion );
        }
    }
    show( hog, "hog" );

    waitKey();
}

Grid::~Grid()
{
}

Cell& Grid::cell( int x, int y )
{
    assert( x >= 0 && x < dimX() );
    assert( y >= 0 && y < dimY() );

    return mCell[ x + y * dimX() ];
}

int Grid::dimX( void ) const
{
    return mGridDims.width;
}

int Grid::dimY( void ) const
{
    return mGridDims.height;
}

