#pragma once

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Shader
{
public:
	//шейдерная программа
	GLuint Program;
	//cчитывание шейдеров и их сборка
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath) { 
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		//проверка на выброс исключений
		vShaderFile.exceptions(std::ifstream::failbit);
		fShaderFile.exceptions(std::ifstream::failbit);
		try {
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream;
			std::stringstream fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			vShaderFile.close();
			fShaderFile.close();
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure fail) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode = fragmentCode.c_str();

		GLuint vertexShader;
		GLuint fragmentShader;
		GLint success;
		GLchar infoLog[512];

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vShaderCode, NULL);
		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertexShader);
		glAttachShader(this->Program, fragmentShader);
		glLinkProgram(this->Program);
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(this->Program, sizeof(infoLog), NULL, infoLog);
			std::cout << "ERROR::SHADERPROGRAM::LINK::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	//использование шейдерной программы
	void use() { 
		glUseProgram(this->Program);
	}
};
