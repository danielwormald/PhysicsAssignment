
#include "PlayerTwo.h"

PlayerTwo::PlayerTwo(GLFWwindow* window, std::vector<CheckSquare*> squares)
{
	m_Window = window;

	m_canMove = false;
	//m_selectedId = 0;

	int variation = 0;
	int id = 1;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			variation = j;
			if(i == 0 || i == 2)
			{
				variation += 1;
			}
			m_checkPieces.push_back(new CheckPiece(vec3(-4 + (variation + j) + 0.5f , 0, -4 + i + 0.5f), vec4(0, 0, 1, 1), Players::Player_Two, id));
			id++;
		}
	}

	SetBackgroundId(squares);
}

void PlayerTwo::Update(float dt, Camera* camera, Background* &background, Players &state, std::vector<CheckPiece*> &opCheckPieces)
{
	bool clicked = false;
	double mouseX, mouseY;

	vec3 mouseWorldPos;

	/*if(state == Players::Player_Two)
	{
	if(glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{
	glfwGetCursorPos(m_Window, &mouseX, &mouseY);

	mouseWorldPos = camera->pickAgainstPlane((float)mouseX, (float)mouseY, vec4(0, 1, 0, 0));

	clicked = true;
	}
	}*/
	/*for (int i = 0; i < m_checkPieces.size(); i++)
	{
	m_checkPieces[i]->m_possibleMoves[0] = nullptr;
	m_checkPieces[i]->m_possibleMoves[1] = nullptr;
	m_checkPieces[i]->m_possibleMoves[2] = nullptr;
	m_checkPieces[i]->m_possibleMoves[3] = nullptr;
	GetPossibleMoves(m_checkPieces[i], background, false);
	}*/


	bool checkClicked = false;

	std::vector<CheckPiece*> lstOfJumpableCheckPieces;
	std::vector<CheckPiece*> lstOfNormalMoves;

	for (int i = 0; i < m_checkPieces.size(); i++)
	{
		//Get the possible moves
		if(state == Players::Player_Two)
		{
			m_checkPieces[i]->m_possibleMoves[0] = nullptr;
			m_checkPieces[i]->m_possibleMoves[1] = nullptr;
			m_checkPieces[i]->m_possibleMoves[2] = nullptr;
			m_checkPieces[i]->m_possibleMoves[3] = nullptr;

			//Get Possible Moves if any
			GetPossibleMoves(m_checkPieces[i], background, false);

			//Check if there is any jumpable checker pieces
			if((m_checkPieces[i]->m_possibleMoves[0] != nullptr ? m_checkPieces[i]->m_possibleMoves[0]->m_jumpableCheckerPiece != nullptr : false) || 
				(m_checkPieces[i]->m_possibleMoves[1] != nullptr ? m_checkPieces[i]->m_possibleMoves[1]->m_jumpableCheckerPiece != nullptr : false) || 
				(m_checkPieces[i]->m_possibleMoves[2] != nullptr ? m_checkPieces[i]->m_possibleMoves[2]->m_jumpableCheckerPiece != nullptr : false) ||
				(m_checkPieces[i]->m_possibleMoves[3] != nullptr ? m_checkPieces[i]->m_possibleMoves[3]->m_jumpableCheckerPiece != nullptr : false))
			{
				lstOfJumpableCheckPieces.push_back(m_checkPieces[i]);
			}
			else if(m_checkPieces[i]->m_possibleMoves[0] != nullptr || 
				m_checkPieces[i]->m_possibleMoves[1] != nullptr || 
				m_checkPieces[i]->m_possibleMoves[2] != nullptr ||
				m_checkPieces[i]->m_possibleMoves[3] != nullptr )
			{
				lstOfNormalMoves.push_back(m_checkPieces[i]);
			}
		}
		else
			m_checkPieces[i]->SetColor(m_checkPieces[i]->GetOriginalColor());

		if(m_checkPieces[i] != nullptr)
			m_checkPieces[i]->Update(dt, background);
	}
	unsigned int backgroundId = 0;

	if(lstOfJumpableCheckPieces.size() > 0)
	{
		if(lstOfJumpableCheckPieces.size() == 1)
		{
			m_selectedPiece = lstOfJumpableCheckPieces[0];
		}
		else
		{
			bool anyChosen = false;
			for (int i = 0; i < lstOfJumpableCheckPieces.size(); i++)
			{
				if(lstOfJumpableCheckPieces[i]->m_possibleMoves[0] != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_jumpableCheckerPiece != nullptr : false) 
				{
					GetPossibleMoves(lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece, background, false);

					if(lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece->m_possibleMoves[0]->m_jumpableCheckerPiece != nullptr : false) : false || 
						lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece->m_possibleMoves[1]->m_jumpableCheckerPiece != nullptr : false) : false || 
						lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece->m_possibleMoves[2]->m_jumpableCheckerPiece != nullptr : false) : false ||
						lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece->m_possibleMoves[3]->m_jumpableCheckerPiece != nullptr : false) : false)
					{
						m_selectedPiece = lstOfJumpableCheckPieces[i];
						break;
					}
				}
				else if(lstOfJumpableCheckPieces[i]->m_possibleMoves[1] != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_jumpableCheckerPiece != nullptr : false)
				{
					GetPossibleMoves(lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece, background, false);

					if(lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece->m_possibleMoves[0]->m_jumpableCheckerPiece != nullptr : false) : false || 
						lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece->m_possibleMoves[1]->m_jumpableCheckerPiece != nullptr : false) : false || 
						lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece->m_possibleMoves[2]->m_jumpableCheckerPiece != nullptr : false) : false ||
						lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece->m_possibleMoves[3]->m_jumpableCheckerPiece != nullptr : false) : false)
					{
						m_selectedPiece = lstOfJumpableCheckPieces[i];
						break;
					}
				}
				else if(lstOfJumpableCheckPieces[i]->m_possibleMoves[2] != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[2]->m_jumpableCheckerPiece != nullptr : false)
				{
					GetPossibleMoves(lstOfJumpableCheckPieces[i]->m_possibleMoves[2]->m_checkerPiece, background, false);

					if(lstOfJumpableCheckPieces[i]->m_possibleMoves[2]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[2]->m_possibleMoves[0]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[2]->m_checkerPiece->m_possibleMoves[0]->m_jumpableCheckerPiece != nullptr : false) : false || 
						lstOfJumpableCheckPieces[i]->m_possibleMoves[2]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[2]->m_possibleMoves[0]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[2]->m_checkerPiece->m_possibleMoves[1]->m_jumpableCheckerPiece != nullptr : false) : false || 
						lstOfJumpableCheckPieces[i]->m_possibleMoves[2]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[2]->m_possibleMoves[0]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[2]->m_checkerPiece->m_possibleMoves[2]->m_jumpableCheckerPiece != nullptr : false) : false ||
						lstOfJumpableCheckPieces[i]->m_possibleMoves[2]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[2]->m_possibleMoves[0]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[2]->m_checkerPiece->m_possibleMoves[3]->m_jumpableCheckerPiece != nullptr : false) : false)
					{
						m_selectedPiece = lstOfJumpableCheckPieces[i];
						break;
					}
				}
				else if(lstOfJumpableCheckPieces[i]->m_possibleMoves[3] != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_jumpableCheckerPiece != nullptr : false)
				{
					GetPossibleMoves(lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece, background, false);

					if(lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece->m_possibleMoves[0]->m_jumpableCheckerPiece != nullptr : false) : false || 
						lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece->m_possibleMoves[1]->m_jumpableCheckerPiece != nullptr : false) : false || 
						lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece->m_possibleMoves[2]->m_jumpableCheckerPiece != nullptr : false) : false ||
						lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece->m_possibleMoves[3]->m_jumpableCheckerPiece != nullptr : false) : false)
					{
						m_selectedPiece = lstOfJumpableCheckPieces[i];
						break;
					}
				}

				/*if( lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece->m_possibleMoves[0]->m_jumpableCheckerPiece != nullptr : false) : false || 
					lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece->m_possibleMoves[1]->m_jumpableCheckerPiece != nullptr : false) : false || 
					lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece->m_possibleMoves[2]->m_jumpableCheckerPiece != nullptr : false) : false ||
					lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[0]->m_checkerPiece->m_possibleMoves[3]->m_jumpableCheckerPiece != nullptr : false) : false || 
					lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece->m_possibleMoves[0]->m_jumpableCheckerPiece != nullptr : false) : false || 
					lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece->m_possibleMoves[1]->m_jumpableCheckerPiece != nullptr : false) : false ||
					lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece->m_possibleMoves[2]->m_jumpableCheckerPiece != nullptr : false) : false || 
					lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[1]->m_checkerPiece->m_possibleMoves[3]->m_jumpableCheckerPiece != nullptr : false) : false || 
					lstOfJumpableCheckPieces[i]->m_possibleMoves[2]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[2]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[2]->m_checkerPiece->m_possibleMoves[0]->m_jumpableCheckerPiece != nullptr : false) : false ||
					lstOfJumpableCheckPieces[i]->m_possibleMoves[2]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[2]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[2]->m_checkerPiece->m_possibleMoves[1]->m_jumpableCheckerPiece != nullptr : false) : false || 
					lstOfJumpableCheckPieces[i]->m_possibleMoves[2]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[2]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[2]->m_checkerPiece->m_possibleMoves[2]->m_jumpableCheckerPiece != nullptr : false) : false || 
					lstOfJumpableCheckPieces[i]->m_possibleMoves[2]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[2]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[2]->m_checkerPiece->m_possibleMoves[3]->m_jumpableCheckerPiece != nullptr : false) : false ||
					lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece->m_possibleMoves[0]->m_jumpableCheckerPiece != nullptr : false) : false || 
					lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece->m_possibleMoves[1]->m_jumpableCheckerPiece != nullptr : false) : false || 
					lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece->m_possibleMoves[2]->m_jumpableCheckerPiece != nullptr : false) : false ||
					lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece != nullptr ? (lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece->m_possibleMoves != nullptr ? lstOfJumpableCheckPieces[i]->m_possibleMoves[3]->m_checkerPiece->m_possibleMoves[3]->m_jumpableCheckerPiece != nullptr : false) : false)
				{
					m_selectedPiece = lstOfJumpableCheckPieces[i];
					break;
				}*/
			}

			if(!anyChosen)
			{
				m_selectedPiece = lstOfJumpableCheckPieces[0];
			}
		}

		if(m_selectedPiece->m_possibleMoves[0] != nullptr ? m_selectedPiece->m_possibleMoves[0]->m_jumpableCheckerPiece != nullptr : false) 
		{
			backgroundId = m_selectedPiece->m_possibleMoves[0]->m_id - 1;
		}
		else if(m_selectedPiece->m_possibleMoves[1] != nullptr ? m_selectedPiece->m_possibleMoves[1]->m_jumpableCheckerPiece != nullptr : false)
		{
			backgroundId = m_selectedPiece->m_possibleMoves[1]->m_id - 1;
		}
		else if(m_selectedPiece->m_possibleMoves[2] != nullptr ? m_selectedPiece->m_possibleMoves[2]->m_jumpableCheckerPiece != nullptr : false)
		{
			backgroundId = m_selectedPiece->m_possibleMoves[2]->m_id - 1;
		}
		else if(m_selectedPiece->m_possibleMoves[3] != nullptr ? m_selectedPiece->m_possibleMoves[3]->m_jumpableCheckerPiece != nullptr : false)
		{
			backgroundId = m_selectedPiece->m_possibleMoves[3]->m_id - 1;
		}
	}
	else if(lstOfNormalMoves.size() > 0)
	{
		m_selectedPiece = lstOfNormalMoves[0];

		if(m_selectedPiece->m_possibleMoves[0] != nullptr) 
		{
			backgroundId = m_selectedPiece->m_possibleMoves[0]->m_id - 1;
		}
		else if(m_selectedPiece->m_possibleMoves[1] != nullptr)
		{
			backgroundId = m_selectedPiece->m_possibleMoves[1]->m_id - 1;
		}
		else if(m_selectedPiece->m_possibleMoves[2] != nullptr)
		{
			backgroundId = m_selectedPiece->m_possibleMoves[2]->m_id - 1;
		}
		else if(m_selectedPiece->m_possibleMoves[3] != nullptr)
		{
			backgroundId = m_selectedPiece->m_possibleMoves[3]->m_id - 1;
		}
	}

	if(lstOfJumpableCheckPieces.size() > 0 || lstOfNormalMoves.size() > 0)
	{
		//CheckSquare* selectedSquare = background->GetClickedSquare(mouseWorldPos);
		//unsigned int backgroundId = background->GetClickedSquareIter(mouseWorldPos);

		bool na = false;

		if(m_selectedPiece != nullptr && background->m_checkerSquare[backgroundId] != nullptr)
		{
			for (int i = 0; i < 4; i++)
			{
				if(m_selectedPiece->m_possibleMoves[i] == background->m_checkerSquare[backgroundId])
				{
					//Sleep(1);

					bool jumpedPiece = false;
					//Check if there is a checker piece that can be jumped over, if so remove the checker piece from the opposing player
					if(background->m_checkerSquare[backgroundId]->m_jumpableCheckerPiece != nullptr)
					{
						RemoveCheckerPiece(opCheckPieces, background->m_checkerSquare[backgroundId]->m_jumpableCheckerPiece);
						background->m_checkerSquare[backgroundId]->m_jumpableCheckerPiece = nullptr;
						jumpedPiece = true;
					}

					m_selectedPiece->SetPosition(vec3(background->m_checkerSquare[backgroundId]->m_position.x, 0.1, background->m_checkerSquare[backgroundId]->m_position.z));
					m_selectedPiece->m_backgroundId->m_isOccupied = false;
					m_selectedPiece->m_backgroundId->m_checkerPiece = nullptr;
					m_selectedPiece->SetBackgroundId(background->m_checkerSquare[backgroundId]);
					m_selectedPiece->m_backgroundId->m_isOccupied = true;
					background->m_checkerSquare[backgroundId]->m_checkerPiece = m_selectedPiece;
					m_selectedPiece->m_backgroundId->m_checkerPiece = m_selectedPiece;

					for (int j = 0; j < 4; j++)
					{
						m_selectedPiece->m_possibleMoves[j] = nullptr;
					}
					bool canMoveAgain = false;

					if(jumpedPiece)
					{
						GetPossibleMoves(m_selectedPiece, background, true);

						for (int k = 0; k < 4; k++)
						{
							if(m_selectedPiece->m_possibleMoves[k] != nullptr)
							{
								canMoveAgain = true;
							}
						}
					}

					if(!canMoveAgain)
					{
						m_selectedPiece = nullptr;
						//selectedSquare = nullptr;
						na = true;
						state = Players::Player_One;
						//break;
					}

					break;
				}
			}
		}

		if(!na)
		{
			m_selectedPiece = nullptr;

			for (int i = 0; i < m_checkPieces.size(); i++)
			{
				m_checkPieces[i]->isSelected = false;
			}	
			//Loop through all squares
			for (int l = 0; l < background->m_checkerSquare.size(); l++)
			{
				background->m_checkerSquare[l]->m_color = background->m_checkerSquare[l]->m_originalColor;
			}
		}
	}
} 

