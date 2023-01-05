#include "Game.h"
#include "GameRenderer.h"
#include "Player.h"
#include "World.h"
#include "SunSource.h"

bool Game::GladDead = false;
bool Game::WindowDead = false;

GameRenderer* Game::renderer = nullptr;
Player* Game::player = nullptr;
World* Game::world = nullptr;

float Game::lastFrame = 0.0f;
float Game::deltaTime = 0.0f;
SunSource Game::sun(glm::vec3(10.f, -10.0f, 10.0f), 5.3f, glm::vec3(0.8, 0.9, 1.0));

bool Game::Initialize(int w, int h, const char* t, int c_ma, int c_mi) 
{
	renderer = new GameRenderer();
	player = new Player(glm::vec3(0.0f, 40.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	world = new World();
	//sun = SunSource(glm::vec3(0.0f, 100.0f, 0.0f), 5.3f, glm::vec3(1.0f, 1.0f, 1.0f));
	RegisterBlocks();
	//sun = SunSource(glm::vec3(0.0f, 100.0f, 0.0f), 5.3f, glm::vec3(1, 1, 1));
	renderer->createWindowContext(w, h, t, c_ma, c_mi);

	if (Game::GladDead || Game::WindowDead) 
	{
		glfwTerminate();
		return false;
	}
	player->Init();
	world->Initialize();


	return true;
}

void Game::Update() 
{
	player->Update();
	world->Update();
	//sun.UpdateSun();
}

void Game::HandleInput() 
{
	player->HandleInput();
}

void Game::Render() 
{
	renderer->PrepareRender();
	renderer->RenderObjects();
	renderer->PostRenderObjects();
}

int Game::Run() 
{
	if (Initialize(800, 600, "Minecraft: Shite Edition", 3, 3)) 
	{
		while (_running)
		{
			float currentFrame = glfwGetTime();
			Game::deltaTime = currentFrame - lastFrame;
			Game::lastFrame = currentFrame;
			//std::cout << Game::deltaTime << std::endl;
			HandleInput();
			Update();
			Render();
			glfwPollEvents();
		}
		glfwTerminate();
		return 0;
	}
	else 
	{
		return -1;
	}
}

void Game::RegisterBlocks() 
{
	int offsets[6] = { 0, 0, 0, 0, 0, 0 };
	Block::CreateBlock(0, offsets, 0); // air is skipped
	Block::CreateBlock(1, offsets, 1); // stone
	Block::CreateBlock(2, offsets, 1); // dirt

	int topUnique[6] = {0, 1, 0, 0, -1, 0};
	Block::CreateBlock(3, topUnique, 2); // grass

	int topAndBottomUnique[6] = { 0, 1, 0, 0, 1, 0 };
	Block::CreateBlock(4, topAndBottomUnique, 2); // wood

	Block::CreateBlock(5, offsets, 1); // leaves
	Block::CreateBlock(6, offsets, 1); // bricks

}