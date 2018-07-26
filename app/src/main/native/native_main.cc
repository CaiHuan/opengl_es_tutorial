#include "native_main.h"

#include <memory>
#include <GLES3/gl3.h>
#include <string>

#include "utils/shader_utils.h"

namespace native {
template <typename T, size_t N> char (&ArraySizeHelper(T (&array)[N]))[N];
#define arraysize(array) (sizeof(ArraySizeHelper(array)))

///////////////////////////////////////////////////////////////////////////////////////////////////
GLenum glCheckError_(const char *file, int line)
{
  GLenum errorCode;
  while ((errorCode = glGetError()) != GL_NO_ERROR)
  {
    std::string error;
    switch (errorCode)
    {
      case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
      case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
      case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
      //case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
      //case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
      case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
      case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
    }

    //std::cout << error << " | " << file << " (" << line << ")" << std::endl;
  }
  return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)
///////////////////////////////////////////////////////////////////////////////////////////////////
static const int kVertexPosSize = 3;
static const int kVertexColorSize = 4;
static const int kVertexPosIndex = 0;
static const int kVertexColorIndex = 1;
static const int kVertexStride = sizeof(GLfloat) * (kVertexPosSize + kVertexColorSize);

static const char DEFAULT_VERTEX_SHADER[] =
      "#version 320 es \n"
      "precision mediump float;\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = vec4(1.0f, 1.0f, 1.0f, 1.0);\n"
      "}\n";

static const char DEFAULT_FRAG_SHADER[] =
      "#version 320 es \n"
      "precision mediump float;\n"
      "out vec4 FragColor;\n"
      "void main()\n"
      "{\n"
      "   FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);\n"
      "}\n";

//set up vertex data( and buffer(s)) and configure vertex attributes
float vertices[] = {
  0.5f, 0.5f, 0.0f,   // 右上角
  0.5f, -0.5f, 0.0f,  // 右下角
  -0.5f, -0.5f, 0.0f, // 左下角
  -0.5f, 0.5f, 0.0f   // 左上角
};
unsigned int indices[] = {
  0, 1, 3,
  1, 2, 3
};

static const GLuint kPositionIndex = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////
static std::unique_ptr<NativeMain> g_native_main;

NativeMain::NativeMain() {}

NativeMain::~NativeMain() {
  glDeleteProgram(program_);
  glDeleteBuffers(2, vertex_buffer_object_);
  glDeleteVertexArrays(1, &vertex_array_object_);
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
  glCheckError();
  glClear(GL_COLOR_BUFFER_BIT);
  glCheckError();
  glBindVertexArray(vertex_array_object_);
  glCheckError();
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
  glCheckError();
  glBindVertexArray(0);
  glCheckError();

}

void NativeMain::OnSurfaceCreatedInternal(JNIEnv* env, jclass clazz, jobject bitmap) {
  program_ = CreateProgram(DEFAULT_VERTEX_SHADER, DEFAULT_FRAG_SHADER);
  if (!program_) {
    return;
  }

  glGenVertexArrays(1, &vertex_array_object_);
  glCheckError();
  glBindVertexArray(vertex_array_object_);
  glCheckError();
  glGenBuffers(2, vertex_buffer_object_);
  glCheckError();
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_[0]);
  glCheckError();
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glCheckError();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer_object_[1]);
  glCheckError();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glCheckError();
  glVertexAttribPointer(kPositionIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glCheckError();
  glEnableVertexAttribArray(kPositionIndex);
  glCheckError();
  glBindVertexArray(0);
  glCheckError();
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glCheckError();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glCheckError();
  glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
}

void NativeMain::OnSurfaceChangedInternal(JNIEnv* env, jclass clazz, jint width, jint height) {
  glViewport(0, 0, width, height);
}

} //namespace native