//
// Created by daniel on 7/18/18.
//
#include "shader_utils.h"

#include <jni.h>
#include <cstdlib>
#include <memory>

namespace native {
GLuint CreateShader(GLenum type, const char* shader_source) {
  GLuint shader = glCreateShader(type);
  if (!shader) {
    return 0;
  }
  glShaderSource(shader, 1, &shader_source, nullptr);
  glCompileShader(shader);

  //check the compile status
  GLint compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    GLint log_info_length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_info_length);
    if (1 < log_info_length) {
      std::unique_ptr<char[]> buffer(new char[log_info_length]);
      glGetShaderInfoLog(shader, log_info_length, nullptr, buffer.get());
    }
    glDeleteShader(shader);
    return 0;
  }

  return shader;
}

GLuint CreateProgram(const char* vertex_shader_source, const char* fragment_shader_source)  {
  int vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_source);
  if(!vertex_shader)
  {
    return 0;
  }

  int fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_source);
  if(!fragment_shader)
  {
    return 0;
  }

  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);
  GLint linked;
  glGetProgramiv(program, GL_LINK_STATUS, &linked);
  if (!linked) {
    GLint log_info_length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_info_length);
    if (1 < log_info_length) {
      std::unique_ptr<char[]> buffer(new char[log_info_length]);
      glGetProgramInfoLog(program, log_info_length, nullptr, buffer.get());
    }
    glDeleteProgram(program);
  }
  return program;
}
}
