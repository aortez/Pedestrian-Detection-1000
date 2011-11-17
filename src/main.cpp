#include <UnitTest++.h>
#include <CImg.h>
#include <cstdio>
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <sstream>
#include "Cell.h"
#include "Grid.h"
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

int main( int argc, const char* argv[] )
{
    Mat src;
    if( argc > 1 )
    {
        if ( argv[ 1 ] == string( "-test" ) )
        {
            return UnitTest::RunAllTests();
        }
        else
        {
            src = imread( argv[ 1 ] );
        }
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
    // create grid of HOG cells
    Vec2i cellDims( 6, 6 );
    Grid grid( src, cellDims );

    // Descriptor Blocks

    // Block Normalization

    // SVM classification

    return false;
}
