//
// Created by Operator on 2018/7/26.
//

#ifndef ANDROIDAPP_JNIFACEDETECTION_H
#define ANDROIDAPP_JNIFACEDETECTION_H

#include "FDJniManage.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL
Java_com_danpe_facedetection_FaceDetection_LoadModels(JNIEnv *env, jobject instance,
                                                         jstring modelPath);

JNIEXPORT jint JNICALL
Java_com_danpe_facedetection_FaceDetection_DeleteModels(JNIEnv *env, jobject instance);

JNIEXPORT jintArray JNICALL
Java_com_danpe_facedetection_FaceDetection_Detect(JNIEnv *env, jobject instance,
                                                     jbyteArray jarrayJpegData,
                                                     jint imgHeight,
                                                     jint imgWidth);

JNIEXPORT jint JNICALL
Java_com_danpe_facedetection_FaceDetection_SetMinFaceSize(JNIEnv *env, jobject instance,
                                                             jint minSize);
JNIEXPORT jint JNICALL
Java_com_danpe_facedetection_FaceDetection_SetThreadsNumber(JNIEnv *env, jobject instance,
                                                               jint threadsNumber);

#ifdef  __cplusplus
}
#endif

#endif //ANDROIDAPP_JNIFACEDETECTION_H
