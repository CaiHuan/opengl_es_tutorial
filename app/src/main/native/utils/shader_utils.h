//
// Created by daniel on 7/18/18.
//

#ifndef OPENGL_ES_TUTORIAL_SHADER_UTILS_H
#define OPENGL_ES_TUTORIAL_SHADER_UTILS_H

#include <GLES3/gl3.h>

namespace native {

GLuint CreateShader(GLenum type, const char shader[]);

GLuint CreateProgram(const char v[], const char f[]);
}
#endif //OPENGL_ES_TUTORIAL_SHADER_UTILS_H
