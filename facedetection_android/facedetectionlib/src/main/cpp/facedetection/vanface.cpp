//
// Created by danpe on 2019/02/18.
//
#include "vanface.h"
#include <string>

using namespace std;
using namespace cv;

VanFace::VanFace(const string &model_path) {
    vanFaceNet.use_int8_inference = 0;
    string param_file = model_path + "/VanFace.param";
    string bin_file = model_path + "/VanFace.bin";

    vanFaceNet.load_param(param_file.data());
    vanFaceNet.load_model(bin_file.data());
}

VanFace::VanFace(const string param_files, const string bin_files) {
    vanFaceNet.load_param(param_files.data());
    vanFaceNet.load_model(bin_files.data());
}

VanFace::~VanFace() {
    vanFaceNet.clear();
}

// Use the faces bbox detected by MTCNN generating the bbox for Vanface
void VanFace::getSenseBbox(vector<Bbox> &faceBbox, int imgHeight, int imgWidth,
                           vector<Rect> &senseBbox) {
    int num_box = faceBbox.size();
    senseBbox.resize(num_box);
    for (int i = 0; i < num_box; i++) {
        int faceH, faceW, faceX1, faceY1, faceX2, faceY2;
        faceH = faceBbox[i].y2 - faceBbox[i].y1 + 1;
        faceW = faceBbox[i].x2 - faceBbox[i].x1 + 1;

        faceX1 = (min)(faceBbox[i].ppoint[0], faceBbox[i].ppoint[3]) - 0.2 * faceW;
        faceY1 = (min)(faceBbox[i].ppoint[5], faceBbox[i].ppoint[6]) - 0.1 * faceH;
        faceX2 = (max)(faceBbox[i].ppoint[1], faceBbox[i].ppoint[4]) + 0.2 * faceW;
        faceY2 = (max)(faceBbox[i].ppoint[8], faceBbox[i].ppoint[8]) + 0.2 * faceH;

        faceX1 = (max)(0, faceX1);
        faceY1 = (max)(0, faceY1);
        faceX2 = (min)(imgWidth - 1, faceX2);
        faceY2 = (min)(imgHeight - 1, faceY2);

        senseBbox[i] = cv::Rect(faceX1, faceY1, faceX2 - faceX1 + 1, faceY2 - faceY1 + 1);
    }
}

void VanFace::detectLandmark(Mat &img_, vector<Bbox> &faceBbox) {
    img = img_;
    std::vector<cv::Rect> senseBbox;
    getSenseBbox(faceBbox, img.rows, img.cols, senseBbox);

    int face_id = 0;
    for (vector<Rect>::iterator it = senseBbox.begin(); it != senseBbox.end(); it++) {
        Mat srcROI(img, *it);
        Mat grayImg;

        cvtColor(srcROI, grayImg, CV_RGB2GRAY);
        grayImg.convertTo(grayImg, CV_32FC1);

        Mat inputImg = Mat(netSize, CV_32FC1);
        resize(grayImg, inputImg, netSize, 0, 0, INTER_CUBIC);

        Mat tmp_m, tmp_sd;
        double m = 0, sd = 0;
        meanStdDev(inputImg, tmp_m, tmp_sd); //Calculate the mean value and variance value
        m = tmp_m.at<double>(0, 0);
        sd = tmp_sd.at<double>(0, 0);

        inputImg = (inputImg - m) / (0.000001 + sd);

        // prepare data into array
        float *data = (float *) malloc(inputImg.rows * inputImg.cols * sizeof(float));
        int pix_count = 0;
        for (int i = 0; i < inputImg.rows; ++i) {
            for (int j = 0; j < inputImg.cols; ++j) {
                data[pix_count] = inputImg.at<float>(i, j);
                ++pix_count;
            }
        }

        ncnn::Mat ncnn_img = ncnn::Mat(netSize.width, netSize.height, data, 4);
        ncnn::Extractor ex = vanFaceNet.create_extractor();
        if (num_threads > 1) {
            ex.set_num_threads(num_threads);
        }
        ex.set_light_mode(true);
        ex.input("data", ncnn_img);
        ncnn::Mat keyPoint;

        ex.extract("Dense3", keyPoint);

        for (int i = 0; i < keyPoint.total() / 2; i++) {
            int pointX = int(keyPoint[2 * i] * (it->width) + it->x);
            int pointY = int(keyPoint[2 * i + 1] * (it->height) + it->y);
            faceBbox[face_id].landmarks[2 * i] = pointX;
            faceBbox[face_id].landmarks[2 * i + 1] = pointY;
        }

        free(data);
        face_id++;
    }
}
