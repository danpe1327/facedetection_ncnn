package com.danpe.facedetection;

public class FaceDetection {
    static {
        System.loadLibrary("opencv_java3");
        System.loadLibrary("facedetection");
    }

    public native int LoadModels(String modelName);

    public native int DeleteModels();

    //检测的最小人脸设置
    public native int SetMinFaceSize(int minSize);

    //线程设置
    public native int SetThreadsNumber(int threadsNumber);

    public native int[] Detect(byte[] imgData, int imgHeight, int imgWidth);

}
