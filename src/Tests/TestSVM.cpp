#include "UnitTest++.h"
#include "cv.h"
#include <opencv2/ml/ml.hpp>

using namespace cv;

SUITE( SVMTest )
{
    TEST( SimpleTest )
    {
        // setup training data
        Mat trainClass1 = ( Mat_< double >( 1, 4 ) << -10, -9, -6, -0.0000001 );
        Mat trainClass2 = ( Mat_< double >( 1, 4 ) << 10, 9, 6, 0.0000001 );

//        CvSVMParams params;
//        params.svm_type    = SVM::C_SVC;
//        params.C              = 0.1;
//        params.kernel_type = SVM::LINEAR;
//        params.term_crit   = TermCriteria(CV_TERMCRIT_ITER, (int)1e7, 1e-6);
    }
}
