#include "PlayerOne.h"

PlayerOne::PlayerOne(GLFWwindow* window, std::vector<CheckSquare*> squares) 
{
	m_Window = window;

	m_canMove = true;
	clickPiece = false;

	m_selectedPiece = nullptr;

	int variation = 0;
	int id = 1;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			variation = j;
			if(i == 1 || i == 3)
			{
				variation += 1;
			}
			m_checkPieces.push_back(new CheckPiece(vec3(-4 + (variation + j) + 0.5f , 0.1, 4 - i - 0.5f), vec4(1, 0, 0, 1), Players::Player_One, id));
			id++;
		}
	}

	SetBackgroundId(squares);
}

void PlayerOne::Update(float dt, Camera* camera, Background* &background, Players &state, std::vector<CheckPiece*> &opCheckPieces)
{
	bool clicked = false;
	double mouseX, mouseY;

	vec3 mouseWorldPos;

	//Check if player ones turn
	if(state == Players::Player_One)
	{
		//Check if mouse button was clicked
		if(glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
		{
			//Get Cursor position in world
			glfwGetCursorPos(m_Window, &mouseX, &mouseY);
			mouseWorldPos = camera->pickAgainstPlane((float)mouseX, (float)mouseY, vec4(0, 1, 0, 0));

			clicked = true;
		}
	}

	bool checkClicked = false;

	//Loop through checker pieces 
	for (int i = 0; i < m_checkPieces.size(); i++)
	{
		//Check if mouse was clicked
		if(clicked)
		{
			//Check if piece was clicked on
			if(m_checkPieces[i]->IsClicked(mouseWorldPos))
			{
				checkClicked = true;
				clickPiece = true;
				m_checkPieces[i]->SetColor(vec4(0, 1, 0, 1));
				m_selectedPiece = m_checkPieces[i];

				//cout
				printf("Checker Id: %u, Board Id: %u \n", m_checkPieces[i]->GetId(), m_checkPieces[i]->GetBackgroundId());

				//Show Moves
				//TODO: Make better
				for (int j = 0; j < m_checkPieces.size(); j++)
				{
					if(m_selectedPiece != m_checkPieces[j])
					{
						m_checkPieces[j]->isSelected = false;
					}
				}
			}
		}

		//Check if the piece has been selected, if not set color to orginal value
		if(m_checkPieces[i]->isSelected && state == Players::Player_One)
		{
			//Get Possible Moves if any
			GetPossibleMoves(m_checkPieces[i], background, false);
		}
		else
			m_checkPieces[i]->SetColor(m_checkPieces[i]->GetOriginalColor());

		if(m_checkPieces[i] != nullptr)
			m_checkPieces[i]->Update(dt, background);
	}

	//If player has clicked on empty Square
	if(!checkClicked && clicked)
	{
		/*CheckSquare* selectedSquare = background->GetClickedSquare(mouseWorldPos);*/
		unsigned int backgroundId = background->GetClickedSquareIter(mouseWorldPos);

		bool na = false;

		//background->m_checkerSquare[backgroundId]

		//if(m_selectedPiece != nullptr && selectedSquare != nullptr)
		if(m_selectedPiece != nullptr && background->m_checkerSquare[backgroundId] != nullptr)
		{
			//loop through each possible move and check if selected square is part of it
			for (int i = 0; i < 4; i++)
			{
				if(m_selectedPiece->m_possibleMoves[i] == background->m_checkerSquare[backgroundId])
				{
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
						state = Players::Player_Two;
					}

					break;
				}
			}
		}

		//if no acceptable moves selected then default all squares and checker pieces to unselected
		if(!na)
		{
			m_selectedPiece = nullptr;

			//Loop through all check pieces
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

void PlayerOne::Draw(Camera* camera)
{
	for (int i = 0; i < m_checkPieces.size(); i++)
	{
		m_checkPieces[i]->Draw(camera);
	}
}

void PlayerOne::SetBackgroundId(std::vector<CheckSquare*> squares)
{
	for (int j = 0; j < m_checkPieces.size(); j++)
	{
		for (int i = 0; i < squares.size(); i++)
		{
			if((m_checkPieces[j]->GetPosition().x >= squares[i]->m_position.x - squares[i]->m_radius) && (m_checkPieces[j]->GetPosition().x <= squares[i]->m_position.x + squares[i]->m_radius) && 
				(m_checkPieces[j]->GetPosition().z >= squares[i]->m_position.z - squares[i]->m_radius) && (m_checkPieces[j]->GetPosition().z <= squares[i]->m_position.z + squares[i]->m_radius))
			{
				m_checkPieces[j]->SetBackgroundId(squares[i]);
				squares[i]->m_checkerPiece = m_checkPieces[j];
				squares[i]->m_isOccupied = true;
			}
		}
	}
}

void PlayerOne::RemoveCheckerPiece(std::vector<CheckPiece*> &opCheckPieces, CheckPiece* jumpablePiece)
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

void PlayerOne::GetPossibleMoves(CheckPiece* checkPiece, Background* background, bool secondMoveCheck)
{
	//Get the ids of the check board that the piece can move to. 
	CheckSquare* selectedCheckSquare = nullptr;
	for (int k = 0; k < background->m_checkerSquare.size(); k++)
	{
		if(background->m_checkerSquare[k]->m_id == checkPiece->m_backgroundId->m_id)
		{
			selectedCheckSquare = background->m_checkerSquare[k];
			break;
		}
	}

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
				if(selectedCheckSquare->m_neighbourSquares[p] != nullptr ? (selectedCheckSquare->m_neighbourSquares[p]->m_id == background->m_checkerSquare[l]->m_id && (selectedCheckSquare->m_id > background->m_checkerSquare[l]->m_id || m_selectedPiece->isKing )) : false)
				{
					//Check if the neighbour square is has a checker piece in it already
					if(background->m_checkerSquare[l]->m_isOccupied)
					{
						//Check if the following checker square is occupied
						if(background->m_checkerSquare[l]->m_neighbourSquares[p] != nullptr ? (!background->m_checkerSquare[l]->m_neighbourSquares[p]->m_isOccupied) : false)
						{
							//If no checker piece in square & checker piece not on player team tell player they can jump over the neighbour square/checker piece
							if(background->m_checkerSquare[l]->m_checkerPiece != nullptr ? background->m_checkerSquare[l]->m_checkerPiece->m_team == Players::Player_Two : false)
							{
								//Add to list of possible moves
								m_selectedPiece->m_possibleMoves[p] = background->m_checkerSquare[l]->m_neighbourSquares[p];
								m_selectedPiece->m_possibleMoves[p]->m_jumpableCheckerPiece = background->m_checkerSquare[l]->m_checkerPiece;
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
							m_selectedPiece->m_possibleMoves[p] = background->m_checkerSquare[l];
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