#include "LaserComponent.h"
#include "Actor.h"
#include "Math.h"

#include <iostream>

LaserComponent::LaserComponent(class Actor* owner, int drawOrder /*= 101*/)
	:SpriteComponent(owner, drawOrder),
	mPosition(0.0f, 0.0f),
	mSpeed(10.0f),
	mOffset(0.0f, 0.0f)
{

}

void LaserComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	mOffset.x += deltaTime * Math::Cos(-1 * mRotation) * mSpeed;
	mOffset.y += deltaTime * Math::Sin(-1 * mRotation) * mSpeed;
}

void LaserComponent::Draw(SDL_Renderer* renderer)
{
	if (mTexture)
	{
		SDL_Rect r;
		// scale the width/height by owner's scale
		r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
		r.h = static_cast<int>(mTexHeight * mOwner->GetScale());
		// center the rectangle around the position of the owner
		r.x = static_cast<int>(mPosition.x - r.w / 2 + mOffset.x + Math::Cos(-1 * mRotation) * 70);
		r.y = static_cast<int>(mPosition.y - r.w / 2 + mOffset.y + Math::Sin(-1 * mRotation) * 70);

		// draw (have to convert angle from radians to degrees, and clockwise to counter)
		SDL_RenderCopyEx(renderer, mTexture, nullptr, &r, -Math::ToDegrees(mRotation),
			nullptr, SDL_FLIP_NONE);

	}
}
