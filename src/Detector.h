#pragma once
#include <cv.h>
#include <opencv2/ml/ml.hpp>
#include <vector>

class Detector
{
public:
    Detector( void );

    void addTrainingFile(
            const std::string& fileName,
            bool shouldShouldHogImages );

    void addTrainingImage(
            const cv::Mat& image,
            bool isPositive,
            bool shouldShowHogImage = false );

    double classify(
            const cv::Mat& image,
            bool shouldShowHogImage = false );

    void train( void );

private:
    cv::Mat cropToDetectionWindow( const cv::Mat& image );

    cv::Size mCellDims;
    std::vector< cv::Mat > mDescriptorVectors;
    std::vector< bool > mDescriptorVectorLabels;
    const cv::Vec2i mDetectionWindow;
    const int mNumBins;
    const bool mShouldIgnoreSign;
    CvSVM mSvm;
    CvSVMParams mSvmParameters;
};
