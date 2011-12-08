#include <UnitTest++.h>
#include <cstdio>
#include <cv.h>
#include <opencv2/ml/ml.hpp>
#include <highgui.h>
#include <iostream>
#include "Detector.h"
using namespace cv;
using namespace std;


int main( int argc, const char* argv[] )
{
//    Mat src;
    if( argc > 1 )
    {
        if ( argv[ 1 ] == string( "-test" ) )
        {
            return UnitTest::RunAllTests();
        }
    }
//        else
//        {
//            src = imread( argv[ 1 ] );
//        }
//    }
//    else
//    {
//        src = imread( "test.png" );
//    }

    Detector detector;

    Mat src1 = imread( "test.png" );
    Mat src2 = imread( "test2.png" );
    Mat src3 = imread( "test3.png" );
    Mat src4 = imread( "test4.png" );

    detector.train( src1, true );
    detector.train( src2, true );
    detector.train( src3, true );
    detector.train( src4, true );

    Mat neg1 = imread( "neg1.png" );
    Mat neg2 = imread( "neg2.png" );
    Mat neg3 = imread( "neg3.png" );
    Mat neg4 = imread( "neg4.png" );


    return 0;
}

