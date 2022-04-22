#pragma once
#pragma once


#include <SDL2/SDL.h>
#include <vector>


struct Vector2 {
	float x;
	float y;
};


class Paddle
{
public:
	Vector2 pos;
	int dir;
};


struct Ball
{
	Vector2 pos;
	Vector2 velocity;
};


class Game
{
public:
	Game();
	//Initialize the game
	bool Initialize();

	// Runs the games loop until the game is over
	void RunLoop();

	// Shutdown the game
	void Shutdown();

private:
	// Helper functions for the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	void ResetBallPos();

	void UpdatePaddlePos(Paddle& paddle, float deltaTime);
	void GenerateBalls(std::vector<Ball>& balls);

	// Window created by SDL
	SDL_Window* mWindow;

	// Renderer for 2D drawing
	SDL_Renderer* mRenderer;
	// Number of ticks since starts of game
	Uint32 mTicksCount;

	// Game should continue to run
	bool mIsRunning;

	// Pong specific
	// Direction of paddle
	int mPaddleDir;
	// Position of paddle
	Vector2 mPaddlePos;
	// Position of ball
	Vector2 mBallPos;
	// Velocity of ball
	Vector2 mBallVel;

	Paddle mPaddleLeft;
	Paddle mPaddleRight;

	Vector2 mGravity;

	std::vector<Ball> mBalls;


};
