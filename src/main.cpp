#include <UnitTest++.h>
#include <cstdio>
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include "Cell.h"
#include "Grid.h"
using namespace cv;
using namespace std;

bool containsPerson( const Mat& img );

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
    Size cellDims( 6, 6 );
    Grid grid( src, cellDims );

    // Descriptor Blocks

    // Block Normalization

    // SVM classification

    return false;
}
