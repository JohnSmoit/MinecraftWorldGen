#pragma once
#ifndef Shader_H
#define Shader_H

#include "stb_image.h"
#include <string>

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Game.h"

class Shader
{
public:
	unsigned int ID;
	//Shader() {}
	Shader(const char* vertexPath, const char* fragmentPath);

	void use();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, const glm::vec3& value) const; //too lazy to make a new struct for vectors...
	//void setVec3(const std::string& name, LaVec3 value) const;
};

#endif
