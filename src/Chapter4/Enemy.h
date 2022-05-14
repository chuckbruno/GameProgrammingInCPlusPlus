#pragma once

#include <iostream>
#include "Actor.h"

class Enemy : public Actor
{
public:
	Enemy(Game* game);
	~Enemy();
	void UpdateActor(float deltaTime) override;
	class CircleComponent* GetCircle() { return mCircle; }

private:
	class CircleComponent* mCircle;

};