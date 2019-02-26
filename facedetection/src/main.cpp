#include <opencv2/opencv.hpp>
#include "mtcnn.h"
#include "vanface.h"


void test_picture()
{
    const char *model_path = "models";
    MTCNN mtcnn(model_path);
    VanFace vanface(model_path);

    cv::Mat image;
    image = cv::imread("sample.jpg");
    ncnn::Mat ncnn_img = ncnn::Mat::from_pixels(image.data, ncnn::Mat::PIXEL_BGR2RGB, image.cols, image.rows);

    clock_t start_time = clock();

    std::vector<Bbox> finalBbox;
    mtcnn.detect(ncnn_img, finalBbox);

    const int num_box = finalBbox.size();
    std::vector<cv::Rect> bbox;
    bbox.resize(num_box);
    for (int i = 0; i < num_box; i++)
    {
        bbox[i] = cv::Rect(finalBbox[i].x1, finalBbox[i].y1, finalBbox[i].x2 - finalBbox[i].x1 + 1, finalBbox[i].y2 - finalBbox[i].y1 + 1);
    }

    std::vector<std::vector<cv::Point>> faceLandmarks;
    vanface.detectLandmark(image, finalBbox, faceLandmarks);

    for (int i = 0; i < num_box; i++)
    {
        cv::rectangle(image, (bbox[i]), cv::Scalar(0, 0, 255), 2, 8, 0);
        /*for (int j = 0; j < 5; j = j + 1)
        {
            cv::circle(image, cvPoint(finalBbox[i].ppoint[j], finalBbox[i].ppoint[j + 5]), 2, CV_RGB(0, 255, 0), CV_FILLED);
        }*/
        for (int k = 0; k < faceLandmarks[i].size(); k++)
        {
            cv::circle(image, cvPoint(faceLandmarks[i][k].x, faceLandmarks[i][k].y), 2, CV_RGB(255, 255, 0), CV_FILLED);
        }
    }

    clock_t finish_time = clock();
    double total_time = (double)(finish_time - start_time) / CLOCKS_PER_SEC;
    std::cout << "time" << total_time * 1000 << "ms" << std::endl;

    cv::imshow("face_detection", image);
    cv::waitKey(0);
}


int main(int argc, char **argv)
{
    test_picture();
    exit(0);
}