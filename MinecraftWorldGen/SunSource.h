#pragma once
#ifndef SunSource_H
#define SunSource_H
#include "Game.h"
class SunSource //if only I wasn't too lazy to do a gameobject-based ecs lol.
{
public:
	SunSource(glm::vec3 startPos, float bright, glm::vec3 col) { direction = startPos; brightness = bright; colRGB = col; }
	glm::vec3 direction;
	float brightness;
	glm::vec3 colRGB;
	void InitSun();
	void DrawSun();
	//void UpdateSun() { direction.x *= cosf((int)glfwGetTime() % 360); direction.y *= sinf((int)glfwGetTime() % 360); std::cout << direction.x << std::endl; }
};

#endif