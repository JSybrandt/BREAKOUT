#include "Block.h"
#include "BREAKOUT.h"


void Block::create(Vector3 pos, Vector3 scale){
		Actor::create(pos, Vector3(0,0,0),scale);
		pow = nullptr;
		if(rand01()<BlockNS::POWER_UP_CHANCE){
			pow = game->spawnPowerUp(getPosition()+Vector3(0,0,0),(PowerUpType)(rand()%PowerUpType::COUNT));
		}

	}

void Block::isHit(){
	isActive = false;
	if(pow!=nullptr)
	{
		pow->drop();
		pow=nullptr;
	}
}