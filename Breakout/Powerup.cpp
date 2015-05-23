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
	type=t;
	Actor::setVelocity(PowerUpNS::VELOCITY);
}