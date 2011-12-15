#pragma once
#include <cv.h>
#include <string>

void assertTrue( bool truthVal, const std::string& errMsg );

double round( double r );
                         
void show(
        const cv::Mat& img,
        const std::string& windowName,
        int scale = 1,
        int interpolation_type = cv::INTER_LINEAR );

std::string toString( const cv::Mat& img );
