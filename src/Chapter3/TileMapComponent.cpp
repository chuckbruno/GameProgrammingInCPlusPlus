
#include "TileMapComponent.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include "Actor.h"


TileMapComponent::TileMapComponent(class Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
{

}

void TileMapComponent::Update(float deltaTime)
{

}

void TileMapComponent::Draw(SDL_Renderer* renderer)
{
	ReadCSVFile();

	if (mTexture)
	{
		int tileWidth = mTexWidth / 8;
		SDL_Rect srect, drect;
		int row, col, tileNum;

		for (int i = 0; i < tileData.size(); ++i)
		{
			for (int j = 0; j < tileData[i].size(); ++j)
			{
				if (tileData[i][j] == -1)
					continue;

				tileNum = tileData[i][j];
				row = static_cast<int>((tileNum + 1) / 8);
				col = tileNum % 8;
				srect.x = col * 32;
				srect.y = row * 32;
				srect.w = srect.h = 32;
				drect.x = j * 32; drect.y = i * 32;
				drect.w = drect.h = 32;

				SDL_RenderCopyEx(renderer, mTexture, &srect, &drect, 0,
					nullptr, SDL_FLIP_NONE);
			}
		}
	}
}

void TileMapComponent::SetCSVFileName(const std::string& fileName)
{
	tileFileName = fileName;
}

void TileMapComponent::ReadCSVFile()
{
	std::string line;
	std::ifstream fstream(this->tileFileName.c_str());
	size_t pos = -1;

	//this->tileData.clear();

	if (fstream && !this->tileData.size())
	{
		while (std::getline(fstream, line))
		{

			std::istringstream sstream(line);
			std::string tileCode;
			std::vector<int> row;
			while (std::getline(sstream, tileCode, ','))
			{
				row.push_back(std::stoi(tileCode.c_str()));
			}
			this->tileData.push_back(row);
		}
	}
}
