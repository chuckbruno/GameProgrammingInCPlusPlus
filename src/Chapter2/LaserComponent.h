#pragma once
#include "SpriteComponent.h"
#include "Math.h"

class LaserComponent : public SpriteComponent
{
public:
	LaserComponent(class Actor* owner, int drawOrder = 101);

	// update / draw overridden from parent
	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer) override;

	Vector2 GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	void SetRotation(float rot) { mRotation = rot; }
	void SetSpeed(float speed) { mSpeed = speed; }
	float GetSpeed() const { return mSpeed; }

private:
	float mSpeed;
	Vector2 mPosition;
	float mRotation;
	Vector2 mOffset;
};
