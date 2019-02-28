//
// Created by Lonqi on 2017/11/18.
//
#pragma once

#ifndef __MTCNN_NCNN_H__
#define __MTCNN_NCNN_H__

#include "net.h"
#include "face_struct.h"
//#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <time.h>
#include <algorithm>
#include <map>
#include <iostream>

using namespace std;
//using namespace cv;


class MTCNN {

public:
    MTCNN(const string &model_path);

    MTCNN(const std::vector<std::string> param_files, const std::vector<std::string> bin_files);

    ~MTCNN();

    void SetMinFace(int minSize);

    void SetNumThreads(int numThreads);

    void detect(ncnn::Mat &img_, std::vector<Bbox> &finalBbox);

    void detectMaxFace(ncnn::Mat &img_, std::vector<Bbox> &finalBbox);
    //  void detection(const cv::Mat& img, std::vector<cv::Rect>& rectangles);
protected:
    void generateBbox(ncnn::Mat score, ncnn::Mat location, vector<Bbox> &boundingBox_, float scale);

    void nmsTwoBoxs(vector<Bbox> &boundingBox_, vector<Bbox> &previousBox_,
                    const float overlap_threshold, string modelname = "Union");

    void nms(vector<Bbox> &boundingBox_, const float overlap_threshold, string modelname = "Union");

    void refine(vector<Bbox> &vecBbox, const int &height, const int &width, bool square);

    void extractMaxFace(vector<Bbox> &boundingBox_);

    void PNet(float scale);

    void PNet();

    void RNet();

    void ONet();

    ncnn::Net Pnet, Rnet, Onet;
    ncnn::Mat img;

    const float nms_threshold[3] = {0.5f, 0.7f, 0.7f};
    const float mean_vals[3] = {127.5, 127.5, 127.5};
    const float norm_vals[3] = {0.0078125, 0.0078125, 0.0078125};
    const int MIN_DET_SIZE = 12;
    std::vector<Bbox> firstPreviousBbox_, secondPreviousBbox_, thirdPrevioussBbox_;
    std::vector<Bbox> firstBbox_, secondBbox_, thirdBbox_;
    int img_w, img_h;

private://
    const float threshold[3] = {0.8f, 0.8f, 0.6f};
    int minsize = 40;
    const float pre_facetor = 0.709f;
    int num_threads = 1;
};


#endif //__MTCNN_NCNN_H__
