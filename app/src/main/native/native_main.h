#include <jni.h>
#include <memory>

namespace native {

class NativeMain {
public:
  static NativeMain& Get();

  static bool Register(JNIEnv* env);

  static jboolean Initialize(JNIEnv* env, jclass clazz);

  static void OnDrawFrame(JNIEnv* env, jclass clazz);

  static void OnSurfaceCreated(JNIEnv* env, jclass clazz, jobject bitmap);

  static void OnSurfaceChanged(JNIEnv* env, jclass clazz, jint width, jint height);

  ~NativeMain();

private:
  NativeMain();
};
}