#include "GameRenderer.h"
#include "Shader.h"
#include "Player.h"
#include "World.h"
#include "SunSource.h"

int W, H;
unsigned int EBO, VBO;
Shader* lame;

float vertices[] = {
	// positions          // texcoords (front face)
	 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   // top right
	 0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    // top left 
	//top face
	 0.5f,  0.5f, 0.5f,    1.0f, 1.0f,   // top right
	 0.5f,  0.5f, -0.5f,   1.0f, 0.0f,   // bottom right
	-0.5f,  0.5f, -0.5f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.5f,    0.0f, 1.0f,    // top left 
	//right face
	 0.5f,  0.5f, 0.5f,    1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.5f,   1.0f, 0.0f,   // bottom right
	 0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   // bottom left
	 0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    // top left 
	 //back face
	 0.5f,  0.5f, 0.5f,    1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.5f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.5f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.5f,    0.0f, 1.0f,    // top left 
	//bottom face
	 0.5f,  -0.5f, 0.5f,    1.0f, 1.0f,   // top right
	 0.5f,  -0.5f, -0.5f,   1.0f, 0.0f,   // bottom right
	-0.5f,  -0.5f, -0.5f,   0.0f, 0.0f,   // bottom left
	-0.5f,  -0.5f, 0.5f,    0.0f, 1.0f,    // top left 
	//left face
	 -0.5f,  0.5f, 0.5f,    1.0f, 1.0f,   // top right
	 -0.5f, -0.5f, 0.5f,   1.0f, 0.0f,   // bottom right
	 -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   // bottom left
	 -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    // top left 
};
unsigned int indices[] =
{
	0, 1, 3,
	1, 2, 3,

	4, 5, 7,
	5, 6, 7,

	8, 9, 11,
	9, 10, 11,

	12, 13, 15,
	13, 14, 15,

	16, 17, 19,
	17, 18, 19,

	20, 21, 23,
	21, 22, 23
};
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	W = width;
	H = height;
}


void GameRenderer::createWindowContext(int w, int h, const char* t, int c_ma, int c_mi)
{
	meshObjs = std::vector<ModelData*>();
	FOV = 45.0f;
	aspectRatio = w / h;
	plane_clipNear = 0.1f;
	plane_clipFar = 260.0f;

	W = w;
	H = h;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, c_ma);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, c_mi);

	gameWindow = glfwCreateWindow(w, h, t, NULL, NULL);
	if (gameWindow == NULL) 
	{
		std::cout << "Nope on da window." << std::endl;
		Game::WindowDead = true;

	}
	glfwMakeContextCurrent(gameWindow);

	glfwSetFramebufferSizeCallback(gameWindow, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		std::cout << "He's dead bro... Glad aint here no more..." << std::endl;
		Game::GladDead = true;
	}
	//all of this shit is for the hello square...
	glViewport(0, 0, w, h);

	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	//glBindVertexArray(VAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//std::cout << sizeof(indices) / sizeof(indices[0]) << std::endl;
	lame = new Shader("C:/Users/jusmo/source/repos/Minecraft/Minecraft/VertShader.glvs", "C:/Users/jusmo/source/repos/Minecraft/Minecraft/FragShader.glfs");
	glGenTextures(1, &textureAtlas);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureAtlas);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);
	int width, height, numChannels;
	unsigned char* data = stbi_load("C:/Users/jusmo/source/repos/Minecraft/Minecraft/Textures/TextureAtlas.png", &width, &height, &numChannels, 0);

	if (data) 
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
	{
		std::cout << "FAILED TO LOAD THE TEXTURE... HOPE YOU ENJOY PINK LOL" << std::endl;
	}
}
void GameRenderer::PrepareRender() 
{
	//using iterators now ig.
	for (std::vector<Chunk*>::iterator i = Game::world->worldChunks.begin(); i != Game::world->worldChunks.end(); ++i)
	{
		if ((*i)->markedDirty) 
		{
			ModelData* m = new ModelData(); //LATER: Check if chunk has existing linked ModelData so that it doesn't create a new one each time a marked dirty chunk is detected.
			m->SetShader(lame);
			(*i)->m = m;
			m->MeshFromChunk((*i));
			(*i)->markedDirty = false;

			meshObjs.push_back(m);
		}
	}
	for (std::vector<ModelData*>::iterator dat = meshObjs.begin(); dat != meshObjs.end(); ++dat) 
	{
		if ((*dat)->done) 
		{
			dat = meshObjs.erase(dat);

			if (dat == meshObjs.end())
			{
				break;
			}
			//meshObjs.erase(meshObjs.begin() + i, meshObjs.begin() + i);
			//break;
		}
	}
}

