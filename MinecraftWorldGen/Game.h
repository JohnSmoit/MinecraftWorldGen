#pragma once
#ifndef Game_H
#define Game_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Noise.hpp"
#include <thread>

class GameRenderer; //Renders all chunks in the game as well as \n stuff like clouds and skyboxes if I get there (which I shouldn't do)
class World;
class Player; //player is duh
class BlockRegistry;
class Block;
class Chunk;
class Shader;
class SunSource;

class Game 
{
	bool _running = true;
	bool Initialize(int w, int h, const char* t, int c_ma, int c_mi);
	void HandleInput();
	void Update();
	void Render();
	void ShutDown();
	static float lastFrame;
	void RegisterBlocks();
public:
	//std::vector<GameObject*> gameObjects;
	static bool GladDead;
	static bool WindowDead;
	static Game* instance;
	static SunSource sun;
	//Camera* mainCamera; // so that the game's objects have access to the camera's matrices
	//std::map<glm::vec3, Chunk*> gameChunks; //each game object will have a connected render object which will be rendered by the renderer...
	static GameRenderer* renderer; //handles all of the game's rendering and contains the window context...
	static World* world;
	static BlockRegistry* blocks; // this shit screams memory leak... fix later...
	static Player* player;
	static float deltaTime;
	//PhysicsLord physics; // basic physics like gravity and AABB collisions
	//WorldGenerator* generator; //generates the world (with a pregame generation and a in-world generation)
	Game() {}
	int Run();
protected:
	//BlockRegistry blockRegister;
};

typedef struct Rect
{
	int x, y, w, h;
} Rect;

enum BlockFace 
{
	FACE_FRONT,
	FACE_TOP,
	FACE_RIGHT,
	FACE_BACK,
	FACE_BOTTOM,
	FACE_LEFT
};
class Utils 
{
public:
	static const char* CStringOf(std::string s) 
	{
		return s.c_str();
	}

	static Rect rect(int x, int y, int w, int h) 
	{
		Rect r;
		r.x = x;
		r.y = y;
		r.w = w;
		r.h = h;
		return r;
	}
	
	static float magnitude(glm::vec3 vec) 
	{
		return sqrtf(sqrtf(vec.x * vec.x + vec.y * vec.y) * sqrtf(vec.x * vec.x + vec.y * vec.y) + vec.z * vec.z);
	}
	static float magnitude2(glm::vec2 vec) 
	{
		return sqrtf(vec.x * vec.x + vec.y * vec.y);
	}
	static float fbm2D(float x, float y, float ampMult, float freqMult, int octaves, float startAmp, float startFreq, siv::PerlinNoise* noise) 
	{
		float result = 0.0f;
		float freq = startFreq;
		float ampl = startAmp;

		for (int i = 0; i < octaves; i++) 
		{
			result += noise->noise2D_0_1(freq * x, freq * y) * ampl;
			freq *= freqMult;
			ampl *= ampMult;
		}
		return result / octaves;
	}
	static float fbm3D(float x, float y, float z, float ampMult, float freqMult, int octaves, float startAmp, float startFreq, siv::PerlinNoise* noise)
	{
		float result = 0.0f;
		float freq = startFreq;
		float ampl = startAmp;

		for (int i = 0; i < octaves; i++)
		{
			result += noise->noise3D_0_1(freq * x, freq * y, freq * z) * ampl;
			freq *= freqMult;
			ampl *= ampMult;
		}
		return result / octaves;
	}
	static struct VectorComp
	{
		bool operator() (const glm::vec2& lhs, const glm::vec2& rhs) const
		{
			if (lhs.x < rhs.x)
				return true;
			if (rhs.x < lhs.x)
				return false;

			// a1==b1: continue with element 2
			if (lhs.y < rhs.y)
				return true;
			if (rhs.y < lhs.y)
				return false;
			return false;
		}
	};
	static bool RandomChance(float chance) 
	{
		//std::cout << ((rand() % 100000) / 100000.f) << std::endl;
		return (RandInRangei(0, 100000) / 100000.0f) <= chance;
	}
	static int RandInRangei(int min, int max) 
	{
		return (rand() % (max - min + 1) + min);
	}
private:

};

#endif
