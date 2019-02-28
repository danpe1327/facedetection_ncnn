//
// Created by Operator on 2018/7/26.
//
#include <cstring>
#include "FDJniManage.h"
#include <common/log.h>

static JavaVM *g_JavaVm = NULL;

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    void *pEnv;
    LogInfo("JNI_OnLoad!");
    if (vm->GetEnv((void **) &pEnv, JNI_VERSION) != JNI_OK) {
        LogError("ERROR: GetEnv failed");
        return RET_FAIL;
    }
    g_JavaVm = vm;
    return JNI_VERSION;
}

char *jStringToChar(JNIEnv *env, jstring strValue) {
    char *pFuncName;
    const char *pTmp = env->GetStringUTFChars(strValue, 0);
    pFuncName = new char[strlen(pTmp) + 1];
    strcpy(pFuncName, pTmp);
    env->ReleaseStringUTFChars(strValue, pTmp);
    return pFuncName;
}

jstring charTojstring(JNIEnv *env, const char *pat)//change type char* into string
{
    return env->NewStringUTF(pat);
}