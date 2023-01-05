#pragma once
#ifndef World_H
#define World_H

#include "Game.h"

class WorldGenerator;
class ModelData;

class Chunk
{

public:
	void Initialize();
	void SetBlockAt(int x, int y, int z, int type);
	void SetBlockRect(Rect r, int type);
	static bool IsSolid(int x, int y, int z, Chunk* c);

	void Destroy();
	ModelData* m;
	static const int CHUNK_X = 16;
	static const int CHUNK_Y = 256;
	static const int CHUNK_Z = 16;

	bool markedDirty; // only if a chunk is marked as dirty will it update the render info n stuff.
	bool generated;
	bool done;

	unsigned int blocks[CHUNK_X][CHUNK_Y][CHUNK_Z] = { 0 }; //array for all o da blockssssssssss...
	glm::vec3 chunkPos;
	int cRelativeCoordX, cRelativeCoordY;

	Chunk() {}
	~Chunk();
};



class World 
{
	WorldGenerator* generator;
	//probably need a worker class that listens for world updates and has callbacks for those sitcheeations.
public:
	void Initialize();
	void Update();
	void TryPlaceBlockOnChunk(int x, int y, int z, Chunk* c, int type);
	std::vector<Chunk*> worldChunks; // dictionary containing each loaded chunk in the world...
	std::map<glm::vec2, Chunk*, Utils::VectorComp> orderedChunks;
	std::map<glm::vec2, Chunk*, Utils::VectorComp> unloadedChunkObjs;
	World() {}
};



class Block 
{
public:
	static int totalAtlasOffset;
	static std::vector<Block*> blockTypes;
	int offsets[6];
	unsigned int ID;
	static void CreateBlock(unsigned int ID, int uniqueFaces[6], int totUnique);
	static int BlockTexOffsetOf(unsigned int ID, int face);
private:
	static void SortByID();
	Block() {}
};
//no templating just vector keys and Chunk* values...

#endif
