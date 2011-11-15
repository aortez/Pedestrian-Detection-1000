#include <CImg.h>
#include <cstdio>
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <sstream>
#include "createCellHogDisplay.h"
using namespace cv;
using namespace std;
using namespace cimg_library;

bool containsPerson( const Mat& img );

string toString( const Mat& img )
{
    stringstream ss;
    ss << "cols: " << img.cols << endl;
    ss << "rows: " << img.rows << endl;
    ss << "type: " << img.type() << endl;
    ss << "depth: " << img.depth() << endl;
    return ss.str();
}

void show( const Mat& img, const string& windowName )
{
    namedWindow( windowName.c_str() );
    imshow( windowName.c_str(), img );
}

int main( int argc, const char* argv[] )
{
    Mat src;
    if( argc > 1 )
    {
        src = imread( argv[ 1 ] );
    }
    else
    {
        src = imread( "test.png" );
    }

    cout << "Is a person: " << boolalpha << containsPerson( src ) << endl;
    return 0;
}

bool containsPerson( const Mat& src )
{
    show( src, "input" );

    // convert to greyscale
    Mat bwSrc;
    cvtColor( src, bwSrc, CV_RGB2GRAY );

    // convert to floating point
    bwSrc.convertTo( bwSrc, CV_64F, 1.0 / 255 );

    // compute gradient images
    Mat gradient_horz( bwSrc.size(), CV_64F );
    Mat gradient_vert( bwSrc.size(), CV_64F );
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
                const double h = gradient_horz.at< double >( y, x );
                const double v = gradient_vert.at< double >( y, x );
                const double angle = std::abs( atan2( v, h ) / CV_PI ); // 0.0 - 1.0
                const double mag = sqrt( v * v + h * h );
                orientation.at< double >( y, x ) = angle;
                magnitude.at< double >( y, x ) = mag;
//                printf( "@(%d,%d): (h,v) = (%f, %f), angle = %f, magnitude = %f\n", x, y, h, v, angle, mag );
            }
        }
    }

    // create a histogram for each cell...
    const int cellWidth = 6;
    const int cellHeight = 6;
    const int numBins = 9;

    // create grid of cells
    const int gridWidth = src.cols / cellWidth;
    const int gridHeight = src.rows / cellHeight;
    printf( "(gridWidth, gridHeight) = (%d,%d)\n", gridWidth, gridHeight );
    CImg< double > cellHistogram( gridWidth, gridHeight, numBins, 1, 0 );
    cimg_forXY( cellHistogram, gridX, gridY )
    {
        for( int cellY = 0; cellY < cellHeight; cellY++ )
        {
            for( int cellX = 0; cellX < cellWidth; cellX++ )
            {
                const int pixelX = gridX * cellWidth + cellX;
                const int pixelY = gridY * cellHeight + cellY;
                const double pixelAngle = orientation.at< double >( pixelY, pixelX );
                const double pixelWeight = magnitude.at< double >( pixelY, pixelX );
                const int binIndex = static_cast< int >( round( pixelAngle * ( numBins - 1 ) ) );
                printf( "angle: %f, binIndex: %d, pixelWeight: %f\n", pixelAngle, binIndex, pixelWeight );
                assert( binIndex < numBins && binIndex >= 0 );
                assert( gridX < gridWidth && gridX >= 0 );
                assert( gridY < gridHeight && gridY >= 0 );

                cellHistogram( gridX, gridY, binIndex ) += pixelWeight;
            }
        }
    }

    createCellHogDisplay( cellHistogram ).display();

    show( bwSrc, "bwSrc" );
    show( gradient_horz, "hgrad" );
    show( gradient_vert, "vgrad" );
    show( orientation, "orientation" );
    show( magnitude, "magnitude" );

    waitKey();

    // Descriptor Blocks

    // Block Normalization

    // SVM classification

    return false;
}
