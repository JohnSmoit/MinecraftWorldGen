#include "Player.h"
#include "GameRenderer.h"
#include "World.h"


float lastX = 400;
float lastY = 300;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xPos, double yPos);

void mouse_callback(GLFWwindow* window, double xPos, double yPos) 
{
	if (firstMouse) 
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	const float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	Game::player->SetEulers(xOffset, yOffset, 0.0f);
}


Player::Player(glm::vec3 startPos, glm::vec3 lookDir) 
{
	position = startPos;
	playerViewDir = lookDir;


	cam_projectionMat = glm::mat4(1.0f);
	cam_viewMat = glm::mat4(1.0f);

	translate_offset = glm::vec3(0.0f);
}

void Player::Init() 
{
	cam_projectionMat = glm::mat4(1.0f);
	cam_viewMat = glm::mat4(1.0f);
	playerCameraEulers = glm::vec3(0.0f);

	glfwSetInputMode(Game::renderer->gameWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(Game::renderer->gameWindow, mouse_callback);
}

void Player::Update() 
{
	cam_projectionMat = glm::mat4(1.0f);
	cam_viewMat = glm::mat4(1.0f);

	position += translate_offset;


	if (playerCameraEulers.y > 89.0f)
		playerCameraEulers.y = 89.0f;
	if (playerCameraEulers.y < -89.0f)
		playerCameraEulers.y = -89.0f;

	playerViewDir.x = cos(glm::radians(playerCameraEulers.x)) * cos(glm::radians(playerCameraEulers.y));
	playerViewDir.y = sin(glm::radians(playerCameraEulers.y));
	playerViewDir.z = sin(glm::radians(playerCameraEulers.x)) * cos(glm::radians(playerCameraEulers.y));
	playerViewDir = glm::normalize(playerViewDir);

	cam_viewMat = glm::lookAt(position, position + playerViewDir, glm::vec3(0.0f, 1.0f, 0.0f));
	cam_projectionMat = glm::perspective(glm::radians(Game::renderer->FOV), Game::renderer->aspectRatio, Game::renderer->plane_clipNear, Game::renderer->plane_clipFar);

	//player direction recalculations
	//player chunk position
	relativeChunkPos = glm::vec2((int)(position.x / Chunk::CHUNK_X), (int)(position.z / Chunk::CHUNK_Z));
	//std::cout << relativeChunkPos.x << " " << relativeChunkPos.y << std::endl;
}

void Player::HandleInput() 
{
	float moveSpeed = 5.3f * Game::deltaTime;
	translate_offset = glm::vec3(0.0f, 0.0f, 0.0f);
	GLFWwindow* w = Game::renderer->gameWindow;
	if (glfwGetKey(w, GLFW_KEY_SPACE))
		moveSpeed *= 4.5f;
	if (glfwGetKey(w, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(w, true);
	if (glfwGetKey(w, GLFW_KEY_W))
		translate_offset += playerViewDir * moveSpeed;
	if (glfwGetKey(w, GLFW_KEY_S))
		translate_offset -= playerViewDir * moveSpeed;
	if (glfwGetKey(w, GLFW_KEY_A))
		translate_offset -= glm::normalize(glm::cross(playerViewDir, glm::vec3(0.0f, 1.0f, 0.0f))) * moveSpeed;
	if (glfwGetKey(w, GLFW_KEY_D))
		translate_offset += glm::normalize(glm::cross(playerViewDir, glm::vec3(0.0f, 1.0f, 0.0f))) * moveSpeed;
	if (glfwGetKey(w, GLFW_KEY_P))
		std::cout << relativeChunkPos.x << " " << relativeChunkPos.y << std::endl;
}

void Player::SetEulers(float yaw, float pitch, float roll) 
{
	playerCameraEulers += glm::vec3(yaw, pitch, roll);
}