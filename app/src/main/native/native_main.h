#include <jni.h>
#include <memory>
#include <GLES3/gl3.h>
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

  bool InitializeInternal(JNIEnv* env, jclass clazz);

  void OnDrawFrameInternal(JNIEnv* env, jclass clazz);

  void OnSurfaceCreatedInternal(JNIEnv* env, jclass clazz, jobject bitmap);

  void OnSurfaceChangedInternal(JNIEnv* env, jclass clazz, jint width, jint height);
private:
  NativeMain();

private:
  GLuint program_;
  GLint position_;
  GLint color_;
  GLint coordinate_;
  GLint texture_;
};
} //namespace native