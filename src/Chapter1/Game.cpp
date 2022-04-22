
#include "Game.h"
//
#include <iostream>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


const int thickness = 15;
const float paddleH = 100.0f;


void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
	const int32_t diameter = (radius * 2);

	int32_t x = (radius - 1);
	int32_t y = 0;
	int32_t tx = 1;
	int32_t ty = 1;
	int32_t error = (tx - diameter);

	while (x >= y)
	{
		//  Each of the following renders an octant of the circle
		SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
		SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
		SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
		SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
		SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
		SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
		SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
		SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

		if (error <= 0)
		{
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0)
		{
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}
}


Game::Game() :mWindow(nullptr), mRenderer(nullptr), mTicksCount(0), mIsRunning(true), mPaddleDir(0)
{

}


bool Game::Initialize()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow(
		"Pong", 100, 100, 1024, 768, 0
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(
		mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	mPaddleLeft.pos.x = 10.0f;
	mPaddleLeft.pos.y = 768.0f / 2.0f;
	mPaddleLeft.dir = 0;

	mPaddleRight.pos.x = 1024.f - 40.0f;
	mPaddleRight.pos.y = 768.0f / 2.0f;
	mPaddleRight.dir = 0;

	mPaddlePos.x = 10.0f;
	mPaddlePos.y = 768.0f / 2.0f;
	mBallPos.x = 1024.0f / 2.0f;
	mBallPos.y = 768.0f / 2.0f;
	//mBallVel.x = 0.0f;
	mBallVel.x = 200.0f;
	mBallVel.y = 235.0f;

	mGravity = { 0, 200 };

	GenerateBalls(mBalls);

	return true;

}


void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}


void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);

	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	// Update paddle direction based on W/S keys
	mPaddleDir = 0;
	mPaddleRight.dir = 0;
	mPaddleLeft.dir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddleDir -= 1;
		mPaddleLeft.dir = -1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddleDir += 1;
		mPaddleLeft.dir = 1;
	}
	if (state[SDL_SCANCODE_I])
	{
		mPaddleRight.dir = -1;
	}
	if (state[SDL_SCANCODE_K])
	{
		mPaddleRight.dir = 1;
	}
}


void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the difference in ticks from last frame
	// converted to seconds
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();

	// Update paddle counts (for next frame)
	//if (mPaddleDir != 0)
	//{
	//	mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
	//	// Make sure paddle doesn't screen!
	//	if (mPaddlePos.y < (paddleH / 2.0f + thickness))
	//	{
	//		mPaddlePos.y = paddleH / 2.0f + thickness;
	//	}
	//	else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
	//	{
	//		mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
	//	}
	//}

	UpdatePaddlePos(mPaddleLeft, deltaTime);
	UpdatePaddlePos(mPaddleRight, deltaTime);

	//mBallVel.x += mGravity.x;
	//mBallVel.y += mGravity.y;

	// Update ball position based on ball velocity
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	for (int i = 0; i < mBalls.size(); ++i)
	{
		mBalls[i].pos.x += mBalls[i].velocity.x * deltaTime;
		mBalls[i].pos.y += mBalls[i].velocity.y * deltaTime;
	}

	// Bounce if needed
	// Did we intersect with the paddle?
	float diff = mPaddlePos.y - mBallPos.y;

	float paddleLeftDiff = mPaddleLeft.pos.y - mBallPos.y;
	float paddleRightDiff = mPaddleRight.pos.y - mBallPos.y;

	// Take absolute value of difference
	diff = (diff > 0.0f) ? diff : -diff;
	paddleLeftDiff = (paddleLeftDiff > 0.0f) ? paddleLeftDiff : -paddleLeftDiff;
	paddleRightDiff = (paddleRightDiff > 0.0f) ? paddleRightDiff : -paddleRightDiff;

	if (mBallPos.x < 0.0f && mBallPos.x > -10.0f)
	{
		ResetBallPos();
	}
	if (
		// Our y-difference is small enough
		paddleLeftDiff <= paddleH / 2.0f &&
		//diff <= paddleH / 2.0f &&
		// We are in the correct x-position
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		// The ball is moving to the left
		mBallVel.x < 0.0f
		)
	{
		mBallVel.x *= -1.0f;
	}
	if (
		// Our y-difference is small enough
		paddleRightDiff <= paddleH / 2.0f &&
		//diff <= paddleH / 2.0f &&
		// We are in the correct x-position
		mBallPos.x <= 1024.0f && mBallPos.x >= 975.0f &&
		// The ball is moving to the left
		mBallVel.x > 0.0f
		)
	{
		mBallVel.x *= -1.0f;
	}
	//else if (mBallPos.x <= 0.0f)
	//{
	//	mIsRunning = true;
	//}
	else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1.0f;
	}

	// Did the ball collide with the top wall?
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
	}
	else if (mBallPos.y >= (768 - thickness) && mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1;
	}

	for (int i = 0; i < mBalls.size(); ++i)
	{
		if (mBalls[i].pos.x >= (1024.0f - thickness) && mBalls[i].velocity.x > 0.0f)
		{
			mBalls[i].velocity.x *= -1 * 0.7;
		}

		// Did the ball collide with the top wall?
		if (mBalls[i].pos.y <= thickness && mBalls[i].velocity.y < 0.0f)
		{
			mBalls[i].velocity.y *= -1 * 0.9;
		}
		else if (mBalls[i].pos.y >= (768 - thickness) && mBalls[i].velocity.y > 0.0f)
		{
			mBalls[i].velocity.y *= -1 * 0.8;
		}

	}

}


