#ifndef PLAYERTWO_H
#define PLAYERTWO_H

#include "Player.h"
#include "MyApplication.h"
#include <vector>

class CheckSquare;

class PlayerTwo : public Player
{
public:
	PlayerTwo(GLFWwindow* window, std::vector<CheckSquare*> squares);

	virtual void Update(float dt, Camera* camera, Background* &background, Players &state, std::vector<CheckPiece*> &opCheckPieces);

	virtual void Draw(Camera* camera);

	void RemoveCheckerPiece(std::vector<CheckPiece*> &opCheckPieces, CheckPiece* jumpablePiece);

	void GetPossibleMoves(CheckPiece* checkPiece, Background* background, bool secondMoveCheck);

	void SetBackgroundId(std::vector<CheckSquare*> squares);

	void Sleep(clock_t sec);

private:

};

#endif