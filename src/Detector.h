#pragma once
#include <cv.h>
#include <opencv2/ml/ml.hpp>
#include <vector>

class Detector
{
public:
    Detector( void );

    void addTrainingImage(
            const cv::Mat& image,
            bool isPositive,
            bool shouldShowHogImage = false );

    float classify(
            const cv::Mat& image,
            bool shouldShowHogImage = false );

    void train( void );

private:
    std::vector< cv::Mat > mDescriptorVectors;
    std::vector< bool > mDescriptorVectorLabels;
    CvSVM mSvm;
    CvSVMParams mSvmParameters;
};
