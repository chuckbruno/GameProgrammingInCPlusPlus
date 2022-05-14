#pragma once

#include <iostream>
#include "Actor.h"

class Enemy : public Actor
{
public:
	Enemy(Game* game);
	~Enemy()
	{
		std::cout << "Destroy Enemy" << std::endl;
	}
	void UpdateActor(float deltaTime) override;
	void SetVelocity(Vector2 velocity);

private:
	Vector2 mPosition;
	Vector2 mVelocity;

};