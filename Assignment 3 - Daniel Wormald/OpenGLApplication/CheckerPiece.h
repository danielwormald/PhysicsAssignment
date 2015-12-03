#ifndef CHECKERPIECE_H
#define CHECKERPIECE_H

#include "MyApplication.h"
#include <vector>

using glm::vec3;
using glm::vec4;
enum Players;

struct CheckSquare;
class Background;

class CheckPiece
{
public:
	CheckPiece(vec3 pos, vec4 color, Players team, unsigned int id);

	~CheckPiece();

	void Update(float dt, Background* background);

	void Draw(Camera* camera);

	bool IsClicked(vec3 mousePos);

	void MovePiece();

	CheckSquare* m_possibleMoves[4];

	vec3 GetPosition() { return m_position; }

	void SetPosition(vec3 pos){ m_position = pos; }

	vec4 GetColor() { return m_color; }

	void SetColor(vec4 color){ m_color = color; }

	vec4 GetOriginalColor() { return m_originalColor; }

	unsigned int GetId() { return m_id; }

	void SetId(unsigned int id){ m_id = id; }

	bool isSelected;

	bool isKing;

	float m_radius;

	void SetBackgroundId(CheckSquare* id) { m_backgroundId = id; }

	bool CheckIfKingPlayerOne(Background* background);

	bool CheckIfKingPlayerTwo(Background* background);

	CheckSquare* GetBackgroundId() { return m_backgroundId; }

	CheckSquare* m_backgroundId;

	Players m_team;

private:

	vec3 m_position;
	vec4 m_color;
	vec4 m_originalColor;
	
	unsigned int m_segments;

	unsigned int m_id;

	

	
};

#endif