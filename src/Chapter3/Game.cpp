
#include "Game.h"

#include "SDL2/SDL_image.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Ship.h"
#include "Enemy.h"
#include "BGSpriteComponent.h"
#include "Utils.h"
#include "LaserComponent.h"
#include "TileMapComponent.h"
#include "Asteroid.h"
#include "Random.h"
#include "Laser.h"
#include "Explosion.h"


Game::Game()
	:mWindow(nullptr),
	mRenderer(nullptr),
	mIsRunning(true),
	mUpdatingActors(false)
{

}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("Chapter3", 100, 100, 1024, 768, 0);
	if (!mWindow)
	{
		SDL_Log("unable to create window: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		SDL_Log("unable to create renderer: %s", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("unable to create SDL_image: %s", SDL_GetError());
		return false;
	}

	LoadData();

	mTicksCount = SDL_GetTicks();

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

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(state);
	}

	mUpdatingActors = false;

	// process ship input
	//mship->ProcessKeyBoard(state);
}

void Game::UpdateGame()
{
	// compute delta time
	// wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	mTicksCount = SDL_GetTicks();

	// update all actors
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}

	mUpdatingActors = false;
	// move any pending actors to mActors

	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}

	mPendingActors.clear();

	// add any dead actors to a temp vector
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	//std::cout << mActors.size() << std::endl;

	// delete dead actors (which removes from mActors)
	for (auto actor : deadActors)
	{
		delete actor;
	}

	//SpawnEnemies();

}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	// draw all sprite components
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}

	SDL_RenderPresent(mRenderer);
}

void Game::LoadData()
{
	// create player's ship
	mship = new Ship(this);
	mship->SetPosition(Vector2(512.0f, 384.0f));
	mship->SetRotation(Math::PiOver2);
	//mship->SetScale(1.5f);

	// create enemy
	//Enemy* tmpEnemy = new Enemy(this);
	//tmpEnemy->SetPosition(Vector2(1350.0f, 500.0f));
	//tmpEnemy->SetVelocity(Vector2(-400.0f, 0.0f));
	//tmpEnemy->SetScale(1.0f);

	//tmpEnemy = new Enemy(this);
	//tmpEnemy->SetPosition(Vector2(1350.0f, 300.0f));
	//tmpEnemy->SetVelocity(Vector2(-300.0f, 0.0f));
	//tmpEnemy->SetScale(1.4f);

	// create actor for the background (this doesn't need a subclass)
	//Actor* temp = new Actor(this);
	//temp->SetPosition(Vector2(512.0f, 384.0f));
	//Explosion* ex = new Explosion(this);
	//ex->SetPosition(Vector2(512.0f, 184.0f));

	// create the "far back" background
	//BGSpriteCompoonent* bg = new BGSpriteCompoonent(temp);
	//bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	//std::vector<SDL_Texture*> bgtexs = {
	//	GetTexture("Assets/Farback01.png"),
	//	GetTexture("Assets/Farback02.png")
	//};

	//bg->SetBGTextures(bgtexs);
	//bg->SetScrolSpeed(-100.0f);
	//// create the closer background
	//bg = new BGSpriteCompoonent(temp);
	//bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	//bgtexs = {
	//	GetTexture("Assets/Stars.png"),
	//	GetTexture("Assets/Stars.png"),
	//};
	//bg->SetBGTextures(bgtexs);
	//bg->SetScrolSpeed(-200.0f);

	//TileMapComponent* tileMap = new TileMapComponent(temp, 98);
	//tileMap->SetCSVFileName("Assets/MapLayer1.csv");
	//tileMap->SetTexture(GetTexture("Assets/Tiles.png"));

	//tileMap = new TileMapComponent(temp, 95);
	//tileMap->SetCSVFileName("Assets/MapLayer2.csv");
	//tileMap->SetTexture(GetTexture("Assets/Tiles.png"));

	//tileMap = new TileMapComponent(temp, 90);
	//tileMap->SetCSVFileName("Assets/MapLayer3.csv");
	//tileMap->SetTexture(GetTexture("Assets/Tiles.png"));

	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; ++i)
	{
		new Asteroid(this);
	}
}

void Game::UnloadData()
{
	// delete actors
	// because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// destroy textures
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}

	mTextures.clear();
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	// is the texture already in the map
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else {
		// load from file
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		// create texture from surface 
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

		mTextures.emplace(fileName.c_str(), tex);
	}

	return tex;
}

void Game::AddAsteroid(class Asteroid* ast)
{
	mAsteroid.emplace_back(ast);
}

void Game::RemoveAsteroid(class Asteroid* ast)
{
	auto iter = std::find(mAsteroid.begin(), mAsteroid.end(), ast);
	if (iter != mAsteroid.end())
	{
		mAsteroid.erase(iter);
	}
}

void Game::Shutdown()
{
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	// if we're updating actors, need to add to pending
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// is it in pending actors
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// is it in actors?
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// swap to end of vector and pop off
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite)
{
	// find the insertion point in the sorted vector
	// the first element with a higher draw order than me

	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// inserts element before position of iterator
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	// we can't swap because it ruins ordering
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void Game::SpawnEnemies()
{
	srand(time(NULL));
	Enemy* tmpEnemy;

	if (mActors.size() < 7)
	{
		int N = static_cast<int>(rand() % 5);
		for (int i = 0; i < N; ++i)
		{
			int x = rand() % 10 + 1350.0f;
			int y = Utils::random(40, 728);
			int v_x = Utils::random(2, 20) * -60;
			tmpEnemy = new Enemy(this);
			tmpEnemy->SetPosition(Vector2(x, y));
			tmpEnemy->SetVelocity(Vector2(v_x, 0));
		}
	}
}
