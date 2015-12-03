#ifndef RENDERER
#define RENDERER

#include "MyApplication.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

struct Vertex {
	vec4 position;
	vec4 colour;
};

class Renderer
{
public:

	Renderer();
	~Renderer();

	void Update(float dt);

	void Draw(Camera* camera);

	void GenerateGrid( unsigned int rows, unsigned int cols );

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;

	unsigned int m_programID;

	int m_rows;
	int m_cols;

private:
};


#endif