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

  void DrawPrimitiveWithoutVBOs(GLfloat* vertices, GLint vertex_stride, GLint number_indices, GLushort* indices);
private:
  NativeMain();

private:
  GLuint program_;

  GLuint vertex_array_object_;
  GLuint vertex_buffer_object_[2];
};
} //namespace native