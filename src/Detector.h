#pragma once
#include <cv.h>
#include <opencv2/ml/ml.hpp>

class Detector
{
public:
    Detector( void );

    void train( const cv::Mat& trainingData, bool isPositive );

private:
    CvSVM mSvm;
    CvSVMParams mSvmParameters;
};
