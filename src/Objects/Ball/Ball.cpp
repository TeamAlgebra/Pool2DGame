#include "Ball.h"

#include <iostream>

namespace pool
{
	namespace ball
	{
		Ball::Ball()
		{
			_value = 0; 
			
			_moving = false;
			_inHole = false;
			_whiteBall = false;

			_radius = 1.0f;
			_mass = 160.0f;

			_speed = {0, 0};
			_position = {0, 0};
		}

		Ball::Ball(short value, Vector2 spawnPoint, float radius, bool whiteBall)
		{
			_value = value; 
			_position = spawnPoint; 
			_radius = radius;
			_speed = { 0, 0 };
			_moving = false;
			_inHole = false;
			_mass = 160.0f;
			_whiteBall = whiteBall;
		}

		void Ball::setSpeed(Vector2 speed)
		{
			_speed.x = speed.x;
			_speed.y = speed.y;
		}

		Vector2 Ball::getSpeed()
		{
			return _speed;
		}

		bool Ball::getMoving()
		{
			return _moving;
		}

		void Ball::update()
		{
			if (_inHole) return;

			if (_speed.x != 0 || _speed.y != 0)
				_moving = true;
			else
				_moving = false;

			if (_moving)
			{
				_position.x += _speed.x * GetFrameTime() * speedMultiplier;
				_position.y += _speed.y * GetFrameTime() * speedMultiplier;

				_speed.x *= friction;
				_speed.y *= friction;

				if ((_speed.x < minVelocity && _speed.y < minVelocity) && (_speed.x > -minVelocity && _speed.y > -minVelocity))
				{
					_speed = { 0.f, 0.f };
				}
			}
		}

		void Ball::draw()
		{
			if (_inHole)
			{
				//DrawCircleV(_position, _radius, MAGENTA); // Solo para debug
				return;
			}

			if (_whiteBall)
			{
				DrawCircleV(_position, _radius, WHITE);
			}
			else
			{
				if (_value == 8)
				{
					DrawCircleV(_position, _radius, BLACK);
				}
				else
				{
					if (_value % 2 == 0)
					{
						DrawCircleV(_position, _radius, RED);
					}
					else
					{
						DrawCircleV(_position, _radius, BLUE);
					}
				}
			}
		}

		Vector2 Ball::getPosition()
		{
			return _position;
		}

		float Ball::getRadius()
		{
			return _radius;
		}

		void Ball::setPosition(Vector2 position)
		{
			_position.x = position.x;
			_position.y = position.y;
		}

		float Ball::getMass()
		{
			return _mass;
		}

		void Ball::setHole(bool inHole)
		{
			_inHole = inHole;
		}

		bool Ball::getHole()
		{
			return _inHole;
		}
	}
}