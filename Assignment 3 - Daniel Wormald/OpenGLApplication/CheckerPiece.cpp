
#include "CheckerPiece.h"

CheckPiece::CheckPiece(vec3 pos, vec4 color, Players team, unsigned int id)
{
	m_position = pos;
	m_color = color;
	m_originalColor = color;
	m_radius = 0.4f;
	m_segments = 50;
	m_team = team;

	m_id = id;

	isSelected = false;
	isKing = false;
	m_possibleMoves[0] = nullptr;
	m_possibleMoves[1] = nullptr;
	m_possibleMoves[2] = nullptr;
	m_possibleMoves[3] = nullptr;
}

void CheckPiece::Update(float dt, Background* background)
{
	if(!this->isKing)
	{
		if(this->m_team == Players::Player_One)
		{
			if(CheckIfKingPlayerOne(background))
			{
				this->isKing = true;
				this->m_originalColor = vec4(0.25, 0, 0, 1);
			}
		}
		if(this->m_team == Players::Player_Two)
		{
			if(CheckIfKingPlayerTwo(background))
			{
				this->isKing = true;
				this->m_originalColor = vec4(0, 0, 0.25, 1);
			}
		}
	}

	Gizmos::addSphere(m_position, m_radius , 10, 10, m_color);
}

void CheckPiece::Draw(Camera* camera)
{

}

void CheckPiece::MovePiece()
{

}

bool CheckPiece::IsClicked(vec3 mousePos)
{
	if((mousePos.x >= m_position.x - m_radius) && (mousePos.x <= m_position.x + m_radius) && 
		(mousePos.z >= m_position.z - m_radius) && (mousePos.z <= m_position.z + m_radius)	&& 
		(mousePos.y >= m_position.y - m_radius) && (mousePos.y <= m_position.y + m_radius))
	{
		isSelected = true;
		return true;
	}
	else
	{
		isSelected = false;
		return false;
	}
}

bool CheckPiece::CheckIfKingPlayerOne(Background* background)
{
	if( this->GetBackgroundId() == background->m_checkerSquare[0] ||
		this->GetBackgroundId() == background->m_checkerSquare[1] ||
		this->GetBackgroundId() == background->m_checkerSquare[2] || 
		this->GetBackgroundId() == background->m_checkerSquare[3] ||
		this->GetBackgroundId() == background->m_checkerSquare[4] ||
		this->GetBackgroundId() == background->m_checkerSquare[5] ||
		this->GetBackgroundId() == background->m_checkerSquare[6] ||
		this->GetBackgroundId() == background->m_checkerSquare[7])
	{
		return true;
	}

	return false;
}

bool CheckPiece::CheckIfKingPlayerTwo(Background* background)
{
	if(this->GetBackgroundId() == background->m_checkerSquare[56] ||
		this->GetBackgroundId() == background->m_checkerSquare[57] ||
		this->GetBackgroundId() == background->m_checkerSquare[58] || 
		this->GetBackgroundId() == background->m_checkerSquare[59] ||
		this->GetBackgroundId() == background->m_checkerSquare[60] ||
		this->GetBackgroundId() == background->m_checkerSquare[61] ||
		this->GetBackgroundId() == background->m_checkerSquare[62] ||
		this->GetBackgroundId() == background->m_checkerSquare[63])
	{
		return true;
	}

	return false;
}