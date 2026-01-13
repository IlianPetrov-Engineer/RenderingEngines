#pragma once
#include <glad/glad.h>
#include <string>

std::string readFileToString(const std::string& filePath);

GLuint generateShader(const std::string& shaderPath, GLuint shaderType);
