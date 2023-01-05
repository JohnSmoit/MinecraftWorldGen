#pragma once
#ifndef GameRenderer_H
#define GameRenderer_H

#include "Game.h"

class ModelData
{
public:
	ModelData();
	//~ModelData();
	std::vector<float> verts;
	std::vector<int> tris;
	void MeshFromChunk(Chunk* c);
	void RenderObject();
	void SetShader(Shader* sh) { s = sh; }
	float* vertsReal;
	int* trisReal;
	bool done;
	//void UpdateChunkMesh();
	void Clear();
	//static void operator delete(void* ptr, std::size_t sz);
private:
	Chunk* p; //use later...
	void AddCubeAt(int x, int y, int z); // adds a cube to the model
	void AddFaceAt(glm::vec3 o, glm::vec3 localX, glm::vec3 localY, float localZOff, int face, int type);
	unsigned int VBO, EBO, VAO;
	int numTris;
	Shader* s;
	glm::vec3 transform;
	glm::mat4 model;
};

class GameRenderer 
{
public:
	GameRenderer() {}
	void createWindowContext(int w, int h, const char* t, int c_ma, int c_mi); // potentially add more options for window based on a settings object
	void ClearRenderer() {} // I think this one is when the game is closed what a terrible name oh well ium not ghcangint iwt.
	//void AddRenderObject(GameObject* g, unsigned int priority);
	void PrepareRender(); // do stuff before rendering the game like binding the proper buffer objects and stuff
	void RenderObjects();
	void PostRenderObjects() {} // for post processing n shit
	//void RenderObject(GameObject* p);
	unsigned int VAO;
	float aspectRatio, FOV, plane_clipNear, plane_clipFar;
	GLFWwindow* gameWindow;
private:
	unsigned int textureAtlas;
	std::vector<ModelData*> meshObjs;
	glm::vec2 ntc_texAtlasCoords(int blockType, int blockFace);
	//std::vector<RenderQueueObject> renderQueue; // a render queue object has a render object and a priority value.

};


#endif
