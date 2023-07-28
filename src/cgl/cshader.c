#include "cloading.h"
#include "cshader.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *get_file_content(const char *file_path)
{
	char *fdat;
	FILE *fptr;
	long size = 0;

	fptr = fopen(file_path, "rb");
	if(fptr == NULL) 
		return "";

	fseek(fptr, 0L, SEEK_END);
	size = ftell(fptr) + 1;
	fclose(fptr);


	fptr = fopen(file_path, "r");
	fdat = memset(malloc(size), '\0', size);
	fread(fdat, 1, size-1, fptr);
	fclose(fptr);

	return fdat;
}

GLuint compile_shader(GLenum shaderType, const char* file_path)
{
	int success;
	char infoLog[512];

	const char* shaderSource = get_file_content(file_path);

	GLuint shaderId = glCreateShader(shaderType);
	if (shaderId == 0) 
		return shaderId;

	glShaderSource(shaderId, 1, (const char**)&shaderSource, NULL);
	glCompileShader(shaderId);
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		glDeleteShader(shaderId);
		printf("Could not compile shader: %s\n", infoLog);
		return shaderId;
	}

	return shaderId;
}

GLuint create_program(const char *vsource, const char *fsource)
{
	GLuint vshader = compile_shader(GL_VERTEX_SHADER, vsource);
	GLuint fshader = compile_shader(GL_FRAGMENT_SHADER, fsource);

	GLuint program = glCreateProgram();

	glAttachShader(program, vshader);
	glAttachShader(program, fshader);
	
	glLinkProgram(program);
	glDeleteShader(vshader);
	glDeleteShader(fshader);

	//Debugging
	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if(!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		printf("Could not create program %s\n", infoLog);
	}

	return program;
}
