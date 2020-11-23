#pragma once
#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "Objects/Ball/Ball.h"
#include "Objects/Field/Field.h"

const int amountBalls = 16;
class Game {

private:
	pool::ball::ball* balls[amountBalls];
	pool::field::Field* field;
public:
	Game();
	void update();
	void draw();
	void reinit();
};
#endif // !GAME_H
