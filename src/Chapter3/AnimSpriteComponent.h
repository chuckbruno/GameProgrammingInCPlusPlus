#pragma once

#include "SpriteComponent.h"
#include <vector>
#include <string>


class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	// update animation every frame (overridden from component)
	void Update(float deltaTime) override;
	// set the textures used for animation
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	// set/get the animation FPS
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }

private:
	// all textures in the animation
	std::vector<SDL_Texture*> mAnimTextures;

	// current frame displayed
	float mCurrFrame;
	// animation frame rate
	float mAnimFPS;
};