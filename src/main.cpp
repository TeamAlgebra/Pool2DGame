#include "raylib.h"
#include "Config.h"
#include "Objects/Field/Field.h"
#include "Objects/Ball/Ball.h"
#include "Objects/Stick/Stick.h"
#include "Collision Tester/Collision Tester.h"

#include <cmath>
#include <iostream>

using namespace pool;

const float max_speed_magnitude = 150;
const float speed_multiplier = .5;
const int amountBalls = 15;

#pragma region GAME_STATE
bool Player1_turn = true;
bool Player2_turn = false;
bool Launched = false;
bool first_ball = false;

int player_pair = -1;

field::Field* _field;
ball::Ball* _balls[amountBalls];
ball::Ball* _whiteBall;
stick::Stick* _stick;
#pragma endregion

void reset_ball_positions()
{
	short row = 1;
	short ballNumber = 0;

	float x, y;
	float ballGap = 2.0f;

	for (int i = 0; i < 6; i++)
	{
		x = -ballRadius * i + screenWidth / 2 - ballRadius + ballGap;

		for (int j = 0; j < i; j++)
		{
			ballNumber++;
			x += ballRadius * 2;
			y = i * sqrt(0.75f) * ballRadius * -2 + screenHeight / 3;

			_balls[ballNumber - 1]->setPosition({ x, y });
			_balls[ballNumber - 1]->setSpeed({ 0, 0 });
			_balls[ballNumber - 1]->setHole(false);
		}
	}
}

void restart_game()
{
	// reseteo de bolas
	reset_ball_positions();

	// reseteo bola blanca
	_whiteBall->setHole(false);
	_whiteBall->setSpeed({ 0,0 });
	_whiteBall->setPosition({ screenWidth / 2,  screenHeight / 1.5f });
	_whiteBall->setColor(WHITE);

	// otros
	Player1_turn = true;
	Player2_turn = false;
	Launched = false;
	first_ball = false;
	player_pair = -1;
	std::cout << (Player1_turn ? "Player 1 turn" : "Player 2 turn") << " " << "PLAYER_PAIR" << ":" << player_pair << std::endl;
}

void init_game()
{
	InitWindow(screenWidth, screenHeight, "Pablo Bianco - Pool @60fps");
	SetTargetFPS(60);

	_field = new pool::field::Field();
	_whiteBall = new pool::ball::Ball(0, { screenWidth / 2,  screenHeight / 1.5f }, ballRadius, true);
	_whiteBall->setColor(WHITE);
	for (short i = 0; i < amountBalls; i++)
	{
		_balls[i] = new pool::ball::Ball(i + 1, { 0, 0 }, ballRadius, false);
		if ((i + 1) % 2 == 0)
			_balls[i]->setColor(RED);
		else
			_balls[i]->setColor(BLUE);

		if (i + 1 == 8)
			_balls[i]->setColor(BLACK);
	}

	reset_ball_positions();

	_stick = new pool::stick::Stick();
	_stick->init();
	std::cout << (Player1_turn ? "Player 1 turn" : "Player 2 turn") << " " << "PLAYER_PAIR" << ":" << player_pair << std::endl;
}

void set_player_color()
{
	if (!first_ball && !_balls[7]->getHole())
	{
		for (short i = 0; i < amountBalls; i++)
		{
			if (_balls[i]->getHole())
			{
				first_ball = true;
				auto isPair = _balls[i]->getValue() % 2 == 0;
				if (Player1_turn) player_pair = isPair ? 1 : 2;
				if (Player2_turn) player_pair = isPair ? 2 : 1;
			}
		}
	}
	else
	{
		int pair = 0;
		int non_pair = 0;
		for (short i = 0; i < amountBalls; i++)
		{
			if (!_balls[i]->getHole())
			{
				if (_balls[i]->getValue() % 2 == 0)
					pair++;
				else
					non_pair++;
			}
		}

		// CHECK BLACK BALL
		if (_balls[7]->getHole())
		{
			if (Player1_turn)
			{
				if (player_pair == 1)
				{
					if (pair > 0)
					{
						std::cout << "player1 loses" << std::endl;
					}
					else {
						std::cout << "player1 wins" << std::endl;
					}
				}
				else
				{
					if (non_pair > 0)
					{
						std::cout << "player1 loses" << std::endl;
					}
					else {
						std::cout << "player1 wins" << std::endl;
					}
				}
			}
			else if (Player2_turn)
			{
				if (player_pair == 2)
				{
					if (pair > 0)
					{
						std::cout << "player2 loses" << std::endl;
					}
					else {
						std::cout << "player2 wins" << std::endl;
					}
				}
				else
				{
					if (non_pair > 0)
					{
						std::cout << "player2 loses" << std::endl;
					}
					else {
						std::cout << "player2 wins" << std::endl;
					}
				}
			}
			restart_game();
		}
	}
}

