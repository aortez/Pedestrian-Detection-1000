#include <cv.h>
#include <highgui.h>
#include <cstdio>
#include "Grid.h"
#include "util.h"
using namespace cv;
using namespace std;

const bool shouldIgnoreSign = false;

Mat Grid::createHogImage( const int scale )
{
    // create HOG
    Size scaledCellDims( mCellDims.width * scale, mCellDims.height * scale );
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

    return hog;
}

Grid::Grid( Mat src, Size cellDims ) :
        mCellDims( cellDims )
{
    // TODO: only perform these conversions if the image is not of the correct type(grayscale 64bit double)
    // convert to greyscale
    Mat bwSrc;
    cvtColor( src, bwSrc, CV_RGB2GRAY );

    // convert to floating point
    bwSrc.convertTo( bwSrc, CV_64F, 1.0 / 255 );

    mSource = bwSrc;

    // populate cells
    const int numBins = 9;
    populateCells( numBins );

    // create hogImage
    show( mSource, "source" );
    Mat hog = createHogImage( 5 );
    show( hog, "hog" );

    // accumulate descriptor vectors
    const int blockWidth = 4;
    vector< Mat > descriptorVectors = createDescriptorVectors( blockWidth );

    waitKey();
}

Grid::~Grid( void )
{
}

Cell& Grid::cell( int x, int y )
{
    assert( x >= 0 && x < dimX() );
    assert( y >= 0 && y < dimY() );

    return mCell[ x + y * dimX() ];
}

vector< Mat > Grid::createDescriptorVectors( const int blockWidth )
{
    // allocate memory for descriptor vector
    const int blockRadius = blockWidth / 2;
    printf( "blockWidth: %d, blockRadius: %d\n", blockWidth, blockRadius );
    const Range gridRangeX( blockRadius, dimX() - blockRadius );
    const Range gridRangeY( blockRadius, dimY() - blockRadius );
    const int numCellsPerBlock = blockWidth * blockWidth;
    vector< Mat > descriptorVectors( gridRangeX.size() * gridRangeY.size(), Mat( numCellsPerBlock, 1, CV_64FC1 ) );
    for( int gridY = gridRangeY.start; gridY < gridRangeY.end; gridY++ )
    {
        for( int gridX = gridRangeX.start; gridX < gridRangeX.end; gridX++ )
        {
            // create the descriptor vector for this block
            const int descriptorVectorIndex = ( gridX - gridRangeX.start ) + ( gridY - gridRangeY.start ) * gridRangeX.size();
            Mat& descriptorVector = descriptorVectors[ descriptorVectorIndex ];
            descriptorVector = Mat( blockWidth * blockWidth, 1, CV_64FC1 );
            for( int blockY = 0; blockY < blockWidth; blockY++ )
            {
                for( int blockX = 0; blockX < blockWidth; blockX++ )
                {
                    const int cellX = gridX + blockX - blockRadius;
                    const int cellY = gridY + blockY - blockRadius;
                    const double cellSum = cell( cellX, cellY ).getSum();
                    const int blockIndex = blockX + blockY * blockX;
                    printf( "cellX,Y: %d, %d, blockIndex: %d\n", cellX, cellY, blockIndex );
                    descriptorVector.at< double >( blockIndex, 1 ) = cellSum;
                }
            }
        }
    }
}

int Grid::dimX( void ) const
{
    return mGridDims.width;
}

int Grid::dimY( void ) const
{
    return mGridDims.height;
}

void Grid::populateCells( int numBins )
{
    // allocate cells
    mGridDims.width = mSource.cols / mCellDims.width;
    mGridDims.height = mSource.rows / mCellDims.height;
    mCell.resize( mGridDims.width * mGridDims.height, Cell( 9, shouldIgnoreSign ) );

    // populate cells
    printf( "(gridWidth, gridHeight) = (%d,%d)\n", dimX(), dimY() );
    for( int gridY = 0; gridY < dimY(); gridY++ )
    {
        for( int gridX = 0; gridX < dimX(); gridX++ )
        {
            // construct cell's ROI
            Rect roi( gridX * mCellDims.width, gridY * mCellDims.height, mCellDims.width, mCellDims.height );
            Mat cellSrc = mSource( roi );
            cell( gridX, gridY ).addImage( cellSrc );
        }
    }
}
