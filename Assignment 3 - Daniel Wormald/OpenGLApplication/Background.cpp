
#include "Background.h"

Background::Background()
{
	vec4 darkColor(0.54, 0.27, 0.07, 1);
	vec4 lightColor(0.87, 0.72, 0.52, 1);
	bool light = true;

	unsigned int id = 1;

	//Create 8x8 board
	for (int i = 0; i < Dimensions; i++)
	{
		//TODO: do better
		if(i == 1 || i == 3 || i == 5 || i == 7)
			light = false;
		else
			light = true;

		for (int j = 0; j < Dimensions; j++)
		{
			vec3 pos(-3.5 + j, -0.5, -3.5 + i);

			if(light)
			{
				m_checkerSquare.push_back(new CheckSquare(pos, lightColor, 0.5f, 4, id, false));
				light = false;
			}
			else
			{
				m_checkerSquare.push_back(new CheckSquare(pos, darkColor, 0.5f, 4, id, false));
				light = true;
			}

			id++;
		}
	}

	SetNeighbourSquares();
}

Background::~Background()
{

}

void Background::Update(float dt)
{
	for (int i = 0; i < m_checkerSquare.size(); i++)
	{
		Gizmos::addAABBFilled(m_checkerSquare[i]->m_position, vec3(0.5, 0.5, 0.5), m_checkerSquare[i]->m_color);
	}
}

void Background::Draw(Camera* camera)
{

}

void Background::ShowAvailMoves()
{

}

void Background::SetNeighbourSquares()
{
	for (int i = 0; i < m_checkerSquare.size(); i++)
	{
		vec3 topLeft(m_checkerSquare[i]->m_position.x - 1, m_checkerSquare[i]->m_position.y, m_checkerSquare[i]->m_position.z + 1);
		vec3 topRight(m_checkerSquare[i]->m_position.x + 1, m_checkerSquare[i]->m_position.y, m_checkerSquare[i]->m_position.z + 1);;
		vec3 bottomRight(m_checkerSquare[i]->m_position.x + 1, m_checkerSquare[i]->m_position.y, m_checkerSquare[i]->m_position.z - 1);;
		vec3 bottomLeft(m_checkerSquare[i]->m_position.x - 1, m_checkerSquare[i]->m_position.y, m_checkerSquare[i]->m_position.z - 1);;

		for (int j = 0; j < m_checkerSquare.size(); j++)
		{
			if((m_checkerSquare[j]->m_position.x >= topLeft.x - m_checkerSquare[j]->m_radius) && (m_checkerSquare[j]->m_position.x <= topLeft.x + m_checkerSquare[j]->m_radius) && 
				(m_checkerSquare[j]->m_position.z >= topLeft.z - m_checkerSquare[j]->m_radius) && (m_checkerSquare[j]->m_position.z <= topLeft.z + m_checkerSquare[j]->m_radius))
			{
				m_checkerSquare[i]->m_neighbourSquares[0] = m_checkerSquare[j];
			}
			if((m_checkerSquare[j]->m_position.x >= topRight.x - m_checkerSquare[j]->m_radius) && (m_checkerSquare[j]->m_position.x <= topRight.x + m_checkerSquare[j]->m_radius) && 
				(m_checkerSquare[j]->m_position.z >= topRight.z - m_checkerSquare[j]->m_radius) && (m_checkerSquare[j]->m_position.z <= topRight.z + m_checkerSquare[j]->m_radius))
			{
				m_checkerSquare[i]->m_neighbourSquares[1] = m_checkerSquare[j];
			}
			if((m_checkerSquare[j]->m_position.x >= bottomRight.x - m_checkerSquare[j]->m_radius) && (m_checkerSquare[j]->m_position.x <= bottomRight.x + m_checkerSquare[j]->m_radius) && 
				(m_checkerSquare[j]->m_position.z >= bottomRight.z - m_checkerSquare[j]->m_radius) && (m_checkerSquare[j]->m_position.z <= bottomRight.z + m_checkerSquare[j]->m_radius))
			{
				m_checkerSquare[i]->m_neighbourSquares[2] = m_checkerSquare[j];
			}
			if((m_checkerSquare[j]->m_position.x >= bottomLeft.x - m_checkerSquare[j]->m_radius) && (m_checkerSquare[j]->m_position.x <= bottomLeft.x + m_checkerSquare[j]->m_radius) && 
				(m_checkerSquare[j]->m_position.z >= bottomLeft.z - m_checkerSquare[j]->m_radius) && (m_checkerSquare[j]->m_position.z <= bottomLeft.z + m_checkerSquare[j]->m_radius))
			{
				m_checkerSquare[i]->m_neighbourSquares[3] = m_checkerSquare[j];
			}
		}
	}
}

