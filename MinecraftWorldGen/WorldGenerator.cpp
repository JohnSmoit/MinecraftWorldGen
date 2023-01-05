#include "WorldGenerator.h"
#include "World.h"
#include "Player.h"

float WorldGenerator::maxDistance = 0.0f;
int WorldGenerator::renderDistance = 0;

void WorldGenerator::Initialize(float maxLoadDist) 
{
	int x = maxLoadDist / Chunk::CHUNK_X;
	int y = maxLoadDist / Chunk::CHUNK_Z;
	int p = x * 2;
	int z = y * 2;
	p *= z;
	//std::cout << x << " " << y << std::endl;
	hasChunks = new bool[p];
	noise = siv::PerlinNoise(time(NULL));
	chunkGenThreads = std::vector<std::thread>();
	maxDistance = maxLoadDist;
	renderDistance = x;

}

void WorldGenerator::addTreeeeeeeee(int x, int y, int z, Chunk* c)
{


	//if there is no block underneath, don't generate a tree. I mean, of course there'd be no floating trees in minecraft, RIGHT?
	const float det = 0.3f;
	if (c->blocks[x][y-1][z] == 0) 
		return;

	int height = Utils::RandInRangei(4, 7);

	int z_l = Utils::RandInRangei(0, 4);
	int z_l2 = z_l + Utils::RandInRangei(2, 3);

	int y_l = Utils::RandInRangei(3, 4);
	//std::cout << y_l << std::endl;

	for (int i = 0; i < height; i++) 
	{
		c->blocks[x][y + i][z] = 4;
	}


	for (int xx = -z_l; xx < z_l + 1; xx++)
	{
		//std::cout << (0 < y_l) << std::endl;
		for (int yy = 0; yy < y_l + 1; yy++)
		{
			for (int zz = -z_l; zz < z_l + 1; zz++)
			{

				Game::world->TryPlaceBlockOnChunk(x + xx, y + (height - y_l) + yy, z + zz, c, 5);
				//std::cout << Utils::RandomChance((float)(abs(xx) + abs(zz)) / (z_l * 2.0f) * det) << std::endl;
				if (Utils::RandomChance((float)(abs(xx) + abs(zz)) / (z_l * 2.0f) * det))
				{
					Game::world->TryPlaceBlockOnChunk(x + xx, y + (height - y_l) + yy, z + zz, c, 0);
				}

			}
		}
	}
}

void WorldGenerator::AddNewChunk(float posX, float posY, int Crc_x, int Crc_y) 
{
	//int x = ((int)posX % (int)maxDistance) / Chunk::CHUNK_X; // chunks pivot around the player ye? so lets just base the x y calculations based on that. I am really trying to avoid using hashtables for some reason.
	//std::cout << x << std::endl;
	Chunk* c = new Chunk();
	c->Initialize();
	c->cRelativeCoordX = Crc_x;
	c->cRelativeCoordY = Crc_y;
	c->chunkPos = glm::vec3(posX, 0.0f, posY);
	Game::world->worldChunks.push_back(c);
	Game::world->orderedChunks.insert(std::pair<glm::vec2, Chunk*>(glm::vec2(c->cRelativeCoordX, c->cRelativeCoordY), c));
}

void WorldGenerator::HandleUnGeneratedChunks() 
{
	chunkGenThreads.clear();
	for (Chunk* c : Game::world->worldChunks) 
	{
		if (!c->generated) 
		{
			chunkGenThreads.push_back(std::thread(&WorldGenerator::GenerateChunk, this, c));
		}
	}
	for (auto &th : chunkGenThreads) //surprisingly, this speeds stuff up considerably...
	{
		th.join();
	}
}

