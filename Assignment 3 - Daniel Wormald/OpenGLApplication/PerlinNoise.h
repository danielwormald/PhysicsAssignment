
#ifndef PERLIN_NOISE
#define PERLIN_NOISE

#include "MyApplication.h"

class AntTweakBar;

struct PerlinNoiseVertex 
{
	glm::vec4 position;
	glm::vec2 texCoords;
	glm::vec4 normal;
};

class PerlinNoise
{
public:
	PerlinNoise(unsigned int programId, unsigned int dimensions, float amplitude, float persistence);
	~PerlinNoise();

	void Draw(Camera* camera);
	void SetTweakBar(AntTweakBar * tweaks);

	unsigned int m_seed;

	unsigned int m_dimensions;
	float m_amplitude, m_persistence, m_octaves;

	void InitializePerlinNoise(int octaves, float scale, float amplitude, float persistence);

private:

	GLuint LoadTexture(char * filename);

	unsigned int m_programId;

	unsigned int m_vertices, m_indexCount;
	unsigned int* auiIndices;
	PerlinNoiseVertex* pnVertices;

	GLuint m_texture, m_perlin_texture;
	GLuint m_waterTexture, m_sandTexture, m_grassTexture, m_snowTexture;

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;

	AntTweakBar * m_antTweaks;
	
};

#endif