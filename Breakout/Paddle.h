#pragma once
#include "Actor.h"
#include "Powerup.h"

namespace PaddleNS{
	const float SPEED = 15;
	const Vector3 PADDLE_SCALE(3,1,1);
	const Vector3 SHIP_SCALE(4,4,4);
	const float GROWTH_AMMT = 1.5;
	const float GROWTH_SPEED = 1.5;
	const float MAX_WIDTH = 6;
	const float MIN_WIDTH = 1.5;
	const float FIRE_RATE = 0.5;
};
using namespace PaddleNS;
class Paddle: public virtual Actor{
public:
	Paddle(){reset();}
	void init(BREAKOUT* game,Geometry *base,Geometry *space);
	void create(Vector3 pos){Actor::create(pos);reset();}
	void update(float dt);
	void grow(){desiredWidth+=GROWTH_AMMT;if(desiredWidth>MAX_WIDTH)desiredWidth=MAX_WIDTH;sizeCooldown=PowerUpNS::SIZE_DURATION;}
	void shrink(){desiredWidth-=GROWTH_AMMT; if(desiredWidth<MIN_WIDTH)desiredWidth=MIN_WIDTH;sizeCooldown=PowerUpNS::SIZE_DURATION;}
	void enterSpaceMode();
	bool isShip(){return spaceInvaderCooldown>0;}
	void reset(){desiredWidth=PaddleNS::PADDLE_SCALE.x;spaceInvaderCooldown=0;sizeCooldown=0;fireCooldown=0;setGeometry(base);}
private:
	float desiredWidth;
	float sizeCooldown;
	float spaceInvaderCooldown, fireCooldown;
	Geometry *base, *space;
};