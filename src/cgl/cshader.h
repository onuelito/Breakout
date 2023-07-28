#ifndef _CSHADER_H_
#define _CSHADER_H_

#include <GL/gl.h>

GLuint compile_shader(GLenum shaderType, const char* file_path);
GLuint create_program(const char *vsource, const char *fsource);

#endif //_CSHADER_H_
