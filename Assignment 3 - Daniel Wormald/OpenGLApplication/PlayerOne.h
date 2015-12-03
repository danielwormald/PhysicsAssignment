#ifndef PLAYERONE_H
#define PLAYERONE_H

#include "Player.h"
#include "MyApplication.h"
#include <vector>

class CheckSquare;

class PlayerOne : public Player
{
public:
	PlayerOne(GLFWwindow* window, std::vector<CheckSquare*> squares);

	/*virtual void Update(float dt, Camera* camera, std::vector<CheckSquare*> squares);*/
	virtual void Update(float dt, Camera* camera, Background* &background, Players &state, std::vector<CheckPiece*> &opCheckPieces);

	virtual void Draw(Camera* camera);

	void RemoveCheckerPiece(std::vector<CheckPiece*> &opCheckPieces, CheckPiece* jumpablePiece);

	void GetPossibleMoves(CheckPiece* checkPiece, Background* background, bool secondMoveCheck);

	void SetBackgroundId(std::vector<CheckSquare*> squares);

private:

};

#endif