//
// Created by Operator on 2019/2/26.
//

#ifndef FACEDETECTION_ANDROID_LOG_H
#define FACEDETECTION_ANDROID_LOG_H

#define RET_FAIL (-1)
#define RET_SUCCESS (0)

#if (defined ANDROID)

#include <android/log.h>
// log标签
#define  TAG    "FaceDetection"

// 定义info信息
#define LogInfo(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
// 定义debug信息
#define LogDebug(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
// 定义error信息
#define LogError(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)
#endif

#endif //FACEDETECTION_ANDROID_LOG_H
