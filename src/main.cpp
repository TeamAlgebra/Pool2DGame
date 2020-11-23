#include "raylib.h"
#include "Config.h"
#include "Objects/Field/Field.h"
#include "Objects/Ball/Ball.h"
#include "Objects/Stick/Stick.h"
#include "Collision Tester/Collision Tester.h"

#include <cmath>

using namespace pool;

int main()
{
#pragma region INITIALIZATION
    const int amountBalls = 15;
    // Initialization
    InitWindow(screenWidth, screenHeight, "Pool game");
    SetTargetFPS(60);

    field::Field* field = new pool::field::Field();

    ball::Ball* balls[amountBalls];
    ball::Ball* whiteBall = new pool::ball::Ball(0, { screenWidth / 2,  screenHeight / 1.5f }, ballRadius, true);

    stick::Stick* stick = new pool::stick::Stick();
    stick->init();
#pragma endregion

#pragma region BALL_INSTANTIATION
    short row = 1;
    short ballNumber = 0;

    float x, y;
    float ballGap = 2.0f;

    for (int i = 0; i < 6; i++)
    {
        x = -0.5f * ballRadius * 2 * i + screenWidth / 2 - ballRadius + ballGap;

        for (int j = 0; j < i; j++)
        {
            ballNumber++;
            x += ballRadius * 2;
            y = i * sqrt(0.75f) * ballRadius * -2 + screenHeight / 3;
            balls[ballNumber - 1] = new pool::ball::Ball(ballNumber, { x, y }, ballRadius, false);
            balls[ballNumber - 1]->getPosition();
        }
    }
#pragma endregion

    const float max_speed_magnitude = 150;
    const float speed_multiplier = .5;

    // Main game loop
    while (!WindowShouldClose())
    {

#pragma region BALL_INPUT
        Vector2 whiteBallPosition = whiteBall->getPosition();
        Vector2 mousePosition = { static_cast<float>(GetMouseX()), static_cast<float>(GetMouseY()) };

        Vector2 ballSpeedDirection = { whiteBallPosition.x - mousePosition.x,  whiteBallPosition.y - mousePosition.y };

        ballSpeedDirection.x *= speed_multiplier;
        ballSpeedDirection.y *= speed_multiplier;

        float magnitude = sqrtf(ballSpeedDirection.x * ballSpeedDirection.x + ballSpeedDirection.y * ballSpeedDirection.y);
        
        if (magnitude > max_speed_magnitude)
        {
            ballSpeedDirection = { ballSpeedDirection.x / magnitude * max_speed_magnitude, ballSpeedDirection.y / magnitude * max_speed_magnitude };
        }

        bool stillMoving = whiteBall->getMoving() && !whiteBall->getHole();

        for (short i = 0; i < amountBalls; i++)
        {
            if (balls[i]->getMoving() && !balls[i]->getHole())
                stillMoving = true;
        }

        if (!stillMoving && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            whiteBall->setSpeed(ballSpeedDirection);
        }
#pragma endregion

#pragma region PHYSICS_MOVEMENT_UPDATE
        whiteBall->update();

        for (short i = 0; i < amountBalls; i++)
        {
            balls[i]->update();
        }
#pragma endregion

#pragma region COLLISION_CHECK
        
        // Colision con las paredes
        collisions::setCollisionBallWall(whiteBall, field->getDimentions());

        for (short i = 0; i < amountBalls; i++)
        {
            collisions::setCollisionBallWall(balls[i], field->getDimentions());
        }

        // Colision bola blanca
        for (short i = 0; i < amountBalls; i++)
        {
            if (collisions::checkCollisionBallBall(balls[i], whiteBall))
            {
                collisions::collisionList.push_back({ balls[i], whiteBall });
            }
        }

        // Colision entre bolas
        for (short i = 0; i < amountBalls; i++)
        {
            for (short j = 0; j < amountBalls; j++)
            {
                if (collisions::checkCollisionBallBall(balls[j], balls[i]) && i != j)
                {
                    collisions::collisionList.push_back({ balls[j], balls[i] });
                }
            }
        }

#pragma endregion

#pragma region APPLY_PHYSICS

        // Aplicacion de la fisica y overlap
        vector <pair<ball::Ball*, ball::Ball*>>::iterator it;
        for (it = collisions::collisionList.begin(); it < collisions::collisionList.end(); it++)
        {
            collisions::setCollisionBallBall(it->first, it->second);
            collisions::setCollisionBallBall(it->second, it->first);
        }

        // Reseteo lista colisiones
        collisions::collisionList.clear();
#pragma endregion

#pragma region IN_HOLE
        // Checkeo in hole
        collisions::checkCollisionBallPosition(whiteBall, field);

        for (short j = 0; j < amountBalls; j++)
        {
            collisions::checkCollisionBallPosition(balls[j], field);
        }

        // Reseteo bola blanca in hole
        if (whiteBall->getHole() && !stillMoving)
        {
            whiteBall->setPosition({ screenWidth / 2,  screenHeight / 1.5f });
            whiteBall->setSpeed({ 0,0 });
            whiteBall->setHole(false);
        }
#pragma endregion

#pragma region DRAW
        BeginDrawing();

        ClearBackground(BLACK);
        field->draw();

        for (int j = 0; j < amountBalls; j++)
        {
            balls[j]->draw();
        }

        whiteBall->draw();

        if (!stillMoving)
        {
            DrawLineEx(whiteBall->getPosition(), { whiteBall->getPosition().x + ballSpeedDirection.x, whiteBall->getPosition().y + ballSpeedDirection.y }, 2.0f, RED);
            stick->draw(whiteBall->getPosition(), ballRadius);
        }

        EndDrawing();
#pragma endregion

    }

#pragma region DEINITIALIZATION
    CloseWindow();

    return 0;
#pragma endregion

}