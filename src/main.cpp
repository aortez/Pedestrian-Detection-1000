#include <cstdio>
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <sstream>
using namespace cv;
using namespace std;


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

/**
 * Entry point to app. Loads image from file system and feeds it through the algorithm.
 */
int main( int argc, const char* argv[] )
{

    Mat src = imread( "test.png" );
//    Mat src = imread( argv[ 1 ] );

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
        Mat kernel_horz = ( Mat_<double>( 1, 3 ) << -1, 0, 1 );
        Mat kernel_vert = ( Mat_<double>( 3, 1 ) << -1, 0, 1 );
        filter2D( bwSrc, gradient_horz, -1, kernel_horz );
        filter2D( bwSrc, gradient_vert, -1, kernel_vert );
    }

    // compute orientation and orientation magnitude image
    Mat orientation( gradient_horz.size(), CV_64F );
    Mat magnitude( gradient_horz.size(), CV_64F );
    {
        for ( int y = 0; y < orientation.rows; y++ )
        {
            for ( int x = 0; x < orientation.cols; x++ )
            {
                const double h = gradient_horz.at< double >( y, x );
                const double v = gradient_vert.at< double >( y, x );
                const double angle = atan2( v, h );
                const double mag = sqrt( v * v + h * h );
                orientation.at< double >( y, x ) = angle;
                magnitude.at< double >( y, x ) = mag;
                printf( "@(%d,%d): (h,v) = (%f, %f), angle = %f, magnitude = %f\n", x, y, h, v, angle, mag );
            }
        }
    }

    show( bwSrc, "bwSrc" );
    show( gradient_horz, "hgrad" );
    show( gradient_vert, "vgrad" );
    show( orientation, "orientation" );
    show( magnitude, "magnitude" );
    waitKey();

    // Histograms -- Orientation Binning

    // Descriptor Blocks

    // Block Normalization

    // SVM classification

    return false;
}
