#include "Block.h"
#include "BREAKOUT.h"


void Block::create(Vector3 pos, Vector3 scale){
		Actor::create(pos, Vector3(0,0,0),scale);
		if(rand01()<BlockNS::POWER_UP_CHANCE)
			pow = (PowerUpType)(rand()%PowerUpType::COUNT);
		else
			pow = NONE;

	}

void Block::isHit(){
	isActive = false;
	if(pow!=NONE)
		game->spawnPowerUp(getPosition(), pow);
}