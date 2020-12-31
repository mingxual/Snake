#pragma once
#include "Component.h"

class MoveComponent : public Component
{
public:
	MoveComponent(class Actor* owner);

	// Update the move component
	void Update(float deltaTime) override;
	
	// Getters/setters
	float GetForwardSpeed() const { return mForwardSpeed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }

private:
	// Forward speed (in pixels/second)
	float mForwardSpeed;
};
