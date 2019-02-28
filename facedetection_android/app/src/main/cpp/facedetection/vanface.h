//
// Created by danpe on 2019/02/18.
//
#pragma once

#ifndef __VANFACE_H__
#define __VANFACE_H__
#include <opencv2/opencv.hpp>
#include "net.h"
#include "face_struct.h"

class VanFace {
public:
    VanFace(const std::string& model_path);
    VanFace(const std::string param_files, const std::string bin_files);
    ~VanFace();

    void detectLandmark(cv::Mat& img_, std::vector<Bbox>& faceBbox);

protected:
    void getSenseBbox(std::vector<Bbox>& faceBbox, int imgHeight, int imgWidth, std::vector<cv::Rect>& senseBbox);

    ncnn::Net vanFaceNet;
    cv::Mat img;
private:
    cv::Size netSize = cv::Size(60, 60);
    int num_threads = 1;
};


#endif //__VANFACE_H__