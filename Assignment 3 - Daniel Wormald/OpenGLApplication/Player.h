#pragma once

#include "MyApplication.h"
#include <vector>

enum Players;
class CheckPiece;
class Background;

class Player
{
public:
	/*virtual void Update(float dt, Camera* camera, std::vector<CheckSquare*> squares);*/
	virtual void Update(float dt, Camera* camera, Background* &background, Players &state, std::vector<CheckPiece*> &opCheckPieces);

	virtual void Draw(Camera* camera);
	
	unsigned int PiecesCount;

	CheckPiece* m_selectedPiece;
	
	Players m_playerId;
	Players m_oppositionPlayerId;

	bool m_canMove;
	bool clickPiece;

	std::vector<CheckPiece*> m_checkPieces;
	GLFWwindow* m_Window;

	std::vector<int*> m_possibleMoves;

private:
	
};
