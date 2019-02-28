//
// Created by danpe on 2019/2/26.
//

#ifndef FACEDETECTION_ANDROID_DETECT_PICTURE_H
#define FACEDETECTION_ANDROID_DETECT_PICTURE_H

#include <opencv2/opencv.hpp>
#include "mtcnn.h"
#include "vanface.h"

class Detection {
public:
    Detection(const std::string modelPath);

    Detection(const std::vector<std::string> param_files, const std::vector<std::string> bin_files);

    ~Detection();

    void detect_picture(cv::Mat &image, std::vector<Bbox> &outFaceInfo);

    void SetMinFace(int i);

    void SetNumThreads(int numThreads);

protected:
private:
    MTCNN *mtcnn;
    VanFace *vanface;
};


#endif //FACEDETECTION_ANDROID_DETECT_PICTURE_H
