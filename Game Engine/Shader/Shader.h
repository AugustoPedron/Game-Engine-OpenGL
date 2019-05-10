#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <exception>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	GLuint program;
	Shader();
	Shader(const GLchar *vertexPath, const GLchar *fragmentPath);
	void use();
	inline void Set1i(const GLchar* attribute, const GLint value) { 
		glUniform1i(glGetUniformLocation(this->program, attribute), value); 
	}

	inline void Set1f(const GLchar* attribute, const GLfloat value) { 
		glUniform1f(glGetUniformLocation(this->program, attribute), value); 
	}

	inline void SetVec3(const GLchar* attribute, const GLfloat val1, const GLfloat val2, const GLfloat val3) { 
		glUniform3f(glGetUniformLocation(this->program, attribute), val1, val2, val3); 
	}

	inline void SetMat4(const GLchar* attribute, const glm::mat4 matrix) { 
		glUniformMatrix4fv(glGetUniformLocation(this->program, attribute), 1, GL_FALSE, glm::value_ptr(matrix)); 
	}
};