
#include "BGSpriteComponent.h"
#include "Actor.h"

BGSpriteCompoonent::BGSpriteCompoonent(class Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder),
	mScrollSpeed(0.0f)
{

}

void BGSpriteCompoonent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);
	for (auto& bg : mBGTextures)
	{
		// update the x offset
		bg.mOffset.x += mScrollSpeed * deltaTime;
		// if this is completely off the screen, reset offset to
		// the right of the last by texture
		if (bg.mOffset.x < -mScreenSize.x)
		{
			bg.mOffset.x = (mBGTextures.size() - 1) * mScreenSize.x - 1;
		}
	}
}

void BGSpriteCompoonent::Draw(SDL_Renderer* renderer)
{
	// draw each background texture
	for (auto& bg : mBGTextures)
	{
		SDL_Rect r;
		// assume screen size dimensions
		r.w = static_cast<int>(mScreenSize.x);
		r.h = static_cast<int>(mScreenSize.y);
		// center the rectangle around the position of the owner
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2 + bg.mOffset.x);
		r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2 + bg.mOffset.y);

		// Draw this background
		SDL_RenderCopy(renderer, bg.mtexture, nullptr, &r);
	}
}

void BGSpriteCompoonent::SetBGTextures(const std::vector<SDL_Texture*>& textures)
{
	int count = 0;
	for (auto tex : textures)
	{
		BGTexture temp;
		temp.mtexture = tex;
		// each texture is screen width in offset
		temp.mOffset.x = count * mScreenSize.x;
		temp.mOffset.y = 0;
		mBGTextures.emplace_back(temp);
		count++;
	}
}