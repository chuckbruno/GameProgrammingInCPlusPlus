#pragma once

#include "SpriteComponent.h"
#include <vector>


class AtlasComponent : public SpriteComponent
{
public:
	AtlasComponent(class Actor* owner, int drawOrder = 105);
	~AtlasComponent();
	void Update(float deltaTime) override;

	void Draw(SDL_Renderer* renderer);
	void SetRow(unsigned int row) { mRow = row; }
	void SetCol(unsigned int col) { mCol = col; }
	unsigned int GetRow() const { return mRow; }
	unsigned int GetCol() const { return mCol; }

	void SetAnimFPS(float fps) { mAnimFPS = fps; }
	float GetAnimFPS() const { return mAnimFPS; }

	void CalRect();

private:
	std::vector<SDL_Rect*> mRects;

	// play tile texture as animation
	unsigned int mRow;
	unsigned int mCol;

	// current frame displayed
	float mCurrFrame;
	// animation frame rate
	float mAnimFPS;
};
