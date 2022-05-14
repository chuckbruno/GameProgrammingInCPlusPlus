#pragma once

#include "SpriteComponent.h"
#include <vector>
#include "Math.h"

class BGSpriteCompoonent : public SpriteComponent
{
public:
	// set draw order to default to lower 
	BGSpriteCompoonent(class Actor* owner, int drawOrder = 10);
	// update / draw overridden from parent
	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer) override;
	// set the textures used for the background
	void SetBGTextures(const std::vector<SDL_Texture*>& textures);
	// get set screen size and scroll speed
	void SetScreenSize(const Vector2& size) { mScreenSize = size; }
	void SetScrolSpeed(float speed) { mScrollSpeed = speed; }
	float GetScrollSpeed() const { return mScrollSpeed; }

private:
	// struct to encapsulate each bg image and its offset
	struct BGTexture
	{
		SDL_Texture* mtexture;
		Vector2 mOffset;
	};

	std::vector<BGTexture> mBGTextures;
	Vector2 mScreenSize;
	float mScrollSpeed;
};