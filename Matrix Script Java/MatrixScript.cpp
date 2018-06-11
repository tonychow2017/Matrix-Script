#include "MatrixScript.h"
#include "main_calculator.hpp"

JNIEXPORT jstring JNICALL Java_MatrixScript_calculateString(JNIEnv* env, jclass c, jstring s) {
    const char* exp = env->GetStringUTFChars(s,NULL);
    std::string result = calculate(std::string(exp));
    jstring returning = env->NewStringUTF(result.c_str());
    env->ReleaseStringUTFChars(s,exp);
    return returning;
}