void PlayerTwo::Draw(Camera* camera)
{
	for (int i = 0; i < m_checkPieces.size(); i++)
	{
		m_checkPieces[i]->Draw(camera);
	}
}

void PlayerTwo::SetBackgroundId(std::vector<CheckSquare*> squares)
{
	for (int j = 0; j < m_checkPieces.size(); j++)
	{
		for (int i = 0; i < squares.size(); i++)
		{
			if((m_checkPieces[j]->GetPosition().x >= squares[i]->m_position.x - squares[i]->m_radius) && (m_checkPieces[j]->GetPosition().x <= squares[i]->m_position.x + squares[i]->m_radius) && 
				(m_checkPieces[j]->GetPosition().z >= squares[i]->m_position.z - squares[i]->m_radius) && (m_checkPieces[j]->GetPosition().z <= squares[i]->m_position.z + squares[i]->m_radius))
			{
				m_checkPieces[j]->SetBackgroundId(squares[i]);
				squares[i]->m_isOccupied = true;
				squares[i]->m_checkerPiece = m_checkPieces[j];
			}
		}
	}
}

void PlayerTwo::RemoveCheckerPiece(std::vector<CheckPiece*> &opCheckPieces, CheckPiece* jumpablePiece)
{
	for (int i = 0; i < opCheckPieces.size(); i++)
	{
		if(opCheckPieces[i] == jumpablePiece)
		{
			opCheckPieces[i]->m_backgroundId->m_isOccupied = false;
			opCheckPieces[i]->m_backgroundId->m_checkerPiece = nullptr;
			opCheckPieces[i] = nullptr;
			opCheckPieces.erase(opCheckPieces.begin() + i);
		}
	}
}

