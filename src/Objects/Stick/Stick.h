#pragma once
#ifndef STICK_H
#define STICK_H

#include "raylib.h"
#include "../../Config.h"

namespace pool
{
	namespace stick
	{
		class Stick {
		private:
			Color _color;
			short _length;
			Vector2 _endPos;
			short _thikness;
		public:
			void init();
			void draw(Vector2 ballCenter, float ballRadius);
		};
	}
}

#endif // !STICK_H

