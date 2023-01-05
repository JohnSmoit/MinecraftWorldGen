#include "World.h"
#include "WorldGenerator.h"
#include "GameRenderer.h"
#include <algorithm>

void World::Initialize() 
{
	worldChunks = std::vector<Chunk*>();
	orderedChunks = std::map<glm::vec2, Chunk*, Utils::VectorComp>();
	unloadedChunkObjs = std::map<glm::vec2, Chunk*, Utils::VectorComp>();
	generator = new WorldGenerator();
	generator->Initialize(48.0);
}



void World::TryPlaceBlockOnChunk(int x, int y, int z, Chunk* c, int type) //safe placement for structures that may be larger than 1 chunk
{
	glm::vec2 chunkOGVec = glm::vec2(c->cRelativeCoordX, c->cRelativeCoordY);
	glm::vec2 chunkQueryVec = glm::vec2(c->cRelativeCoordX, c->cRelativeCoordY); // bad I know...
	if (y > 255 || y < 0) // because no vertical chunks yet...
		return;
	if (x < 0)
		chunkQueryVec.x -= 1;
	else if (x > Chunk::CHUNK_X - 1)
		chunkQueryVec.x += 1;
	if (z < 0)
		chunkQueryVec.y -= 1;
	else if (z > Chunk::CHUNK_Z - 1)
		chunkQueryVec.y += 1;

	//std::cout << chunkQueryVec.x << ", " << chunkQueryVec.y << std::endl;

	//std::cout << y << std::endl;

	if (!(chunkOGVec == chunkQueryVec)) //if the block place request falls outside this chunks bounds...
	{
		//std::cout << "EE" << std::endl;
		int xReal = (x < 0) ? (Chunk::CHUNK_X + x % Chunk::CHUNK_X) : (x % Chunk::CHUNK_X); //ugh
		int zReal = (z < 0) ? (Chunk::CHUNK_Z + z % Chunk::CHUNK_Z) : (z % Chunk::CHUNK_Z);

		//std::cout << "x: " << xReal;
		//std::cout << "y: " << y;
		//std::cout << "z: " << zReal << std::endl;
		std::map<glm::vec2, Chunk*, Utils::VectorComp>::iterator it = unloadedChunkObjs.find(chunkQueryVec);
		if (!(it == unloadedChunkObjs.end())) // if CBO does exist..
		{


			//std::cout << "FF" << std::endl;
			it->second->blocks[xReal][y][zReal] = type;
		}
		else //if it doesnt, then just usmake an ewe that one...
		{
			Chunk* obj = new Chunk();
			obj->Initialize();
			obj->blocks[xReal][y][zReal] = type;
			unloadedChunkObjs.insert(std::pair<glm::vec2, Chunk*>(chunkQueryVec, obj));
		}
	}
	else 
	{
		//just place the block normally...
		c->blocks[x][y][z] = type;
	}
}

void World::Update() 
{
	generator->UpdateChunkRoster(); 
	generator->HandleUnGeneratedChunks();
}
void Chunk::Destroy() 
{
	if (m != nullptr)
		m->Clear();
	done = true;
	//delete this;
}
void Chunk::Initialize() 
{
	generated = false;
	markedDirty = true;
	m = nullptr;
}
std::vector<Block*> Block::blockTypes = std::vector<Block*>();
int Block::totalAtlasOffset = 0;
void Block::CreateBlock(unsigned int ID, int uniqueFaces[6], int totUnique)
{
	Block* b = new Block();
	b->ID = ID;
	b->offsets[0] = totalAtlasOffset + uniqueFaces[0];
	b->offsets[1] = totalAtlasOffset + uniqueFaces[1];
	b->offsets[2] = totalAtlasOffset + uniqueFaces[2];
	b->offsets[3] = totalAtlasOffset + uniqueFaces[3];
	b->offsets[4] = totalAtlasOffset + uniqueFaces[4];
	b->offsets[5] = totalAtlasOffset + uniqueFaces[5];

	totalAtlasOffset += totUnique;
	blockTypes.push_back(b);

	//Block::SortByID();
}

int Block::BlockTexOffsetOf(unsigned int ID, int face) 
{
	//std::cout << ID << " " << blockTypes.size() << std::endl;
	return Block::blockTypes[ID]->offsets[face];
}

void Block::SortByID() // bubble sort cuz I'm lazy
{
	for (int i = 0; i < blockTypes.size(); i++) 
	{
		for (int j = 0; j < blockTypes.size(); j++) 
		{
			if (blockTypes[i] > blockTypes[j]) 
			{
				Block* temp = blockTypes[i];
				blockTypes[i] = blockTypes[j];
				blockTypes[j] = temp;
			}
		}
	}
}

bool Chunk::IsSolid(int x, int y, int z, Chunk* c) 
{
	if (z < 0 || z >= Chunk::CHUNK_Z) 
		return false;
	if (y < 0 || y >= Chunk::CHUNK_Y)
		return false;
	if (x < 0 || x >= Chunk::CHUNK_X)
		return false;

	if (c->blocks[x][y][z] != 0) 
	{
		return true;
	}
	return false;
}

Chunk::~Chunk() 
{
	delete m;
}
