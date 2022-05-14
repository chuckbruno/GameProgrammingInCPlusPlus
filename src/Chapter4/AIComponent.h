#pragma once

#include "Component.h"
#include <unordered_map>
#include <string.h>


class AIComponent : public Component
{
public:
	AIComponent(class Actor* owner);
	void Update(float deltaTime) override;
	void ChangeState(const std::string& name);

	// add a new state to the map
	void RegisterState(class AIState* state);

private:
	// maps name of state of AIState instance
	std::unordered_map<std::string, class AIState*> mStateMap;
	// current state we're in
	class AIState* mCurrentState;
};