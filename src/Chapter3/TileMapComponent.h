#pragma once

#include "SpriteComponent.h"
#include <vector>
#include <string>


class TileMapComponent : public SpriteComponent
{

public:
	TileMapComponent(class Actor* owner, int drawOrder = 90);

	// update / draw overridden from parent
	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer) override;

	void SetCSVFileName(const std::string& fileName);
	void ReadCSVFile();

private:
	std::vector<std::vector<int>> tileData;
	std::string tileFileName;
};