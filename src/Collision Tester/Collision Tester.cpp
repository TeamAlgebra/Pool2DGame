#include "Collision Tester.h"

namespace pool
{
	namespace collisions
	{
		const short amountBalls = 15;
		vector <pair<ball::Ball*, ball::Ball*>> collisionList;

		bool checkCollisionBallBall(ball::Ball* ball1, ball::Ball* ball2)
		{
			if (ball1->getHole() || ball2->getHole()) return false;

			float distanceF = distance(ball1->getPosition(), ball2->getPosition());

			return (distanceF < ball1->getRadius() + ball2->getRadius() && (ball1->getMoving() || ball2->getMoving()));
		}

		void setCollisionBallBall(ball::Ball* ball1, ball::Ball* ball2)
		{
			float distanceF = distance(ball1->getPosition(), ball2->getPosition());

			if (distanceF < ball1->getRadius() + ball2->getRadius() && (ball1->getMoving() || ball2->getMoving()))
			{
				float overlap = 0.5f * (distanceF - ball1->getRadius() - ball2->getRadius());

				float newPosX = ball1->getPosition().x - overlap * (ball1->getPosition().x - ball2->getPosition().x) / distanceF;
				float newPosY = ball1->getPosition().y - overlap * (ball1->getPosition().y - ball2->getPosition().y) / distanceF;
				ball1->setPosition({ newPosX,newPosY });

				newPosX = ball2->getPosition().x + overlap * (ball1->getPosition().x - ball2->getPosition().x) / distanceF;
				newPosY = ball2->getPosition().y + overlap * (ball1->getPosition().y - ball2->getPosition().y) / distanceF;
				ball2->setPosition({ newPosX, newPosY});

				moveBalls(ball1, ball2);
			}
		}

		void setCollisionBallWall(ball::Ball* ball, Rectangle field)
		{
			if (ball->getPosition().x + ball->getRadius() > field.x + field.width)
			{
				ball->setPosition({ field.x + field.width - ball->getRadius(), ball->getPosition().y });

				ball->setSpeed({ ball->getSpeed().x * -1, ball->getSpeed().y });
			}
			else if (ball->getPosition().x - ball->getRadius() < field.x)
			{
				ball->setPosition({ field.x + ball->getRadius(), ball->getPosition().y });

				ball->setSpeed({ ball->getSpeed().x * -1, ball->getSpeed().y });
			}

			if (ball->getPosition().y + ball->getRadius() > field.y + field.height)
			{
				ball->setPosition({ ball->getPosition().x, field.y + field.height - ball->getRadius() });

				ball->setSpeed({ ball->getSpeed().x, ball->getSpeed().y * -1});
			}
			else if (ball->getPosition().y - ball->getRadius() < field.y)
			{
				ball->setPosition({ ball->getPosition().x, field.y + ball->getRadius() });

				ball->setSpeed({ ball->getSpeed().x, ball->getSpeed().y * -1 });
			}
		}

		void checkCollisionBallPosition(ball::Ball* ball, field::Field* field)
		{
			float distanceF;

			for (short i = 0; i < field::amountHoles; i++)
			{
				distanceF = distance(ball->getPosition(), field->getPos(i));
				if (distanceF < field->getRadius())
				{
					ball->setHole(true);
				}
			}
		}

		//Collision Solvers
		void moveBalls(ball::Ball* ball1, ball::Ball* ball2)
		{
			float distanceF = distance(ball1->getPosition(), ball2->getPosition());

			// Normal
			float normalX = (ball2->getPosition().x - ball1->getPosition().x) / distanceF;
			float normalY = (ball2->getPosition().y - ball1->getPosition().y) / distanceF;

			// Tan
			float tanX = -normalY;
			float tanY = normalX;

			// Producto punto con la tangente
			float dotTanBall1 = dot(ball1->getSpeed(), { tanX, tanY });
			float dotTanBall2 = dot(ball2->getSpeed(), { tanX, tanY });

			// Producto punto con la normal
			float dotNormalBall1 = dot(ball1->getSpeed(), { normalX, normalY });
			float dotNormalBall2 = dot(ball2->getSpeed(), { normalX, normalY });

			// Momentum
			float momentumBall1 = (dotNormalBall1 / (ball1->getMass() * 2)) + dotNormalBall2;
			float momentumBall2 = (dotNormalBall2 / (ball2->getMass() * 2)) + dotNormalBall1;
			
			ball1->setSpeed( { tanX * dotTanBall1 + normalX * momentumBall1,tanY * dotTanBall1 + normalY * momentumBall1 });
			ball2->setSpeed( { tanX * dotTanBall2 + normalX * momentumBall2,tanY * dotTanBall2 + normalY * momentumBall2 });
		}

		float dot(Vector2 from, Vector2 to)
		{
			return from.x * to.x + from.y * to.y;
		}

		// Utils
		float distance(Vector2 from, Vector2 to)
		{
			Vector2 distanceVector = { to.x - from.x, to.y - from.y };
			return sqrtf(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y);
		}
	}
}