int main()
{
	init_game();

	// Main game loop
	while (!WindowShouldClose())
	{
		if (IsKeyDown(KEY_R))
			restart_game();

#pragma region BALL_INPUT
		Vector2 whiteBallPosition = _whiteBall->getPosition();
		Vector2 mousePosition = { static_cast<float>(GetMouseX()), static_cast<float>(GetMouseY()) };

		Vector2 ballSpeedDirection = { whiteBallPosition.x - mousePosition.x,  whiteBallPosition.y - mousePosition.y };

		ballSpeedDirection.x *= speed_multiplier;
		ballSpeedDirection.y *= speed_multiplier;

		float magnitude = sqrtf(ballSpeedDirection.x * ballSpeedDirection.x + ballSpeedDirection.y * ballSpeedDirection.y);

		if (magnitude > max_speed_magnitude)
		{
			ballSpeedDirection = { ballSpeedDirection.x / magnitude * max_speed_magnitude, ballSpeedDirection.y / magnitude * max_speed_magnitude };
		}

		bool stillMoving = _whiteBall->getMoving() && !_whiteBall->getHole();

		for (short i = 0; i < amountBalls; i++)
		{
			if (_balls[i]->getMoving() && !_balls[i]->getHole())
				stillMoving = true;
		}

		if (!stillMoving && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		{
			Launched = true;
			stillMoving = true;
			_whiteBall->setSpeed(ballSpeedDirection);
		}
#pragma endregion

#pragma region PHYSICS_MOVEMENT_UPDATE
		_whiteBall->update();

		for (short i = 0; i < amountBalls; i++)
		{
			_balls[i]->update();
		}
#pragma endregion

#pragma region COLLISION_CHECK

		// Colision con las paredes
		collisions::setCollisionBallWall(_whiteBall, _field->getDimentions());

		for (short i = 0; i < amountBalls; i++)
		{
			collisions::setCollisionBallWall(_balls[i], _field->getDimentions());
		}

		// Colision bola blanca
		for (short i = 0; i < amountBalls; i++)
		{
			if (collisions::checkCollisionBallBall(_balls[i], _whiteBall))
			{
				collisions::collisionList.push_back({ _balls[i], _whiteBall });
			}
		}

		// Colision entre bolas
		for (short i = 0; i < amountBalls; i++)
		{
			for (short j = 0; j < amountBalls; j++)
			{
				if (collisions::checkCollisionBallBall(_balls[j], _balls[i]) && i != j)
				{
					collisions::collisionList.push_back({ _balls[j], _balls[i] });
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
		collisions::checkCollisionBallPosition(_whiteBall, _field);

		for (short j = 0; j < amountBalls; j++)
		{
			collisions::checkCollisionBallPosition(_balls[j], _field);
		}

		// Cambio de turno
		if (Launched && !stillMoving)
		{
			set_player_color();
			Player1_turn = Player1_turn ? false : true;
			Player2_turn = Player2_turn ? false : true;
			Launched = false;

			if (Player1_turn && player_pair > 0)
			{
				_whiteBall->setColor(player_pair == 1 ? ORANGE : SKYBLUE);
			}
			if (Player2_turn && player_pair > 0)
			{
				_whiteBall->setColor(player_pair == 2 ? ORANGE : SKYBLUE);
			}

			std::cout << (Player1_turn ? "Player 1 turn" : "Player 2 turn") << " " << "PLAYER_PAIR" << ":" << player_pair << std::endl;
		}

		// Reseteo bola blanca in hole
		if (_whiteBall->getHole() && !stillMoving)
		{
			_whiteBall->setPosition({ screenWidth / 2,  screenHeight / 1.5f });
			_whiteBall->setSpeed({ 0,0 });
			_whiteBall->setHole(false);
		}
#pragma endregion

#pragma region DRAW
		BeginDrawing();

		ClearBackground(BLACK);
		_field->draw();

		for (int j = 0; j < amountBalls; j++)
		{
			_balls[j]->draw();
		}

		_whiteBall->draw();

		if (!stillMoving)
		{
			DrawLineEx(_whiteBall->getPosition(), { _whiteBall->getPosition().x + ballSpeedDirection.x, _whiteBall->getPosition().y + ballSpeedDirection.y }, 2.0f, RED);
			_stick->draw(_whiteBall->getPosition(), ballRadius);
		}

		EndDrawing();
#pragma endregion

	}

#pragma region DEINITIALIZATION
	CloseWindow();

	return 0;
#pragma endregion

}