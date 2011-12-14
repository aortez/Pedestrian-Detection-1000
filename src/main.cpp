#include <UnitTest++.h>
#include <cstdio>
#include <cv.h>
#include <opencv2/ml/ml.hpp>
#include <highgui.h>
#include <iostream>
#include "Detector.h"
#include "util.h"
using namespace cv;
using namespace std;

namespace OperatingMode
{
enum e
{
    unittest, train, classify, undefined
};
}

int main( int argc, const char* argv[] )
{
    printf( "Pedestrian Detector 1000\n" );

    OperatingMode::e operatingMode = OperatingMode::undefined;
    string fileName;
    if( argc > 1 )
    {
        if( argv[ 1 ] == string( "-test" ) )
        {
            operatingMode = OperatingMode::unittest;
        }
        else if( argv[ 1 ] == string( "-train" ) )
        {
            if( argc < 3 )
            {
                printf( "Usage error: please specify the training filename.\n" );
                return -1;
            }
            operatingMode = OperatingMode::train;
            fileName = argv[ 2 ];
        }
        else if( argv[ 1 ] == string( "-classify" ) )
        {
            if( argc < 3 )
            {
                printf( "Usage error: please specify the classifying filename.\n" );
                return -1;
            }
            operatingMode = OperatingMode::classify;
            fileName = argv[ 2 ];
        }
    }

    Detector detector;
    switch( operatingMode )
    {
    case OperatingMode::classify:
    	printf( "Classifying: %s\n", fileName.c_str() );
        detector.classify( imread( fileName ), true );
        break;
    case OperatingMode::train:
        detector.addTrainingFile( fileName, false );
        detector.train();
        break;
    case OperatingMode::unittest:
        printf( "Running unit tests...\n" );
        return UnitTest::RunAllTests();
        break;
    case OperatingMode::undefined:
    default:
        printf( "Usage options: \n" );
        printf( "\t-classify <filename>\t classifies the specified image.\n" );
        printf( "\t-train <filename>   \t trains the detector with the images in the file.\n" );
        printf( "\t-test               \t runs unit tests.\n" );
        return 0;
        break;
    }

    return 0;
}

