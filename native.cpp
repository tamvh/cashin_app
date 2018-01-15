#if 0
#include <jni.h>

#include <QtAndroidExtras>
#include <android/log.h>

static const char kJniClassName[] {"com/vng/gbc/MyActivity"};
static const char kJTag[] {"USB_QSerialPort"};

static void jniDeviceNewData(JNIEnv *envA, jobject thizA, jint userDataA, jbyteArray dataA)
{
    Q_UNUSED(thizA);
    if (userDataA != 0)
    {
        jbyte *bytesL = envA->GetByteArrayElements(dataA, NULL);
        jsize lenL = envA->GetArrayLength(dataA);
        envA->ReleaseByteArrayElements(dataA, bytesL, JNI_ABORT);
    }
}


static JNINativeMethod javaMethods[] {
    {"nativeDeviceNewData",         "(I[B)V",                 reinterpret_cast<void *>(jniDeviceNewData)}
};

void cleanJavaException()
{
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
}
#if 1
// this method is called automatically by Java after the .so file is loaded
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
{
    __android_log_print(ANDROID_LOG_DEBUG, kJTag, "JNI_OnLoad");
    QAndroidJniEnvironment jniEnv;
    if (jniEnv->ExceptionCheck()) {
        jniEnv->ExceptionDescribe();
        jniEnv->ExceptionClear();
    }

    QAndroidJniObject javaClass(kJniClassName);
    if(!javaClass.isValid()) {
        __android_log_print(ANDROID_LOG_ERROR, kJTag, "Java class %s not valid", kJniClassName);
        return false;
    }

    jclass objectClass = jniEnv->GetObjectClass(javaClass.object<jobject>());
    jint val = jniEnv->RegisterNatives(objectClass, javaMethods, sizeof(javaMethods) / sizeof(javaMethods[0]));
    jniEnv->DeleteLocalRef(objectClass);


    if (jniEnv->ExceptionCheck()) {
        jniEnv->ExceptionDescribe();
        jniEnv->ExceptionClear();
    }

    __android_log_print(ANDROID_LOG_DEBUG, kJTag, "JNI_OnLoad load finish");
    return JNI_VERSION_1_6;
}
#endif


#endif