void GameRenderer::RenderObjects() 
{
	aspectRatio = (float)W / H;//eeeeeeeeeeeeeeeeeeeeeeee

	glClearColor(0.2f, 0.6f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::vec3 planeModel = glm::vec3(0.0f, 0.0f, 1.0f);
	glBindVertexArray(VAO);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureAtlas);
/*
	glUniformMatrix4fv(plane_model, 1, GL_FALSE, glm::value_ptr(model));*/

	////chunk rendering goes here...

	//lame->use();

	///*glBindBuffer(GL_VERTEX_ARRAY, VBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);*/

	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	////std::cout << (int)(sizeof(indices) / sizeof(indices[0])) << std::endl;

	//actually it goes here]
	for (std::vector<ModelData*>::iterator i = meshObjs.begin(); i != meshObjs.end(); ++i) 
	{
		if (!(*i)->done)
			(*i)->RenderObject();
	}
	////std::cout << meshObjs.size() << std::endl;

	glfwSwapBuffers(gameWindow);
}

ModelData::ModelData()
{
	verts = std::vector<float>();
	tris = std::vector<int>();

	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);
	glGenVertexArrays(1, &this->VAO);
	model = glm::mat4(1.0f);
	transform = glm::vec3(0.0f);
	done = false;

	//s = lame;
}

void ModelData::Clear() 
{
	//if (!(trisReal == nullptr))
	//	delete[] trisReal;
	//if (!(vertsReal == nullptr))
	//	delete[] vertsReal;

	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
	glDeleteVertexArrays(1, &this->VAO);
	done = true;
}

void ModelData::RenderObject() 
{
	model = glm::mat4(1.0f);
	glBindVertexArray(this->VAO); // bind VAO

	model = glm::translate(model, p->chunkPos);

	unsigned int obj_modelLoc = glGetUniformLocation(s->ID, "model");
	unsigned int cam_projectLoc = glGetUniformLocation(s->ID, "projection");
	unsigned int cam_viewLoc = glGetUniformLocation(s->ID, "view");
	s->setVec3("lightColor", Game::sun.colRGB);
	s->setVec3("lightDir", glm::normalize(Game::sun.direction));
	s->setVec3("viewPos", Game::player->position);
	//std::cout << Game::sun.colRGB.x << std::endl;

	glUniformMatrix4fv(cam_projectLoc, 1, GL_FALSE, glm::value_ptr(Game::player->cam_projectionMat));
	glUniformMatrix4fv(cam_viewLoc, 1, GL_FALSE, glm::value_ptr(Game::player->cam_viewMat));
	glUniformMatrix4fv(obj_modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

	lame->use();
	glDrawElements(GL_TRIANGLES, numTris, GL_UNSIGNED_INT, 0);
	//std::cout << numTris << std::endl;

	//std::cout << sizeof(*trisReal) / sizeof(trisReal[0]) << std::endl;
}

void ModelData::MeshFromChunk(Chunk* c) 
{
	done = false;
	p = c;
	numTris = 0;
	//p->m = this;
	//int e = 0;
	for (int x = 0; x < Chunk::CHUNK_X; x++) 
	{
		for (int y = 0; y < Chunk::CHUNK_Y; y++) 
		{
			for (int z = 0; z < Chunk::CHUNK_Z; z++) 
			{
				if (c->blocks[x][y][z] != 0) 
				{
					AddCubeAt(x, y, z);
					//std::cout << c->blocks[x][y][z] << std::en
					//e++;
				}
			}
		}
	}
	//std::cout << numTris << std::endl;

	vertsReal = &verts[0];
	trisReal = &tris[0];

	//std::cout << "E" << std::endl;
	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), vertsReal, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tris.size() * sizeof(int), trisReal, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}