void WorldGenerator::UpdateChunkRoster() 
{
	//clear far away chunks
	for (std::vector<Chunk*>::iterator i = Game::world->worldChunks.begin(); i != Game::world->worldChunks.end(); ++i) 
	{
		if (abs(Utils::magnitude2(Game::player->relativeChunkPos - glm::vec2((*i)->cRelativeCoordX, (*i)->cRelativeCoordY)) > renderDistance+2))
		{
			(*i)->Destroy();
			//std::cout << "Deleting Chunks at: " << (*i)->cRelativeCoordX << " " << (*i)->cRelativeCoordY << std::endl;
			Game::world->orderedChunks.erase(glm::vec2((*i)->cRelativeCoordX, (*i)->cRelativeCoordY));
			i = Game::world->worldChunks.erase(i);
			if (i == Game::world->worldChunks.end()) 
			{
				break;
			}
		}
	}
	//create new chunks where they are needed.
	for (int i = Game::player->relativeChunkPos.x - renderDistance; i < Game::player->relativeChunkPos.x + renderDistance; i++) 
	{
		for (int j = Game::player->relativeChunkPos.y - renderDistance; j < Game::player->relativeChunkPos.y + renderDistance; j++)
		{
			//std::cout << i << " " << j << std::endl;
			if (!Game::world->orderedChunks.count(glm::vec2(i, j))) 
			{
				//std::cout << "Making new chunkkkkk at: " << i << " " << j << std::endl;
				AddNewChunk(i * Chunk::CHUNK_X, j * Chunk::CHUNK_Z, i, j);
			}
		}
	}
}

void WorldGenerator::GenerateChunk(Chunk* c) //more generation stuff later
{
	for (int i = 0; i < Chunk::CHUNK_X; i++) 
	{
		for (int j = 0; j < Chunk::CHUNK_Z; j++) 
		{
			float poop = Utils::fbm2D(c->chunkPos.x + i, c->chunkPos.z + j, 0.56f, 1.25f, 8, 12.0f, 0.035f, &noise);
			//float poop = noise.accumulatedOctaveNoise2D_0_1(c->chunkPos.x + i, c->chunkPos.y + j, 8);
			int heightY = 40 + poop * 20;
			//std::cout << heightY << std::endl;
			for (int k = 0; k < heightY; k++) 
			{

				float poop2 = Utils::fbm3D(c->chunkPos.x + i, c->chunkPos.y + k, c->chunkPos.z + j, 0.56f, 1.25f, 8, 12.0f, 0.035f, &noise);
				if (heightY - k < 2)
				{
					c->blocks[i][k][j] = 3;
				}
				else if (heightY - k < 6) 
				{
					c->blocks[i][k][j] = 2;
				}
				else if (heightY - k >= 6)
				{
					c->blocks[i][k][j] = 1;
				}

				if (poop2 < 1.3f) 
				{
					c->blocks[i][k][j] = 0; 
				}
			}

			if (Utils::RandomChance(0.01f)) 
			{
				//std::cout << "placing tree at: " << i << " " << j << std::endl;
				//srand((unsigned int)Utils::magnitude(c->chunkPos));
				addTreeeeeeeee(i, heightY, j, c);
			}
		}
	} 

	//Game::world->TryPlaceBlockOnChunk(15, 100, 15, c, 4);
	//Game::world->TryPlaceBlockOnChunk(15, 100, 16, c, 4);
	//Game::world->TryPlaceBlockOnChunk(15, 100, 17, c, 4);

	std::map<glm::vec2, Chunk*, Utils::VectorComp>::iterator it = Game::world->unloadedChunkObjs.find(glm::vec2(c->cRelativeCoordX, c->cRelativeCoordY));
	if (it != Game::world->unloadedChunkObjs.end())
	{
		for (int x = 0; x < Chunk::CHUNK_X; x++)
		{
			for (int y = 0; y < Chunk::CHUNK_Y; y++)
			{
				for (int z = 0; z < Chunk::CHUNK_Z; z++)
				{
					if (it->second->blocks[x][y][z] != 0)
					{
						c->blocks[x][y][z] = it->second->blocks[x][y][z];
					}
				}
			}
		}
	}

	c->generated = true;
}