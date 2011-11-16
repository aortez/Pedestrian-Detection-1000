#include <cstdio>
#include <highgui.h>
#include "Grid.h"
using namespace cv;

void show( const Mat& img, const string& windowName )
{
    namedWindow( windowName.c_str() );
    imshow( windowName.c_str(), img );
}

Grid::Grid( Mat src, Vec2i cellDims )
{
    show( src, "input" );

    // TODO: only perform these conversions if the image is not of the correct type(grayscale 64bit float)
    // convert to greyscale
    Mat bwSrc;
    cvtColor( src, bwSrc, CV_RGB2GRAY );

    // convert to floating point
    bwSrc.convertTo( bwSrc, CV_64F, 1.0 / 255 );

    // compute gradients
    Mat gradient_horz( src.size(), CV_64F );
    Mat gradient_vert( src.size(), CV_64F );
    {
        Mat kernel_horz = ( Mat_< double >( 1, 3 ) << -1, 0, 1 );
        Mat kernel_vert = ( Mat_< double >( 3, 1 ) << -1, 0, 1 );
        filter2D( bwSrc, gradient_horz, -1, kernel_horz );
        filter2D( bwSrc, gradient_vert, -1, kernel_vert );
    }

    // compute orientation and orientation magnitude image
    Mat orientation( gradient_horz.size(), CV_64F );
    Mat magnitude( gradient_horz.size(), CV_64F );
    {
        for( int y = 0; y < orientation.rows; y++ )
        {
            for( int x = 0; x < orientation.cols; x++ )
            {
                const Vec2d vec( gradient_horz.at< double >( y, x ), gradient_vert.at< double >( y, x ) );
                const double mag = norm( vec );
                const double angle = Cell::vecToAngle( vec );
                orientation.at< double >( y, x ) = angle;
                magnitude.at< double >( y, x ) = mag;
                printf( "@(%d,%d): (h,v) = (%f, %f), angle = %f, magnitude = %f\n", x, y, vec[ 0 ], vec[ 1 ], angle, mag );
            }
        }
    }

    // allocate cells
    mDims[ 0 ] = src.cols / cellDims[ 0 ];
    mDims[ 1 ] = src.rows / cellDims[ 1 ];
    mCell.resize( mDims[ 0 ] * mDims[ 1 ], Cell( 9 ) );

    // bin up weighted angles into histograms
    printf( "(gridWidth, gridHeight) = (%d,%d)\n", dimX(), dimY() );
    for( int gridY = 0; gridY < dimY(); gridY++ )
    {
        for( int gridX = 0; gridX < dimX(); gridX++ )
        {
            for( int cellY = 0; cellY < cellDims[ 1 ]; cellY++ )
            {
                for( int cellX = 0; cellX < cellDims[ 0 ]; cellX++ )
                {
                    const int pixelX = gridX * cellDims[ 0 ] + cellX;
                    const int pixelY = gridY * cellDims[ 1 ] + cellY;
                    const double pixelAngle = orientation.at< double >( pixelY, pixelX );
                    const double pixelWeight = magnitude.at< double >( pixelY, pixelX );
                    assert( gridX < dimX() && gridX >= 0 );
                    assert( gridY < dimY() && gridY >= 0 );

                    cell( gridX, gridY ).addPixel( pixelAngle, pixelWeight );
                }
            }
        }
    }
    show( bwSrc, "bwSrc" );
    show( gradient_horz, "hgrad" );
    show( gradient_vert, "vgrad" );
    show( orientation, "orientation" );
    show( magnitude, "magnitude" );

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
    return mDims[ 0 ];
}

int Grid::dimY( void ) const
{
    return mDims[ 1 ];
}