void ModelData::AddCubeAt(int x, int y, int z) 
{
	//local variables for useful things our faces might need
	unsigned int type = p->blocks[x][y][z];
	glm::vec3 origin = glm::vec3(x, y, z);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 worldRight = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 worldForward = glm::cross(worldUp, worldRight);

	if (!Chunk::IsSolid(x, y, z-1, p))
		AddFaceAt(origin, worldRight, worldUp, -1.0f, FACE_FRONT, type); // front face
	if (!Chunk::IsSolid(x, y+1, z, p))
		AddFaceAt(origin, worldRight, worldForward, 1.0f, FACE_TOP, type); // top face
	if (!Chunk::IsSolid(x+1, y, z, p))
		AddFaceAt(origin, worldForward, worldUp, 1.0f, FACE_RIGHT, type); // right face
	if (!Chunk::IsSolid(x, y, z+1, p))
		AddFaceAt(origin, worldRight, worldUp, 0.0f, FACE_BACK, type); // back face face
	if (!Chunk::IsSolid(x, y-1, z, p))
		AddFaceAt(origin, worldRight, worldForward, 0.0f, FACE_BOTTOM, type); // bottom face
	if (!Chunk::IsSolid(x-1, y, z, p))
		AddFaceAt(origin, worldForward, worldUp, 0.0f, FACE_LEFT, type); // left face
}

void ModelData::AddFaceAt(glm::vec3 o, glm::vec3 localX, glm::vec3 localY, float localZOff, int face, int type) 
{
	glm::vec3 localZ = glm::cross(localX, localY) * localZOff;
	glm::vec3 norm = glm::normalize(glm::cross(localX, localZ));
	glm::vec3 vertPos1 = o + localZ + localX + localY; // top right
	glm::vec3 vertPos2 = o + localZ + localX; // bottom right
	glm::vec3 vertPos3 = o + localZ; // bottom left
	glm::vec3 vertPos4 = o + localZ + localY; // top left
	float texOffset = Block::BlockTexOffsetOf(type, face);
	//triangles
	int numVerts = verts.size() / 8;
	tris.push_back(numVerts);
	tris.push_back(numVerts + 1);
	tris.push_back(numVerts + 3);
	tris.push_back(numVerts + 1);
	tris.push_back(numVerts + 2);
	tris.push_back(numVerts + 3);
	numTris += 6;
	//vertices
	float texY = 1.0f - (1.0f / 8) * texOffset;
	verts.push_back(vertPos1.x);
	verts.push_back(vertPos1.y);
	verts.push_back(vertPos1.z);
	verts.push_back(1.0f);
	verts.push_back(texY);
	verts.push_back(norm.x);
	verts.push_back(norm.y);
	verts.push_back(norm.z);

	verts.push_back(vertPos2.x);
	verts.push_back(vertPos2.y);
	verts.push_back(vertPos2.z);
	verts.push_back(1.0f);
	verts.push_back(texY - 1.0f / 8);
	verts.push_back(norm.x);
	verts.push_back(norm.y);
	verts.push_back(norm.z);

	verts.push_back(vertPos3.x);
	verts.push_back(vertPos3.y);
	verts.push_back(vertPos3.z);
	verts.push_back(0.0f);
	verts.push_back(texY - (1.0f / 8));
	verts.push_back(norm.x);
	verts.push_back(norm.y);
	verts.push_back(norm.z);

	verts.push_back(vertPos4.x);
	verts.push_back(vertPos4.y);
	verts.push_back(vertPos4.z);
	verts.push_back(0.0f);
	verts.push_back(texY);
	verts.push_back(norm.x);
	verts.push_back(norm.y);
	verts.push_back(norm.z);
}