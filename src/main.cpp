#include <cv.h>
#include <highgui.h>
#include <iostream>
using namespace cv;
using namespace std;

/**
 * Process the input image to determine if it contains a person.
 */
bool containsPerson( const Mat& img );


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
    namedWindow( "input" );
    imshow( "input", src );

    // convert to greyscale
    Mat bwSrc;
    cvtColor( src, bwSrc, CV_RGB2GRAY );
    
    // compute gradient images
    Mat gradient_horz( bwSrc.size(), bwSrc.type() );
    Mat gradient_vert( bwSrc.size(), bwSrc.type() );
    {
        Mat kernel_horz = ( Mat_<double>( 1, 3 ) << -1, 0, 1 );
        Mat kernel_vert = ( Mat_<double>( 3, 1 ) << -1, 0, 1 );
        filter2D( bwSrc, gradient_horz, -1, kernel_horz );
        filter2D( bwSrc, gradient_vert, -1, kernel_vert );
    }
    namedWindow( "bw" );
    imshow( "bw", bwSrc );

    namedWindow( "hgrad" );
    imshow( "hgrad", gradient_horz );

    namedWindow( "vgrad" );
    imshow( "vgrad", gradient_vert );
    waitKey();

    // compute orientation image
//    Mat orientation( gradient_horz.size(), CV_64FC1 );
//    {
//        for ( int y = 0; y < orientation.rows; y++ )
//        {
//            for ( int x = 0; x < orientation.cols; x++ )
//            {
//                const double h = gradient_horz.at< double >( y, x );
//                const double v = gradient_vert.at< double >( y, x );
//                orientation.at< double >( y, x ) = atan2( v, h );
//            }
//        }
//    }

    // Histograms -- Orientation Binning

    // Descriptor Blocks

    // Block Normalization

    // SVM classification

    return false;
}
