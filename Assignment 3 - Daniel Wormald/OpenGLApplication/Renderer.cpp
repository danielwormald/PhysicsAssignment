
#include "Renderer.h"

Renderer::Renderer()
{
	/*int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	unsigned char* data = stbi_load("./data/textures/crate.png", &imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_image_free(data);*/

	m_cols = 10;
	m_rows = 10;
	// create shaders
	const char* vsSource = "#version 410\n \
						   layout(location=0) in vec4 Position; \
						   layout(location=1) in vec4 Colour; \
						   out vec4 vColour; \
						   uniform mat4 ProjectionView; \
						   void main() { vColour = Colour; gl_Position = ProjectionView * Position; }";
	const char* fsSource = "#version 410\n \
						   in vec4 vColour; \
						   out vec4 FragColor; \
						   void main() { FragColor = vColour; }";
	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);
	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n",infoLog);
		delete[] infoLog;
	}
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	GenerateGrid(m_rows, m_cols);
}

Renderer::~Renderer()
{

}

void Renderer::Update(float dt)
{
}

void Renderer::Draw(Camera* camera)
{
	glUseProgram(m_programID);
	unsigned int projectionViewUniform = glGetUniformLocation(m_programID,"ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false,	glm::value_ptr(camera->getProjectionView()));
	glBindVertexArray(m_VAO);
	unsigned int indexCount = (m_rows - 1) * (m_cols - 1) * 6;
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void Renderer::GenerateGrid( unsigned int rows, unsigned int cols )
{
	Vertex* aoVertices = new Vertex[ rows * cols ];
	for ( unsigned int r = 0 ; r < rows ; ++r ) {
		for ( unsigned int c = 0 ; c < cols ; ++c ) {
			aoVertices[ r * cols + c ].position = vec4(
				(float)c, 0, (float)r, 1);
			// create some arbitrary colour based off something
			// that might not be related to tiling a texture
			vec3 colour = vec3( sinf( (c / (float)(cols - 1)) *
				( r / (float)(rows - 1))) );
			aoVertices[ r * cols + c ].colour = vec4( colour, 1 );
		}
	}
	// we’ll do more here soon!

	unsigned int* auiIndices = new unsigned int[ (rows - 1) * (cols - 1) *
		6 ];
	unsigned int index = 0;
	for ( unsigned int r = 0 ; r < (rows - 1) ; ++r ) {
		for ( unsigned int c = 0 ; c < (cols - 1) ; ++c ) {
			// triangle 1
			auiIndices[ index++ ] = r * cols + c;
			auiIndices[ index++ ] = (r + 1) * cols + c;
			auiIndices[ index++ ] = (r + 1) * cols + (c + 1);
			// triangle 2
			auiIndices[ index++ ] = r * cols + c;
			auiIndices[ index++ ] = (r + 1) * cols + (c + 1);
			auiIndices[ index++ ] = r * cols + (c + 1);
		}
	}

	//Create buffers
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	//Bind Buffers
	glBindVertexArray( m_VAO );
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	//Get Data
	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex), aoVertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1) * (cols - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);

	//Send to shader
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));

	glBindVertexArray(0);
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	delete[] aoVertices;
	delete[] auiIndices;
}