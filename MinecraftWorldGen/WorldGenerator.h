#pragma once
#ifndef WorldGenerator_H
#define WorldGenerator_H
#include "Game.h"

class WorldGenerator 
{
public:
	WorldGenerator() {}
	static float maxDistance;
	static int renderDistance;

	void Initialize(float maxLoadDist);
	void UpdateChunkRoster(); //make new chunks closer to the player while removing chunks further away...
	bool* hasChunks;
	void HandleUnGeneratedChunks();
	std::vector<std::thread> chunkGenThreads;
	siv::PerlinNoise noise;
private:
	void AddNewChunk(float posX, float posY, int Crc_x, int Crc_y);
	void GenerateChunk(Chunk* c);
	void addTreeeeeeeee(int x, int y, int z, Chunk* c);

};
#endif