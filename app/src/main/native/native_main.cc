#include "native_main.h"

#include <memory>
#include <GLES3/gl3.h>

#include "utils/shader_utils.h"

namespace native {
template <typename T, size_t N> char (&ArraySizeHelper(T (&array)[N]))[N];
#define arraysize(array) (sizeof(ArraySizeHelper(array)))
///////////////////////////////////////////////////////////////////////////////////////////////////
#define JNI_VALUE(x) #x
static const char DEFAULT_VERTEX_SHADER[] = JNI_VALUE(
  precision highp float;
  attribute vec3 a_position;
  attribute vec4 a_color;
  varying vec4 coordinate_color;
  void main() {
    gl_Position = vec4(a_position, 1.0);
    coordinate_color = a_color;
  }
);

static const char DEFAULT_FRAG_SHADER[] = JNI_VALUE(
  precision highp float;
  varying vec4 coordinate_color;
  void main() {
    gl_FragColor = coordinate_color;
  }
);


static const GLfloat VERTEX[] = {
  0.0f, 0.5f, 0.0f,
  -0.5f, -0.5f, 0.0f,
  0.5f, -0.5f, 0.0f};

static const GLfloat COLOR[] = {
  1.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 1.0f, 1.0f,
};

///////////////////////////////////////////////////////////////////////////////////////////////////
static std::unique_ptr<NativeMain> g_native_main;

NativeMain::NativeMain() {}

NativeMain::~NativeMain() {
  glDeleteProgram(program_);
}

NativeMain& NativeMain::Get() {
  return *g_native_main;
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
  return Get().InitializeInternal(env, clazz);
}

void NativeMain::OnDrawFrame(JNIEnv *env, jclass clazz) {
  Get().OnDrawFrameInternal(env, clazz);
}

void NativeMain::OnSurfaceCreated(JNIEnv* env, jclass clazz, jobject bitmap) {
  Get().OnSurfaceCreatedInternal(env, clazz, bitmap);
}

void NativeMain::OnSurfaceChanged(JNIEnv* env, jclass clazz, jint width, jint height) {
  Get().OnSurfaceChangedInternal(env, clazz, width, height);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
bool NativeMain::InitializeInternal(JNIEnv* env, jclass clazz) {
  return true;
}

void NativeMain::OnDrawFrameInternal(JNIEnv* env, jclass clazz) {
  glUseProgram(program_);

  glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glEnableVertexAttribArray(position_index_);
  glEnableVertexAttribArray(color_index_);
  glVertexAttribPointer(position_index_, 3, GL_FLOAT, GL_FALSE, 0, VERTEX);

  glVertexAttribPointer(color_index_, 4, GL_FLOAT, GL_FALSE, 0, COLOR);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableVertexAttribArray(position_index_);
  glDisableVertexAttribArray(color_index_);
}

void NativeMain::OnSurfaceCreatedInternal(JNIEnv* env, jclass clazz, jobject bitmap) {
  program_ = CreateProgram(DEFAULT_VERTEX_SHADER, DEFAULT_FRAG_SHADER);
  if (!program_) {
    return;
  }
  position_index_ = glGetAttribLocation(program_, "a_position");
  color_index_ = glGetAttribLocation(program_, "a_color");
  coordinate_index_ = glGetAttribLocation(program_, "a_coordinate");
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void NativeMain::OnSurfaceChangedInternal(JNIEnv* env, jclass clazz, jint width, jint height) {
  glViewport(0, 0, width, height);
}

} //namespace native