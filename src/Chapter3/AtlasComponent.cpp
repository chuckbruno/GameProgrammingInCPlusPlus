#include "AtlasComponent.h"
#include "Actor.h"
#include <iostream>



AtlasComponent::AtlasComponent(class Actor* owner, int drawOrder /*= 105*/)
	:SpriteComponent(owner, drawOrder),
	mRow(0),
	mCol(0),
	mCurrFrame(0.0f),
	mAnimFPS(24.0f)
{

}


void AtlasComponent::Draw(SDL_Renderer* renderer)
{
	if (mTexture && mRects.size() > 0)
	{
		SDL_Rect r;
		// scale the width/height by owner's scale
		r.w = mRects[0]->w;
		r.h = mRects[0]->h;
		// center the rectangle around the position of the owner
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
		r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

		// draw (have to convert angle from radians to degrees, and clockwise to counter)
		SDL_RenderCopyEx(renderer, mTexture, mRects[static_cast<int>(mCurrFrame)], &r, -Math::ToDegrees(mOwner->GetRotation()),
			nullptr, SDL_FLIP_NONE);

	}
}

void AtlasComponent::CalRect()
{
	int tileWidth = static_cast<int>(mTexWidth / mCol);
	int tileHeight = static_cast<int>(mTexHeight / mRow);

	for (int j = 0; j < mRow; j++)
	{
		for (int i = 0; i < mCol; i++)
		{
			SDL_Rect* rect = new SDL_Rect();
			rect->w = tileWidth;
			rect->h = tileHeight;
			rect->x = i * tileWidth;
			rect->y = j * tileHeight;

			mRects.emplace_back(rect);
		}
	}
}


AtlasComponent::~AtlasComponent()
{
	for (auto rect : mRects)
	{
		delete rect;
	}
}

void AtlasComponent::Update(float deltaTime)
{
	if (mRects.size() == 0)
	{
		CalRect();
	}

	SpriteComponent::Update(deltaTime);
	if (mRects.size() > 0)
	{
		// update the current frame based on frame rate and delta time
		mCurrFrame += mAnimFPS * deltaTime;
		// wrap current frame if needed
		if (mCurrFrame >= mRects.size())
		{
			mCurrFrame -= mRects.size();

		}
	}
}

