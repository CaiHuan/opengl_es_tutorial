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
  attribute vec3 aPosition;
  attribute vec4 aColor;
  attribute vec2 aCoordinate;
  varying vec4 coordColor;
  varying vec2 aVaryingCoordinate;

void main() {
  gl_Position = vec4(aPosition, 1.0);
  coordColor = aColor;
  aVaryingCoordinate = aCoordinate;
}
);

static const char DEFAULT_FRAG_SHADER[] = JNI_VALUE(
  precision highp float;
  uniform sampler2D sampler;
  varying vec2 aVaryingCoordinate;
  varying vec4 coordColor;


  void main(){
    gl_FragColor = texture2D(sampler, aVaryingCoordinate.xy);
  }
);


static const GLfloat VERTEX[] = {
  0.0f, 0.5f, 0.0f,
  -0.5f, -0.5f, 0.0f,
  0.5f, -0.5f, 0.0f};

static const GLfloat COLOR[] = {
  1.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f, 1.0f,
  0.0f, 0.0f, 1.0f, 1.0f,
};

static const GLfloat TEXTURE[] = {
  0.5f, 0.0f,
  0.0f, 1.0f,
  1.0f, 1.0f
};
///////////////////////////////////////////////////////////////////////////////////////////////////
static std::unique_ptr<NativeMain> g_native_main;

NativeMain::NativeMain() {}

NativeMain::~NativeMain() = default;

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

  glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, VERTEX);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void NativeMain::OnSurfaceCreatedInternal(JNIEnv* env, jclass clazz, jobject bitmap) {
  program_ = CreateProgram(DEFAULT_VERTEX_SHADER, DEFAULT_FRAG_SHADER);
  if (!program_) {
    return;
  }
  position_ = glGetAttribLocation(program_, "aPosition");
  color_ = glGetAttribLocation(program_, "aColor");
  coordinate_ = glGetAttribLocation(program_, "aCoordinate");
}

void NativeMain::OnSurfaceChangedInternal(JNIEnv* env, jclass clazz, jint width, jint height) {
  glViewport(0, 0, width, height);
}

} //namespace native