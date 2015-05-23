#pragma once
#include "Actor.h"

enum PowerUpType{
	SPLIT,
	FLIP,
	GROW,
	SHRINK,
	COUNT,
	NONE
};

namespace PowerUpNS{
	const Vector3 VELOCITY(0,-10,0);
};

class Powerup: public virtual Actor{

public:
	PowerUpType type;

	void update(float dt);
	void create(Vector3 pos, PowerUpType t);

	void drop();
};