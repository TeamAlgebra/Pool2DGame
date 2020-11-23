#include "Field.h"

namespace pool 
{
	namespace field 
	{
		Field::Field() 
		{
			_field.x = distanceScreenField;
			_field.y = distanceScreenField;
			_field.width = screenWidth - (distanceScreenField * 2);
			_field.height = screenHeight - (distanceScreenField * 2);
			_radius = ballRadius * 1.5;

			short auxY = _field.y;

			//setting holes
			for (short i = 0; i < amountHoles; i++) 
			{
				if (i % 2 == 0 && i != 0)
				{
					_pos[i].y = auxY;
					auxY += _field.height / 2;
				}

				_pos[i].y = auxY;

				if (i % 2 == 0) 
				{
					_pos[i].x = _field.x;
				}
				else  
				{
					_pos[i].x = _field.x + _field.width;
				}
			}
		}

		void Field::draw() 
		{
			DrawRectangleRec(_field, GREEN);

			for (short i = 0;i < amountHoles;i++) 
			{
				DrawCircleV(_pos[i], _radius,BROWN);
			}
		}

		Rectangle Field::getDimentions()
		{
			return _field;
		}

		float Field::getRadius()
		{
			return _radius;
		}

		Vector2 Field::getPos(short i)
		{
			return _pos[i];
		}
	}
}