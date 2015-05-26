#include "Ball.h"
#include "BREAKOUT.h"

void Ball::hitBlock(Actor* a)
{	
	Vector3 diff = getPosition()-a->getPosition();
	//normalize for block size
	diff.x/=a->getScale().x;
	diff.y/=a->getScale().y;
	Normalize(&diff,&diff);
	Vector3 vel = getVelocity();
	if(abs(diff.x) > abs(diff.y))//hit left or right side
	{
		vel.x*=-1;
	}
	else//hit top or bottom
	{
		vel.y*=-1;
	}
	setVelocity(vel);
	desiredSpeed = min(desiredSpeed+2,BallNS::MAX_SPEED);
	setPosition(prevLoc);
	blockCombo++;
	
}
void Ball::hitPaddle(Actor* a)
{	
	Vector3 diff = getPosition()-a->getPosition()+Vector3(0,1,0);
	Normalize(&diff,&diff);
	setVelocity(diff);
	desiredSpeed = BallNS::SPEED;
	setPosition(prevLoc);
	position.y = a->getPosition().y+a->getScale().y/2 + this->getScale().y/2;
	
	blockCombo=0;
}
void Ball::hitWall(Actor* a)
{	
	Vector3 diff = getPosition()-a->getPosition();
	//normalize for block size
	diff.x/=a->getScale().x;
	diff.y/=a->getScale().y;
	Normalize(&diff,&diff);
	Vector3 vel = getVelocity();
	if(abs(diff.x) > abs(diff.y))//hit left or right side
	{
		vel.x*=-1;
	}
	else//hit top or bottom
	{
		vel.y*=-1;
	}
	setVelocity(vel);
	setPosition(prevLoc);
}
void Ball::update(float dt){
	if(isActive){
		prevLoc = getPosition();
		Vector3 t = velocity;
		velocity*=desiredSpeed;
		Actor::update(dt);
		velocity=t;
		if(getPosition().y < BRK::LOWEST_POINT)
			isActive = false;
	}
}