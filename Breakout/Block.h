#pragma once
#include "Actor.h"
#include "Powerup.h"
#include "mathUtil.h"

namespace BlockNS{
const Vector3 BLOCK_SCALE(2,1,1);
const float POWER_UP_CHANCE = 0.1;
};

class Block: public virtual Actor{
public:
	void create(Vector3 pos, Vector3 scale = BlockNS::BLOCK_SCALE);

	void isHit();

private:
	PowerUpType pow; 
};