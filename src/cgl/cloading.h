#ifndef _CLOADING_H_
#define _CLOADING_H_

#include <GL/glx.h>
#include <GL/glext.h>

void cgl_load_opengl_funcs();

extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;

extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;

extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;

#endif //_CLOADING_H_
