#include <jni.h>
#include <sys/prctl.h>

#include "native_main.h"

JavaVM* g_jvm = nullptr;

JNIEnv* AttachCurrentThread() {
  JNIEnv* env = nullptr;
  jint ret = g_jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_2);
  if (ret == JNI_EDETACHED || !env) {
    JavaVMAttachArgs args;
    args.version = JNI_VERSION_1_2;
    args.group = nullptr;

    // 16 is the maximum size for thread names on Android.
    char thread_name[16];
    int err = prctl(PR_GET_NAME, thread_name);
    if (err < 0) {
      args.name = nullptr;
    } else {
      args.name = thread_name;
    }

    ret = g_jvm->AttachCurrentThread(&env, &args);
  }
  return env;
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
  if (!g_jvm) {
    g_jvm = vm;
  }

  JNIEnv* env = AttachCurrentThread();
  bool reslut = native::NativeMain::Register(env);
  return JNI_VERSION_1_4;
}