//
// Created by Operator on 2018/7/26.
//

#ifndef ANDROIDAPP_FDJNIMANAGE_H
#define ANDROIDAPP_FDJNIMANAGE_H

#include <jni.h>

#define JNI_VERSION JNI_VERSION_1_4

char *jStringToChar(JNIEnv* env,jstring strValue);
jstring charTojstring(JNIEnv* env, const char* pChar);

#endif //ANDROIDAPP_FDJNIMANAGE_H
