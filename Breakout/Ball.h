#pragma once
#include "Actor.h"

namespace BallNS{
	const float SPEED = 10;
	const float MAX_SPEED = 20;
};

class Ball: public virtual Actor{
public:
	Ball(){blockCombo = 0;desiredSpeed=BallNS::SPEED;}
	void hitBlock(Actor* a);
	void hitWall(Actor* a);
	void hitPaddle(Actor* a);
	void update(float dt);
	void setDirection(Vector3 d){Actor::setVelocity(d);}
	void create(Vector3 pos){desiredSpeed=BallNS::SPEED;Actor::create(pos);}
private:
	Vector3 prevLoc;
	int blockCombo;
	float desiredSpeed;

	void setVelocity(Vector3 v){Actor::setVelocity(v);}
};