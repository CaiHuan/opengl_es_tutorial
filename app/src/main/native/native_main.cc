#include "native_main.h"

#include <memory>

namespace native {
template <typename T, size_t N> char (&ArraySizeHelper(T (&array)[N]))[N];
#define arraysize(array) (sizeof(ArraySizeHelper(array)))
///////////////////////////////////////////////////////////////////////////////////////////////////
static std::unique_ptr<NativeMain> g_native_main;

NativeMain::NativeMain() {}

NativeMain::~NativeMain() = default;

NativeMain& NativeMain::Get() {
  *g_native_main;
}
bool NativeMain::Register(JNIEnv *env) {
  static const JNINativeMethod methods[] = {
    {
      .name = "nativeInit",
      .signature = "()Z",
      .fnPtr = reinterpret_cast<void*>(&Initialize),
    },
    {
      .name = "nativeOnDrawFrame",
      .signature = "()V",
      .fnPtr = reinterpret_cast<void*>(&OnDrawFrame),
    },
    {
      .name = "nativeOnSurfaceCreated",
      .signature = "(Landroid/graphics/Bitmap;)V",
      .fnPtr = reinterpret_cast<void*>(&OnSurfaceCreated),
    },
    {
      .name = "nativeOnSurfaceChanged",
      .signature = "(II)V",
      .fnPtr = reinterpret_cast<void*>(&OnSurfaceChanged),
    },
  };

  jclass clazz = env->FindClass("com/google/daniel/opengl_es_tutorial/MainActivity");
  if (nullptr == clazz) {
    return false;
  }
  return 0 == env->RegisterNatives(clazz, methods, arraysize(methods));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//static function
jboolean NativeMain::Initialize(JNIEnv* env, jclass clazz) {
  g_native_main.reset(new NativeMain());
  return true;
}

void NativeMain::OnDrawFrame(JNIEnv *env, jclass clazz) {
}

void NativeMain::OnSurfaceCreated(JNIEnv* env, jclass clazz, jobject bitmap) {

}

void NativeMain::OnSurfaceChanged(JNIEnv* env, jclass clazz, jint width, jint height) {

}
///////////////////////////////////////////////////////////////////////////////////////////////////
}