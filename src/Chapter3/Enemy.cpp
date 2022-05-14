
#include "Enemy.h"
#include "AnimSpriteComponent.h"
#include "Game.h"


Enemy::Enemy(Game* game)
	:Actor(game),
	mPosition(0.0f, 0.0f),
	mVelocity(0.0f, 0.0f)
{
	// create an animated sprite component
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> anims = {

		game->GetTexture("Assets/Enemy01.png"),
		game->GetTexture("Assets/Enemy02.png"),
		game->GetTexture("Assets/Enemy03.png"),
		game->GetTexture("Assets/Enemy04.png"),
		game->GetTexture("Assets/Enemy05.png"),
		game->GetTexture("Assets/Enemy06.png"),
	};

	asc->SetAnimTextures(anims);
}

void Enemy::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	// update position based on speeds and detla time
	Vector2 pos = GetPosition();
	pos.x += mVelocity.x * deltaTime;
	pos.y += mVelocity.y * deltaTime;
	// restrict positions to left half of screen
	if (pos.x < -35.0f)
	{
		//pos.x = 25;
		SetState(Actor::EDead);
	}
	//else if (pos.x > 500.0f)
	//{
	//	pos.x = 500.0f;
	//}

	//if (pos.y < 25.0f)
	//{
	//	pos.y = 25.0f;
	//}
	//else if (pos.y > 743.0f)
	//{
	//	pos.y = 743.0f;
	//}

	SetPosition(pos);
}

void Enemy::SetVelocity(Vector2 velocity)
{
	mVelocity = velocity;
}

