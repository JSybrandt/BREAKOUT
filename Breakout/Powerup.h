#pragma once
#include "Actor.h"

enum PowerUpType{
	SPLIT,
	FLIP,
	GROW,
	SHRINK,
	SPACE_INVADERS,
	COUNT,
	NONE
};

namespace PowerUpNS{
	const Vector3 VELOCITY(0,-10,0);
	const float FLIP_DURATION=10;
	const float SPACE_DURATION = 10;
	const float SIZE_DURATION = 10;
};

class Powerup: public virtual Actor{

public:
	PowerUpType type;

	void update(float dt);
	void create(Vector3 pos, PowerUpType t);

	void drop();
};