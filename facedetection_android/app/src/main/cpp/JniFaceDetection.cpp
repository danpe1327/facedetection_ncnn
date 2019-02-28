//
// Created by Operator on 2018/7/26.
//
#include <string>
#include <facedetection/detect_picture.h>
#include <common/log.h>
#include "JniFaceDetection.h"

static Detection *faceDetector = NULL;

/*****************************************JNI接口*********************************************/
JNIEXPORT jint JNICALL
Java_com_danpe_facedetection_FaceDetection_LoadModels(JNIEnv *env, jobject instance,
                                                         jstring modelPath) {
    if (NULL == modelPath) {
        return RET_FAIL;
    }
    char *inputModelPath = jStringToChar(env, modelPath);
    faceDetector = new Detection(inputModelPath);
    LogInfo("<---- Init face detection models success --->");
    return RET_SUCCESS;
}

JNIEXPORT jint JNICALL
Java_com_danpe_facedetection_FaceDetection_DeleteModels(JNIEnv *env, jobject instance) {
    if (faceDetector != NULL) {
        free(faceDetector);
        faceDetector = NULL;
        LogInfo("<---- Delete face detection models --->");
    } else {
        LogInfo("<---- Face detection models has been deleted --->");
        return RET_FAIL;
    }
    return RET_SUCCESS;
}

JNIEXPORT jintArray JNICALL
Java_com_danpe_facedetection_FaceDetection_Detect(JNIEnv *env, jobject instance,
                                                     jbyteArray jarrayJpegData, jint imgHeight,
                                                     jint imgWidth) {
    if (faceDetector == NULL) {
        return NULL;
    }
    jbyte *pJpegBuf = env->GetByteArrayElements(jarrayJpegData, 0);

    cv::Mat inputImage(imgHeight, imgWidth, CV_8UC4, (unsigned char *) pJpegBuf);
    cv::cvtColor(inputImage, inputImage, CV_RGB2BGR);

    std::vector<Bbox> faceInfos;
    faceDetector->detect_picture(inputImage, faceInfos);

    int32_t num_box = static_cast<int32_t>(faceInfos.size());
    LogInfo("人脸数目：%d\n", num_box);

    int outSize = 1 + num_box * 150;
    int *outFaceInfo = new int[outSize];
    outFaceInfo[0] = num_box;
    for (int i = 0; i < num_box; i++) {
        outFaceInfo[150 * i + 1] = faceInfos[i].x1;//left
        outFaceInfo[150 * i + 2] = faceInfos[i].y1;//top
        outFaceInfo[150 * i + 3] = faceInfos[i].x2;//right
        outFaceInfo[150 * i + 4] = faceInfos[i].y2;//bottom

        // 5 keypoints of mtcnn
        for (int j = 0; j < 10; j++) {
            outFaceInfo[150 * i + 5 + j] = static_cast<int>(faceInfos[i].ppoint[j]);
        }
        // 68 keypoints of vanface
        for (int k = 0; k < 136; k++) {
            outFaceInfo[150 * i + 5 + 10 + k] = static_cast<int>(faceInfos[i].landmarks[k]);
        }
    }

    jintArray tFaceInfo = env->NewIntArray(outSize);
    env->SetIntArrayRegion(tFaceInfo, 0, outSize, outFaceInfo);
    delete[] outFaceInfo;
    env->ReleaseByteArrayElements(jarrayJpegData, pJpegBuf, 0);
    inputImage.release();

    return tFaceInfo;
}

JNIEXPORT jint JNICALL
Java_com_danpe_facedetection_FaceDetection_SetMinFaceSize(JNIEnv *env, jobject instance,
                                                             jint minSize) {
    if (faceDetector == NULL) {
        LogError("Face detection models has not been initialization");
        return RET_FAIL;
    }

    if (minSize <= 20) {
        minSize = 20;
    }

    faceDetector->SetMinFace(minSize);
    return RET_SUCCESS;
}

JNIEXPORT jint JNICALL
Java_com_danpe_facedetection_FaceDetection_SetThreadsNumber(JNIEnv *env, jobject instance,
                                                               jint threadsNumber) {
    if(faceDetector == NULL){
        LogError("Face detection models has not been initialization");
        return RET_FAIL;
    }

    if(threadsNumber!=1&&threadsNumber!=2&&threadsNumber!=4&&threadsNumber!=8){
        LogError("Threads number must be 1, 2, 4 or 8");
        return RET_FAIL;
    }

    faceDetector->SetNumThreads(threadsNumber);
    return  RET_SUCCESS;
}