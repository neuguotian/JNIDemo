#include <jni.h>
#include <string>
#include <android/log.h>

const char *LOG_TGA = "LOG_TGA";

#define JNIREG_CLASS "com/permission/cmakedynamictest/MainActivity"

// 定义java层对应的package名称


// Native方法实现
static jstring stringFromJNI(JNIEnv *env, jobject classz) {
    std::string hello = "Hello from C++";
    jclass clazz = env->FindClass(JNIREG_CLASS);
    jmethodID id = env->GetStaticMethodID(clazz, "callback", "(Ljava/lang/String;)V");
    env->CallStaticVoidMethod(clazz, id, env->NewStringUTF(hello.c_str()));
    return env->NewStringUTF(hello.c_str());
}

// Java层和Native层的方法签名
static JNINativeMethod nativeMethods[] = {
        {"stringFromJNI", "()Ljava/lang/String;", (jstring *) stringFromJNI},
};

// 注册native方法
jint register_native_methods(JNIEnv *env) {
    jclass clazz = env->FindClass(JNIREG_CLASS);
    if (clazz == NULL) {
        __android_log_print(ANDROID_LOG_DEBUG, LOG_TGA, "-----------------------hello native log");
        return JNI_FALSE;
    }

    if (env->RegisterNatives(clazz, nativeMethods,
                             sizeof(nativeMethods) / sizeof(nativeMethods[0])) < 0) {
        env->DeleteLocalRef(clazz);
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

// 重写JNI_OnLoad方法
extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    if (JNI_TRUE != register_native_methods(env)) {
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}