CheckSquare* Background::GetClickedSquare(vec3 mousePos)
{
	for (int i = 0; i < m_checkerSquare.size(); i++)
	{
		if((mousePos.x >= m_checkerSquare[i]->m_position.x - m_checkerSquare[i]->m_radius) && (mousePos.x <= m_checkerSquare[i]->m_position.x + m_checkerSquare[i]->m_radius) && 
			(mousePos.z >= m_checkerSquare[i]->m_position.z - m_checkerSquare[i]->m_radius) && (mousePos.z <= m_checkerSquare[i]->m_position.z + m_checkerSquare[i]->m_radius)	&& 
			(mousePos.y >= m_checkerSquare[i]->m_position.y - m_checkerSquare[i]->m_radius) && (mousePos.y <= m_checkerSquare[i]->m_position.y + m_checkerSquare[i]->m_radius))
		{
			return m_checkerSquare[i];
		}
	}
	return nullptr;
}

unsigned int Background::GetClickedSquareIter(vec3 mousePos)
{
	for (int i = 0; i < m_checkerSquare.size(); i++)
	{
		if((mousePos.x >= m_checkerSquare[i]->m_position.x - m_checkerSquare[i]->m_radius) && (mousePos.x <= m_checkerSquare[i]->m_position.x + m_checkerSquare[i]->m_radius) && 
			(mousePos.z >= m_checkerSquare[i]->m_position.z - m_checkerSquare[i]->m_radius) && (mousePos.z <= m_checkerSquare[i]->m_position.z + m_checkerSquare[i]->m_radius)	&& 
			(mousePos.y >= m_checkerSquare[i]->m_position.y - m_checkerSquare[i]->m_radius) && (mousePos.y <= m_checkerSquare[i]->m_position.y + m_checkerSquare[i]->m_radius))
		{
			return i;
		}
	}
	return -1;
}

//for (int i = 0; i < m_checkerSquare.size(); i++)
//	{
//		vec3 topLeft(m_checkerSquare[i]->m_position.x - 1, m_checkerSquare[i]->m_position.y, m_checkerSquare[i]->m_position.z + 1);
//		vec3 topRight(m_checkerSquare[i]->m_position.x + 1, m_checkerSquare[i]->m_position.y, m_checkerSquare[i]->m_position.z + 1);;
//		vec3 bottomRight(m_checkerSquare[i]->m_position.x + 1, m_checkerSquare[i]->m_position.y, m_checkerSquare[i]->m_position.z - 1);;
//		vec3 bottomLeft(m_checkerSquare[i]->m_position.x - 1, m_checkerSquare[i]->m_position.y, m_checkerSquare[i]->m_position.z - 1);;
//
//		for (int j = 0; j < m_checkerSquare.size(); j++)
//		{
//			if((m_checkerSquare[j]->m_position.x >= topLeft.x - m_checkerSquare[j]->m_radius) && (m_checkerSquare[j]->m_position.x <= topLeft.x + m_checkerSquare[j]->m_radius) && 
//				(m_checkerSquare[j]->m_position.z >= topLeft.z - m_checkerSquare[j]->m_radius) && (m_checkerSquare[j]->m_position.z <= topLeft.z + m_checkerSquare[j]->m_radius))
//			{
//				m_checkerSquare[i]->m_neighbourSquares[0] = m_checkerSquare[j];
//			}
//			if((m_checkerSquare[j]->m_position.x >= topRight.x - m_checkerSquare[j]->m_radius) && (m_checkerSquare[j]->m_position.x <= topRight.x + m_checkerSquare[j]->m_radius) && 
//				(m_checkerSquare[j]->m_position.z >= topRight.z - m_checkerSquare[j]->m_radius) && (m_checkerSquare[j]->m_position.z <= topRight.z + m_checkerSquare[j]->m_radius))
//			{
//				m_checkerSquare[i]->m_neighbourSquares[1] = m_checkerSquare[j];
//			}
//			if((m_checkerSquare[j]->m_position.x >= bottomRight.x - m_checkerSquare[j]->m_radius) && (m_checkerSquare[j]->m_position.x <= bottomRight.x + m_checkerSquare[j]->m_radius) && 
//				(m_checkerSquare[j]->m_position.z >= bottomRight.z - m_checkerSquare[j]->m_radius) && (m_checkerSquare[j]->m_position.z <= bottomRight.z + m_checkerSquare[j]->m_radius))
//			{
//				m_checkerSquare[i]->m_neighbourSquares[2] = m_checkerSquare[j];
//			}
//			if((m_checkerSquare[j]->m_position.x >= bottomLeft.x - m_checkerSquare[j]->m_radius) && (m_checkerSquare[j]->m_position.x <= bottomLeft.x + m_checkerSquare[j]->m_radius) && 
//				(m_checkerSquare[j]->m_position.z >= bottomLeft.z - m_checkerSquare[j]->m_radius) && (m_checkerSquare[j]->m_position.z <= bottomLeft.z + m_checkerSquare[j]->m_radius))
//			{
//				m_checkerSquare[i]->m_neighbourSquares[3] = m_checkerSquare[j];
//			}
//		}
//	}