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

    vector< string > posFiles;
    posFiles.push_back( "test.png" );
    posFiles.push_back( "test2.png" );
    posFiles.push_back( "test3.png" );
    posFiles.push_back( "test4.png" );
//        posFiles.push_back( "Test/pos/crop001501a.png" );
//        posFiles.push_back( "Test/pos/crop001501b.png" );
//        posFiles.push_back( "Test/pos/crop001501c.png" );
//        posFiles.push_back( "Test/pos/crop001501d.png" );
//        posFiles.push_back( "Test/pos/crop001501e.png" );
//        posFiles.push_back( "Test/pos/crop001501f.png" );
//        posFiles.push_back( "Test/pos/crop001501g.png" );
//        posFiles.push_back( "Test/pos/crop001501h.png" );
//        posFiles.push_back( "Test/pos/crop001504a.png" );
//        posFiles.push_back( "Test/pos/crop001504b.png" );
//        posFiles.push_back( "Test/pos/crop001504c.png" );
//        posFiles.push_back( "Test/pos/crop001504d.png" );

    vector< string > negFiles;
    negFiles.push_back( "neg1.png" );
    negFiles.push_back( "neg2.png" );
    negFiles.push_back( "neg3.png" );
    negFiles.push_back( "neg4.png" );
//        negFiles.push_back( "Test/neg/00001147.png" );
//        negFiles.push_back( "Test/neg/00001148.png" );
//        negFiles.push_back( "Test/neg/00001149.png" );
//        negFiles.push_back( "Test/neg/00001150.png" );
//        negFiles.push_back( "Test/neg/00001153.png" );

    vector< string > testFiles;
    testFiles.push_back( "test.png" );
    testFiles.push_back( "test2.png" );
    testFiles.push_back( "test3.png" );
    testFiles.push_back( "test4.png" );
    testFiles.push_back( "neg1.png" );
    testFiles.push_back( "neg2.png" );
    testFiles.push_back( "neg3.png" );
    testFiles.push_back( "neg4.png" );
    testFiles.push_back( "Test/pos/crop001501c.png" );
    testFiles.push_back( "Test/pos/crop001501d.png" );
    testFiles.push_back( "Test/pos/crop001501e.png" );
    testFiles.push_back( "Test/pos/crop001501f.png" );
    testFiles.push_back( "Test/neg/00001150.png" );
    testFiles.push_back( "Test/neg/00001153.png" );
    testFiles.push_back( "Test/pos/crop001504d.png" );
    testFiles.push_back( "Test/pos/crop001511a.png" );
    testFiles.push_back( "Test/pos/crop001511b.png" );

//    for( size_t i = 0; i < posFiles.size(); i++ )
//    {
//        Mat pos = imread( posFiles[ i ].c_str() );
//        detector.addTrainingImage( pos, true, true );
////            Mat flipped;
////            flip( pos, flipped, 1 );
////            detector.addTrainingImage( flipped, true, true );
//
//    }
//
//    for( size_t i = 0; i < negFiles.size(); i++ )
//    {
//        Mat neg = imread( negFiles[ i ].c_str() );
//        detector.addTrainingImage( neg, false, true );
////            Mat flipped;
////            flip( neg, flipped, 1 );
////            detector.addTrainingImage( flipped, false, true );
//    }

//    detector.train();
//
//    for( size_t i = 0; i < testFiles.size(); i++ )
//    {
//        Mat test = imread( testFiles[ i ].c_str() );
//        detector.classify( test, true );
//    }

    return 0;
}

