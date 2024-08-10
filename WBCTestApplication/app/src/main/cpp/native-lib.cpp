#include <jni.h>
#include <string>
#include "CHOWAES.h"
#include <android/log.h>

unsigned char* as_unsigned_char_array(JNIEnv *env, jbyteArray array);
 void as_jchar_array(JNIEnv *env, unsigned char in[], jbyteArray out);

extern "C" JNIEXPORT jstring JNICALL
Java_com_adri4n9_wbctestapplication_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */,
        jstring inp
        ) {
    const  char *nativeString = env->GetStringUTFChars(inp, 0);
    std::string hello = "Hello from C++";

    __android_log_write(ANDROID_LOG_ERROR, "Tag", "Error here");
    return env->NewStringUTF(hello.c_str());
}


extern "C" JNIEXPORT jint JNICALL
Java_com_adri4n9_wbctestapplication_MainActivity_encrypt(
        JNIEnv *env,
        jobject  jobj/* this */,
        jbyteArray input,
        jbyteArray output
) {
    unsigned char* in = as_unsigned_char_array(env, input);
    unsigned char* out = as_unsigned_char_array(env, output);
    aes128_enc_wb_final(in, out);
    as_jchar_array(env, out,output);
    __android_log_write(ANDROID_LOG_ERROR, "Tag", "Error here");
    return 0;
}

unsigned char* as_unsigned_char_array(JNIEnv *env, jbyteArray array) {
    int len = env->GetArrayLength (array);
    unsigned char* buf = new unsigned char[len];
    env->GetByteArrayRegion (array, 0, len, reinterpret_cast<jbyte*>(buf));
    return buf;
}

void as_jchar_array(JNIEnv *env, unsigned char in[], jbyteArray out) {
    int len = env->GetArrayLength (out);
    jbyte *body = env->GetByteArrayElements(out, JNI_FALSE);
    for (int i=0; i<len; i++) {
        body[i]= in[i];
    }
    env->ReleaseByteArrayElements(out,body,JNI_FALSE);
    return;
}