#include "NavComponent.h"
#include "Tile.h"


NavComponent::NavComponent(class Actor* owner, int updateOrder)
	:MoveComponent(owner, updateOrder),
	mNextNode(nullptr)
{

}

void NavComponent::Update(float deltaTime)
{
	if (mNextNode)
	{
		// if we're at the next node, advance along path
		Vector2 diff = mOwner->GetPosition() - mNextNode->GetPosition();
		if (Math::NearZero(diff.Length(), 2.0f))
		{
			mNextNode = mNextNode->GetParent();
			TurnTo(mNextNode->GetPosition());
		}
	}

	MoveComponent::Update(deltaTime);
}

void NavComponent::StartPath(const Tile* start)
{
	mNextNode = start->GetParent();
	TurnTo(mNextNode->GetPosition());
}

void NavComponent::TurnTo(const Vector2& pos)
{
	// vector from me to pos
	Vector2 dir = pos - mOwner->GetPosition();
	// new angle is just atan2 of this dir vector
	// negate y beacuse +y is down on screen
	float angle = Math::Atan2(-dir.y, dir.x);
	mOwner->SetRotation(angle);
}
