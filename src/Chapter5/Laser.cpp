#include "Laser.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "CircleComponent.h"
#include "Asteroid.h"
#include <iostream>


Laser::Laser(Game* game)
	:Actor(game),
	mDeathTime(3.0f)
{
	// create a sprite component
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Laser.png"));

	// create a move component, and set a forward speed
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(800.0f);

	// create a circle component for collision
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(11.0f);
}

void Laser::UpdateActor(float deltaTime)
{
	// if we run out to time, Laser is dead
	mDeathTime -= deltaTime;
	if (mDeathTime <= 0.0)
	{
		SetState(Actor::EDead);
	}
	else
	{
		// do we intersect with an asteroid?
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*mCircle, *(ast->GetCircle())))
			{
				Vector2 circle1Center = mCircle->GetCenter();

				// the first asteroid we intersect with
				// set ourselves and the asteroid to dead
				SetState(Actor::EDead);
				ast->SetState(Actor::EDead);
				break;
			}
		}
	}
}
