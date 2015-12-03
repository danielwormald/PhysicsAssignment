
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "MyApplication.h"
#include <vector>

using glm::vec3;
using glm::vec4;
class CheckPiece;

struct CheckSquare
{
	CheckSquare(vec3 pos, vec4 color, float rad, unsigned int seg, unsigned int id, bool isOcc)
	{
		m_position = pos;
		m_color = color;
		m_originalColor = color;
		m_radius = rad;
		m_segments = seg;
		m_id = id;
		m_isOccupied = isOcc;
		m_neighbourSquares[0] = nullptr;
		m_neighbourSquares[1] = nullptr;
		m_neighbourSquares[2] = nullptr;
		m_neighbourSquares[3] = nullptr;
		m_checkerPiece = nullptr;
		m_jumpableCheckerPiece = nullptr;
	};

	vec3 m_position;
	vec4 m_color;
	vec4 m_originalColor;
	float m_radius;
	unsigned int m_segments;

	unsigned int m_id;
	bool m_isOccupied;

	CheckSquare* m_neighbourSquares[4];
	CheckPiece* m_checkerPiece;
	CheckPiece* m_jumpableCheckerPiece;
	/*std::vector<CheckSquare*> m_neighbourSquares;*/
	//int m_neighbourSquares[4];
};

class Background
{
public:
	const static int Dimensions = 8;

	Background();
	~Background();

	void Update(float dt);

	void Draw(Camera* camera);

	void ShowAvailMoves();

	void SetNeighbourSquares();

	CheckSquare* GetClickedSquare(vec3 mousePos);

	unsigned int GetClickedSquareIter(vec3 mousePos);

	std::vector<CheckSquare*> m_checkerSquare;
	std::vector<CheckSquare*> test;

private:

};

#endif