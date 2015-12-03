
#include "PerlinNoise.h"
#include <stb_image.h>

PerlinNoise::PerlinNoise(unsigned int programId, unsigned int dimensions, float amplitude, float persistence)
{
	m_programId = programId;
	m_dimensions = dimensions;
	m_amplitude = amplitude;
	m_persistence = persistence;
	m_octaves = 6;
	m_indexCount = 0;

	float scale = (1.0f / m_dimensions) * 3;
	float HI = 4294967294 / 2;
	float LO = 0;

	float seed = (LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO))));
	m_seed = (unsigned int)seed;
	
	InitializePerlinNoise(m_octaves, scale, m_amplitude, m_persistence);
	InitializePerlinNoise(m_octaves, scale, m_amplitude, m_persistence);
	
	m_waterTexture = LoadTexture("data/textures/water.jpg");
	m_sandTexture = LoadTexture("data/textures/sand.jpg");
	m_grassTexture = LoadTexture("data/textures/grass.jpg");
	m_snowTexture = LoadTexture("data/textures/grass.jpg");
};

PerlinNoise::~PerlinNoise()
{

};

void PerlinNoise::InitializePerlinNoise(int octaves, float scale, float amplitude, float persistence)
{	
	float *perlin_data = new float[m_dimensions * m_dimensions];
	unsigned int size = 0;
	for (unsigned int x = 0; x < m_dimensions; ++x)
	{
		for (unsigned int y = 0; y < m_dimensions; ++y)
		{
			perlin_data[y * m_dimensions + x] = 0;

			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(2, (float)o);
				float perlin_sample = glm::perlin(glm::vec3((float)x, (float)y, m_seed) * scale * freq) * 0.5f + 0.5f;

				perlin_data[y * m_dimensions + x] += perlin_sample * amplitude;
				amplitude *= persistence;
			}
			size++;
		}
	}

	glGenTextures(1, &m_perlin_texture);
	glBindTexture(GL_TEXTURE_2D, m_perlin_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, m_dimensions, m_dimensions, 0, GL_RED, GL_FLOAT, perlin_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	unsigned int rows = m_dimensions;
	unsigned int cols = m_dimensions;

	pnVertices = new PerlinNoiseVertex[rows * cols];

	for (unsigned int r = 0; r < rows; ++r)
	{
		for (unsigned int c = 0; c < cols; ++c)
		{
			pnVertices[r * cols + c].position = glm::vec4((float)c, perlin_data[r * cols + c] * 5, (float)r, 1);
			pnVertices[r * cols + c].texCoords = glm::vec2((float)c / cols, (float)r / rows);
			pnVertices[r * cols + c].normal = glm::vec4(1);
		}
	}

	for (unsigned int i = 0; i < m_indexCount; i += 3)
	{
		GLushort ia = auiIndices[i];
		GLushort ib = auiIndices[i+1];
		GLushort ic = auiIndices[i+2];
		glm::vec3 normal = glm::normalize(glm::cross(
			glm::vec3(pnVertices[ib].position) - glm::vec3(pnVertices[ia].position),
			glm::vec3(pnVertices[ic].position) - glm::vec3(pnVertices[ia].position)
			));
		pnVertices[ia].normal = pnVertices[ib].normal = pnVertices[ic].normal = glm::vec4(normal, 1);
	}

	auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];
	unsigned int index = 0;
	for (unsigned int r = 0; r < (rows - 1); ++r)
	{
		for (unsigned int c = 0; c < (cols - 1); ++c)
		{
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);

			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			auiIndices[index++] = r * cols + (c + 1);
		}
	}

	m_indexCount = index;

	// Create VAO, VBO and IBO
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(PerlinNoiseVertex), pnVertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(PerlinNoiseVertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(PerlinNoiseVertex), (void*)(sizeof(glm::vec4)));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(PerlinNoiseVertex), (void*)((sizeof(glm::vec4)) + (sizeof(glm::vec2))));
	
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint PerlinNoise::LoadTexture(char * filename)
{
	GLuint tex = 0;
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	unsigned char* data = stbi_load(filename, &imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &tex); 
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);		

	return tex;
}

void PerlinNoise::Draw(Camera* camera)
{
	glUseProgram(m_programId);
	
	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, m_perlin_texture);
	
	glActiveTexture(GL_TEXTURE1); 
	glBindTexture(GL_TEXTURE_2D, m_waterTexture);
	
	glActiveTexture(GL_TEXTURE2); 
	glBindTexture(GL_TEXTURE_2D, m_sandTexture);
	
	glActiveTexture(GL_TEXTURE3); 
	glBindTexture(GL_TEXTURE_2D, m_grassTexture);
	
	glActiveTexture(GL_TEXTURE4); 
	glBindTexture(GL_TEXTURE_2D, m_snowTexture);

	unsigned int location = glGetUniformLocation(m_programId, "perlin_texture");
	glUniform1i(location, 0);


	location = glGetUniformLocation(m_programId, "waterTexture");
	glUniform1i(location, 1);
	
	location = glGetUniformLocation(m_programId, "sandTexture");
	glUniform1i(location, 2);
	
	location = glGetUniformLocation(m_programId, "grassTexture");
	glUniform1i(location, 3);
	
	location = glGetUniformLocation(m_programId, "snowTexture");
	glUniform1i(location, 4);
	
	location = glGetUniformLocation(m_programId, "dimensions");
	glUniform1f(location, (GLfloat)m_dimensions);

	glm::vec3 color = m_antTweaks->m_lightColour;
	glm::vec3 pos = m_antTweaks->m_lightPos;
	
	int projectionViewUniform = glGetUniformLocation(m_programId,"ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, GL_FALSE, &(camera->getProjectionView()[0][0]));

	int lightPosUniform = glGetUniformLocation(m_programId, "LightPos");
	glUniform3f(lightPosUniform, pos.x, pos.y, pos.z);

	int lightColourUniform = glGetUniformLocation(m_programId, "LightColour");
	glUniform3f(lightColourUniform, color.r, color.g, color.b);

	int cameraPosUniform = glGetUniformLocation(m_programId, "CameraPos");
	glUniform3f(cameraPosUniform, camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);

	int specPow = glGetUniformLocation(m_programId, "SpecPow");
	glUniform1f(specPow, m_antTweaks->m_specPower);

	location = glGetUniformLocation(m_programId,"ProjectionView");
	glUniformMatrix4fv(location, 1, GL_FALSE, &(camera->getProjectionView()[0][0]));

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}		

void PerlinNoise::SetTweakBar(AntTweakBar * tweaks)
{ 
	m_antTweaks = tweaks;
	m_antTweaks->SetSeed(m_seed); 
}