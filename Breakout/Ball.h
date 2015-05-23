#pragma once
#include "Actor.h"

namespace BallNS{
	const float SPEED = 10;
};

class Ball: public virtual Actor{
public:
	Ball(){blockCombo = 0;}
	void hitBlock(Actor* a);
	void hitWall(Actor* a);
	void hitPaddle(Actor* a);
	void update(float dt);
private:
	Vector3 prevLoc;
	int blockCombo;
};