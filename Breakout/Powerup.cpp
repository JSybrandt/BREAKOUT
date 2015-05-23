#include "Powerup.h"
#include "BREAKOUT.h"

void Powerup::update(float dt)
{
	if(isActive){
		Actor::update(dt);
		if(getPosition().y < BRK::LOWEST_POINT)
			isActive = false;
	}
}

void Powerup::create(Vector3 pos, PowerUpType t)
{
	Actor::create(pos);
	setVelocity(Vector3(0,0,0));
	type=t;
}

void Powerup::drop()
{
	setVelocity(PowerUpNS::VELOCITY);
}