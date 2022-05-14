
#include "Ship.h"
#include "Actor.h"

#include "Game.h"
#include "AnimSpriteComponent.h"
#include "LaserComponent.h"

Ship::Ship(Game* game):
	Actor(game),
	mRightSpeed(0.0f),
	mDownSpeed(0.0f)
{
	// create an animated sprite component
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> anims = {

		game->GetTexture("Assets/Ship01.png"),
		game->GetTexture("Assets/Ship02.png"),
		game->GetTexture("Assets/Ship03.png"),
		game->GetTexture("Assets/Ship04.png"),
	};

	asc->SetAnimTextures(anims);

}

void Ship::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	// update position based on speeds and detla time
	Vector2 pos = GetPosition();
	pos.x += mRightSpeed * deltaTime;
	pos.y += mDownSpeed * deltaTime;
	// restrict positions to left half of screen
	if (pos.x < 25.0f)
	{
		pos.x = 25;
	}
	else if (pos.x > 500.0f)
	{
		pos.x = 500.0f;
	}
	
	if (pos.y < 25.0f)
	{
		pos.y = 25.0f;
	}
	else if (pos.y > 743.0f)
	{
		pos.y = 743.0f;
	}

	SetPosition(pos);
}

void Ship::ProcessKeyBoard(const uint8_t* state)
{
	mRightSpeed = 0.0f;
	mDownSpeed = 0.0f;

	bool trigger = true;

	float rot = 0.0f;
	// right / left
	if (state[SDL_SCANCODE_D])
	{
		mRightSpeed += 250.0f;
	}
	if (state[SDL_SCANCODE_A])
	{
		mRightSpeed -= 250.0f;
	}
	// up and down
	if (state[SDL_SCANCODE_S])
	{
		mDownSpeed += 300.0f;
	}
	if (state[SDL_SCANCODE_W])
	{
		mDownSpeed -= 300.0f;
	}

	if (state[SDL_SCANCODE_LEFT])
	{
		rot = GetRotation();
		rot += (5 / 180.0) * 3.14f;
		SetRotation(rot);
	}

	if (state[SDL_SCANCODE_RIGHT])
	{
		rot = GetRotation();
		rot -= (5 / 180.0) * 3.14f;
		SetRotation(rot);
	}

	if (trigger)
	{
		if (state[SDL_SCANCODE_SPACE])
		{
			SDL_Texture* laserTexture = GetGame()->GetTexture("Assets/Laser.png");

			LaserComponent* laser = new LaserComponent(this);
			laser->SetTexture(laserTexture);
			laser->SetPosition(this->GetPosition());
			laser->SetRotation(this->GetRotation());
			laser->SetSpeed(1200.0f);
		}

		trigger = false;
	}

}