void PlayerTwo::GetPossibleMoves(CheckPiece* checkPiece, Background* background, bool secondMoveCheck)
{
	//Get the ids of the check board that the piece can move to. 
	CheckSquare* selectedCheckSquare = nullptr;
	/*for (int k = 0; k < background->m_checkerSquare.size(); k++)
	{
	if(background->m_checkerSquare[k]->m_id == checkPiece->m_backgroundId->m_id)
	{
	selectedCheckSquare = background->m_checkerSquare[k];
	break;
	}
	}*/
	if(checkPiece != nullptr)
		selectedCheckSquare = checkPiece->GetBackgroundId();

	//Check that the selected square isnt null
	if(selectedCheckSquare != nullptr)
	{
		//GetPossibleMoves(selectedCheckSquare, background);

		//Loop through all squares
		for (int l = 0; l < background->m_checkerSquare.size(); l++)
		{
			//Loop through neighbour squares (topleft,topright,bottomright,bottomleft)
			for (int p = 0; p < 4; p++)
			{
				//Check if neighbour square isnt null and if the square is part of the neighbour squares, and the square is in front of the original square
				if(selectedCheckSquare->m_neighbourSquares[p] != nullptr ? (selectedCheckSquare->m_neighbourSquares[p]->m_id == background->m_checkerSquare[l]->m_id && (selectedCheckSquare->m_id < background->m_checkerSquare[l]->m_id || checkPiece->isKing))  : false)
				{
					//Check if the neighbour square is has a checker piece in it already
					if(background->m_checkerSquare[l]->m_isOccupied)
					{
						//Check if the following checker square is occupied
						if(background->m_checkerSquare[l]->m_neighbourSquares[p] != nullptr ? (!background->m_checkerSquare[l]->m_neighbourSquares[p]->m_isOccupied) : false)
						{
							//If no checker piece in square & checker piece not on player team tell player they can jump over the neighbour square/checker piece
							if(background->m_checkerSquare[l]->m_checkerPiece != nullptr ? background->m_checkerSquare[l]->m_checkerPiece->m_team == Players::Player_One : false)
							{
								//Add to list of possible moves
								/*m_selectedPiece->m_possibleMoves[p] = background->m_checkerSquare[l]->m_neighbourSquares[p];
								m_selectedPiece->m_possibleMoves[p]->m_jumpableCheckerPiece = background->m_checkerSquare[l]->m_checkerPiece;*/
								checkPiece->m_possibleMoves[p] = background->m_checkerSquare[l]->m_neighbourSquares[p];
								checkPiece->m_possibleMoves[p]->m_jumpableCheckerPiece = background->m_checkerSquare[l]->m_checkerPiece;

								background->m_checkerSquare[l]->m_neighbourSquares[p]->m_color = vec4(0, 1, 0, 1);
							}
							else
								background->m_checkerSquare[l]->m_color = background->m_checkerSquare[l]->m_originalColor;
						}
						else
							background->m_checkerSquare[l]->m_color = background->m_checkerSquare[l]->m_originalColor;
					}
					else
					{
						if(!secondMoveCheck)
						{
							//m_selectedPiece->m_possibleMoves[p] = background->m_checkerSquare[l];
							checkPiece->m_possibleMoves[p] = background->m_checkerSquare[l];

							background->m_checkerSquare[l]->m_color = vec4(0, 1, 0, 1);
						}
						else
							background->m_checkerSquare[l]->m_color = background->m_checkerSquare[l]->m_originalColor;
					}
					break;
				}
				else
					background->m_checkerSquare[l]->m_color = background->m_checkerSquare[l]->m_originalColor;
			}
		}
	}
}

void PlayerTwo::Sleep(clock_t sec) // clock_t is a like typedef unsigned int clock_t. Use clock_t instead of integer in this context
{
	clock_t start_time = clock();
	clock_t end_time = sec * 1000 + start_time;
	while(clock() != end_time)
	{

	}
}