#define _CRT_SECURE_NO_WARNINGS
#include "Shader.h"
#include <stb_image.h>
#include "AntTweakBar.h"
#include "Player.h"
#include "PlayerOne.h"
#include "PlayerTwo.h"

Shader::Shader(ShaderManager* shaderManager, Camera* pCamera, GLFWwindow* window) 
{
	m_pCamera = pCamera;
	m_shaderManager = shaderManager;
	//InitializeShader();

	m_turn = Players::Player_One;

	m_background = new Background();

	m_players.push_back(new PlayerOne(window, m_background->m_checkerSquare));
	m_players.push_back(new PlayerTwo(window, m_background->m_checkerSquare));
}

Shader::~Shader()
{
	//cleanupOpenGLBuffers(m_fbx);
	glDeleteProgram(m_program);
}

void Shader::Update(float dt, Camera* camera)
{
	m_pCamera = camera;
	
	m_background->Update(dt);

	for (int i = 0; i < m_players.size(); i++)
	{
		if(m_players[i]->m_checkPieces.size() == 0)
		{
			if(m_players[i]->m_playerId == Players::Player_One)
				printf("Player Two Wins");
			else
				printf("Player One Wins");
		}
		m_players[i]->Update(dt, m_pCamera, m_background, m_turn, i == 0 ? m_players[1]->m_checkPieces : m_players[0]->m_checkPieces);
	}
}

void Shader::Draw()
{
	for (int i = 0; i < m_players.size(); i++)
	{
		m_players[i]->Draw(m_pCamera);
	}
}

void Shader::InitializeShader()
{
	/*int unlitSuccess = GL_FALSE;

	unsigned int vsUnlit = RetrieveShader(GL_VERTEX_SHADER, "data/shaderSrc/shader.vert");		
	unsigned int fsUnlit = RetrieveShader(GL_FRAGMENT_SHADER, "data/shaderSrc/shader.frag");

	m_program = glCreateProgram();
	glAttachShader(m_program, vsUnlit);
	glAttachShader(m_program, fsUnlit);
	glLinkProgram(m_program);
	glGetProgramiv(m_program, GL_LINK_STATUS, &unlitSuccess);

	if (unlitSuccess == GL_FALSE) 
	{
		int infoLogLength = 0;
		glGetShaderiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(m_program, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader unlit program!\n");
		printf("%s\n",infoLog);
		delete[] infoLog;
	}

	glDeleteShader(fsUnlit);
	glDeleteShader(vsUnlit);*/
}

void Shader::LoadModels()
{
	/*m_fbx = new FBXFile();
	m_fbx->load("./data/textures/Models/level1.fbx");	*/
}

void Shader::LoadTextures()
{
	//int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	//unsigned char* data;

	//// load diffuse map
	//data = stbi_load("./data/characters/Pyro/Pyro_D.tga",
	//	&imageWidth, &imageHeight, &imageFormat, STBI_default);

	//glGenTextures(1, &m_texture);
	//glBindTexture(GL_TEXTURE_2D, m_texture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 
	//	0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//stbi_image_free(data);

	//// load normal map
	//data = stbi_load("./data/characters/Pyro/Pyro_N.tga",
	//	&imageWidth, &imageHeight, &imageFormat, STBI_default);

	//glGenTextures(1, &m_normal);
	//glBindTexture(GL_TEXTURE_2D, m_normal);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight,
	//	0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//stbi_image_free(data);
}

void Shader::createOpenGLBuffers(FBXFile* fbx)
{
	// create the GL VAO/VBO/IBO data for each mesh
	for (unsigned int i = 0; i < fbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = fbx->getMeshByIndex(i);
		// storage for the opengl data in 3 unsigned int
		unsigned int* glData = new unsigned int[3];

		glGenVertexArrays(1, &glData[0]);
		glBindVertexArray(glData[0]);

		glGenBuffers(1, &glData[1]);
		glGenBuffers(1, &glData[2]);

		glBindBuffer(GL_ARRAY_BUFFER, glData[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glData[2]);

		glBufferData(GL_ARRAY_BUFFER,mesh->m_vertices.size() * sizeof(FBXVertex),mesh->m_vertices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,mesh->m_indices.size() * sizeof(unsigned int),mesh->m_indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); // position
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,sizeof(FBXVertex), 0);

		glEnableVertexAttribArray(1); // normal
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE,sizeof(FBXVertex),((char*)0) + FBXVertex::NormalOffset);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mesh->m_userData = glData;
	}
}

void Shader::cleanupOpenGLBuffers(FBXFile* fbx) {
	// clean up the vertex data attached to each mesh
	for (unsigned int i = 0; i < fbx->getMeshCount(); ++i) {
		FBXMeshNode* mesh = fbx->getMeshByIndex(i);
		unsigned int* glData = (unsigned int*)mesh->m_userData;

		glDeleteVertexArrays(1, &glData[0]);
		glDeleteBuffers(1, &glData[1]);
		glDeleteBuffers(1, &glData[2]);

		delete[] glData;
	}
}

unsigned int Shader::RetrieveShader(unsigned int type, const char* path) 
{
	FILE* file = fopen(path, "rb");

	if (file == nullptr)
		return 0;

	// read the shader source
	fseek(file, 0, SEEK_END);
	unsigned int length = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* source = new char[length + 1];

	memset(source, 0, length + 1);
	fread(source, sizeof(char), length, file);
	fclose(file);

	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);

	delete[] source;

	return shader;
}
