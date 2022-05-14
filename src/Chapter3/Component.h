#pragma once

#include <cstdint>


class Component
{
public:
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();
	// update this component by delta time
	virtual void Update(float deltaTime);
	virtual void ProcessInput(const uint8_t* keyState) {}

	int GetUpdateOrder() const { return mUpdateOrder; }
protected:
	// owning actor
	class Actor* mOwner;
	int mUpdateOrder;
};