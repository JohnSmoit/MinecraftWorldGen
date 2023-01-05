#pragma once

#include "Game.h"

/*
* Player will need:
* To be able to handle input and transform itself apropriately ()
* Collide with the terrain 
* Place Blocks
* Break Blocks
* Have a Camera view. ()
*/

class Player 
{
public:
	Player(glm::vec3 startPos, glm::vec3 lookDir);
	void Init();
	void Update();
	void HandleInput();
	void SetPosition(glm::vec3 pos) {}
	void SetEulers(float yaw, float pitch, float roll);

	glm::mat4 cam_projectionMat;
	glm::mat4 cam_viewMat;

	glm::vec3 position;
	glm::vec2 relativeChunkPos;
private:
	glm::vec3 playerViewDir;
	glm::vec3 playerCameraEulers;
	glm::vec3 translate_offset;
};