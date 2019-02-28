#include <common/log.h>
#include "detect_picture.h"

using namespace std;

Detection::Detection(string modelPath) {
    mtcnn = new MTCNN(modelPath);
    vanface = new VanFace(modelPath);
}

Detection::Detection(const vector<string> param_files,
                     const vector<string> bin_files) {
    mtcnn = new MTCNN(param_files, bin_files);
    vanface = new VanFace(param_files[3].data(), bin_files[3].data());
}

void Detection::detect_picture(cv::Mat &image, vector<Bbox> &outFaceInfo) {
    ncnn::Mat ncnn_img = ncnn::Mat::from_pixels(image.data, ncnn::Mat::PIXEL_BGR2RGB, image.cols,
                                                image.rows);
    mtcnn->detect(ncnn_img, outFaceInfo);
    vanface->detectLandmark(image, outFaceInfo);
}

void Detection::SetMinFace(int i) {
    mtcnn->SetMinFace(i);
}

void Detection::SetNumThreads(int numThreads) {
    mtcnn->SetNumThreads(numThreads);
}
