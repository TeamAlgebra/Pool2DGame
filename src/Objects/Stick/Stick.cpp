#include "Stick.h"

#include <cmath>
#include <iostream>

namespace pool
{
	namespace stick
	{
		void Stick::init() 
		{
			_color = BROWN;
			_length = 100;
			_endPos.x = 0;
			_endPos.y = 0;
			_thikness = 8;
		}

		void Stick::draw(Vector2 ballCenter, float ballRadius)
		{
			const short hidenLength = ballRadius - 1;
			Vector2 hidenEndPos;

			_endPos.x = ballCenter.x - GetMouseX();
			_endPos.y = ballCenter.y - GetMouseY();

			float hypotenuse = sqrtf(powf(_endPos.x, 2) + powf(_endPos.y, 2));

			float relation = _length / hypotenuse;

			float hidenRelation = hidenLength / hypotenuse;

			_endPos.x = ballCenter.x - (ballCenter.x - GetMouseX()) * relation;
			_endPos.y = ballCenter.y - (ballCenter.y - GetMouseY()) * relation;

			hidenEndPos.x = ballCenter.x - (ballCenter.x - GetMouseX()) * hidenRelation;
			hidenEndPos.y = ballCenter.y - (ballCenter.y - GetMouseY()) * hidenRelation;

			DrawLineEx(ballCenter, _endPos, _thikness, _color);
			DrawLineEx(ballCenter, hidenEndPos, _thikness, WHITE);
		}
	}
}
