

#ifndef SHADER
#define SHADER

#include "MyApplication.h"
#include "FBXFile.h"

class ShaderManager;
class AntTweakBar;
class ParticleEmitter;
class GPUParticleEmitter;
class Player;
class Background;
enum Players;

class Shader
{
public :
	Shader(ShaderManager* shaderManager, Camera* pCamera, GLFWwindow* window);
	~Shader();

	void Update(float dt, Camera* camera);
	void Draw();

private:

	//Initialize Shaders
	void InitializeShader();

	//Load Textires
	void LoadTextures();

	//Load Textires
	void LoadModels();

	//Retrieve Shaders
	unsigned int RetrieveShader(unsigned int type, const char* path);

	void createOpenGLBuffers(FBXFile* fbx);
	void cleanupOpenGLBuffers(FBXFile* fbx);

	//Camera 
	Camera* m_pCamera;

	//Loading fbx files
	//FBXFile* m_fbx;

	//Shader Manager
	ShaderManager* m_shaderManager;

	//Loading Particles
	GPUParticleEmitter* m_emitter;

	//Program id
	unsigned int m_program;
	unsigned int m_normal;

	//Loading textures
	unsigned int m_texture;

	unsigned int m_vbo, m_vao, m_ibo;

	//Scene Management
	unsigned int m_fbo;
	unsigned int m_fboDepth;
	unsigned int m_fboTexture;

	std::vector<Player*> m_players;

	Background* m_background;

	Players m_turn;

private:


};


#endif