void Game::GenerateOutput()
{
	// Set draw color to blue
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);

	// Clear back buffer
	SDL_RenderClear(mRenderer);

	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	// Draw top wall

	SDL_Rect wall{
		0, // top left x
		0, // top lelft y
		1024, // width
		thickness // height
	};

	//DrawCircle(mRenderer, mBallPos.x, mBallPos.y, 20);

	SDL_RenderFillRect(mRenderer, &wall);

	// Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw right wall
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;

	SDL_RenderFillRect(mRenderer, &wall);

	// Draw paddle
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH / 2.0f),
		thickness,
		static_cast<int>(paddleH)
	};

	SDL_Rect paddleLeft{
		static_cast<int>(mPaddleLeft.pos.x),
		static_cast<int>(mPaddleLeft.pos.y - paddleH / 2.0f),
		thickness,
		static_cast<int>(paddleH)

	};

	SDL_Rect paddleRight
	{
		static_cast<int>(mPaddleRight.pos.x),
		static_cast<int>(mPaddleRight.pos.y - paddleH / 2.0f),
		thickness,
		static_cast<int>(paddleH)
	};

	SDL_RenderFillRect(mRenderer, &paddleLeft);
	SDL_RenderFillRect(mRenderer, &paddleRight);

	//SDL_RenderFillRect(mRenderer, &paddle);

	// Draw Ball Color
	srand(time(NULL));
	int red = rand() % 255;
	int green = rand() % 255;
	int blue = rand() % 255;
	SDL_SetRenderDrawColor(mRenderer, red, green, blue, 255);
	// Draw ball
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2.0f),
		static_cast<int>(mBallPos.y - thickness / 2.0f),
		thickness,
		thickness
	};

	SDL_RenderFillRect(mRenderer, &ball);

	for (int i = 0; i < mBalls.size(); ++i)
	{
		SDL_Rect _ball{
			static_cast<int>(mBalls[i].pos.x - thickness / 2.0f),
			static_cast<int>(mBalls[i].pos.y - thickness / 2.0f),
			thickness,
			thickness
		};

		SDL_RenderFillRect(mRenderer, &_ball);
	}

	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);


}

void Game::ResetBallPos()
{
	mBallPos.x = 1024.0f / 2.0f;
	mBallPos.y = 768.0f / 2.0f;
	mBallVel.x = 200.0f;
	mBallVel.y = 235.0f;
}

//	mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
//	// Make sure paddle doesn't screen!
//	if (mPaddlePos.y < (paddleH / 2.0f + thickness))
//	{
//		mPaddlePos.y = paddleH / 2.0f + thickness;
//	}
//	else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
//	{
//		mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
//	}

void Game::UpdatePaddlePos(Paddle& paddle, float deltaTime)
{
	if (paddle.dir != 0)
	{
		paddle.pos.y += paddle.dir * 300.0f * deltaTime;
		// Make sure paddle doesn't screen!
		if (paddle.pos.y < (paddleH / 2.0f + thickness))
		{
			paddle.pos.y = (paddleH / 2.0f + thickness);
		}
		else if (paddle.pos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			paddle.pos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

}

void Game::GenerateBalls(std::vector<Ball>& balls)
{
	srand(3);

	for (int i = 0; i < 10; ++i)
	{
		Ball ball;
		ball.pos = { static_cast<float>((rand()) % 10 * i * 12) , static_cast<float>(rand() % 10 * 3) };
		ball.velocity = { static_cast<float>(rand() % 10 * 15) , static_cast<float>(rand() % 100 * 9) };
		balls.push_back(std::move(ball));
	}
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

