#include "Explosion.h"
#include "SpriteComponent.h"
#include "AnimSpriteComponent.h"
#include "AtlasComponent.h"
#include "Game.h"


Explosion::Explosion(Game* game)
	:Actor(game),
	mDeathTime(1.0f),
	mAtlas(nullptr)
{
	// create sprite component
	mAtlas = new AtlasComponent(this);
	mAtlas->SetTexture(game->GetTexture("Assets/explosion.png"));
	mAtlas->SetRow(6);
	mAtlas->SetCol(8);
	mAtlas->SetAnimFPS(48.0f);
}

void Explosion::UpdateActor(float deltaTime)
{
	mDeathTime -= deltaTime;

	if (mDeathTime < 0.0f)
	{
		SetState(Actor::EDead);
	}
}
