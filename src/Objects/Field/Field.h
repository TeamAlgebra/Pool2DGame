#pragma once
#ifndef FIELD_H
#define FIELD_H

#include "raylib.h"
#include "../../Config.h"

namespace pool 
{
	namespace field 
	{
		const short amountHoles = 6;
		const short distanceScreenField = 20;

		class Field 
		{
		private:
			Rectangle _field;
			float _radius;
			Vector2 _pos[amountHoles];
		public:
			Field();
			void draw();
			Rectangle getDimentions();
			Vector2 getPos(short i);
			float getRadius();
		};
	}
}
#endif // !FIELD_H
