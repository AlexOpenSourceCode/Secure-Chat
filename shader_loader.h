#pragma once

#include "GL/glew.h"
#include "GL/freeglut.h"
#include <iostream>

namespace Shaders
{

	class Shader_Loader
	{
	private:

		std::string ReadShader(char *filename);
		GLuint CreateShader(GLenum shaderType,
			std::string source,
			char* shaderName);

	public:

		Shader_Loader(void);
		~Shader_Loader(void);
		GLuint CreateProgram(char* VertexShaderFilename,
			char* FragmentShaderFilename);

	};
}