#ifndef BALL_H
#define BALL_H

#include "raylib.h"

namespace pool
{
	namespace ball
	{
		const float friction = 0.993f;
		const float minVelocity = 1.0f;
		const float speedMultiplier = 7.f;

		class Ball
		{
		private:
			int _value;
			//bool _lastBall;
			Vector2 _speed;
			Vector2 _position;
			float _radius;
			float _mass;
			bool _moving;
			bool _inHole;
			bool _whiteBall;
			Color _color;
		public:
			Ball();
			Ball(short value, Vector2 spawnPoint, float radius, bool whiteBall);
			void update();
			void draw();
			Vector2 getPosition();
			float getRadius();
			void setSpeed(Vector2 speed);
			Vector2 getSpeed();
			bool getMoving();
			void setPosition(Vector2 position);
			float getMass();
			void setHole(bool inHole);
			bool getHole();
			void setColor(Color color);
			int getValue();
		};
	}
}

#endif // !BALL_H

