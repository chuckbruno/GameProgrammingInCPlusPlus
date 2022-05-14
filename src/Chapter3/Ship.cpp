
#include "Ship.h"
#include "Actor.h"
#include <iostream>

#include "Game.h"
#include "AnimSpriteComponent.h"
#include "LaserComponent.h"
#include "Laser.h"
#include "InputComponent.h"
#include "CircleComponent.h"
#include "Asteroid.h"
#include "Explosion.h"


Ship::Ship(Game* game):
	Actor(game),
	mLaserCooldown(0.0f)
{
	// create an animated sprite component
	SpriteComponent* sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));

	// create input component and set keys/ speed
	InputComponent* ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	ic->SetMaxForwardSpeed(300.0f);
	ic->SetMaxAngularSpeed(Math::TwoPi);

	// create a circle component
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(50.0f);
}

void Ship::UpdateActor(float deltaTime)
{
	mLaserCooldown -= deltaTime;

	// do we intersect with an asteroid?
	for (auto ast : GetGame()->GetAsteroids())
	{
		if (Intersect(*mCircle, *(ast->GetCircle())))
		{
			// the first asteroid we intersect with
			// set ourselves and the asteroid to dead
			Vector2 circle1Center = mCircle->GetCenter();
			Vector2 circle2Center = ast->GetCircle()->GetCenter();

			Vector2 midPoint = (circle1Center + circle2Center) * 0.5f;
			Explosion* ex = new Explosion(GetGame());
			ex->SetPosition(midPoint);
			ex->SetScale(2.5f);

			SetState(Actor::EDead);
			ast->SetState(Actor::EDead);
			break;
		}
	}
}

void Ship::ActorInput(const uint8_t* keyState)
{
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
	{
		// create laser and set it position/ rotation to mine
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		// reset laser cool down 
		mLaserCooldown = 0.01f;
	}
}
