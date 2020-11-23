#ifndef COLLISION_CHECKS_AND_LOGIC_H
#define COLLISION_CHECKS_AND_LOGIC_H

#include "../Objects/Ball/Ball.h"
#include "../Objects/Field/Field.h"
#include "raylib.h"
#include <math.h>
#include <vector>

using namespace std;

namespace pool
{
	namespace collisions
	{
		//Collision Vector
		extern vector <pair<ball::Ball*, ball::Ball*>> collisionList;

		//Collision testers and listers
		bool checkCollisionBallBall(ball::Ball* ball1, ball::Ball* ball2);
		void setCollisionBallBall(ball::Ball* ball1, ball::Ball* ball2);
		void setCollisionBallWall(ball::Ball* ball, Rectangle field);
		void checkCollisionBallPosition(ball::Ball* ball, field::Field* field);

		//Collision Solvers
		void moveBalls(ball::Ball* ball1, ball::Ball* ball2);

		//Utils
		float distance(Vector2 from, Vector2 to);
		float dot(Vector2 from, Vector2 to);
	}
}

#endif
