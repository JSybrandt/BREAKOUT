#include "Paddle.h"
#include "BREAKOUT.h"

void Paddle::init(BREAKOUT* game,Geometry *b,Geometry *s)
{
	Actor::init(game,b,b->mDiffuseMapRV,1,PADDLE_SCALE);
	base = b;
	space = s;
}

void Paddle::update(float dt){

	//if we will be ending the space
	if(spaceInvaderCooldown>0 && spaceInvaderCooldown-dt <=0)
	{
		this->setScale(PaddleNS::PADDLE_SCALE);
		this->setGeometry(base);
		setRotation(Vector3(0,0,0));
	}

	spaceInvaderCooldown=max(0,spaceInvaderCooldown-dt);

	sizeCooldown=max(0,sizeCooldown-dt);
	if(sizeCooldown==0)desiredWidth = PaddleNS::PADDLE_SCALE.x;

	fireCooldown=max(0,fireCooldown-dt);

	Vector3 input(0,0,0);
	if(GetAsyncKeyState('A') || GetAsyncKeyState(VK_LEFT))
	{
		input.x -=1;
	}
	if(GetAsyncKeyState('D') || GetAsyncKeyState(VK_RIGHT))
	{
		input.x +=1;
	}
	position += input*PaddleNS::SPEED * dt;

	if(position.x - scale.x/2 < 1)
		position.x = 1 + scale.x/2;
	if(position.x + scale.x/2 > 31)
		position.x = 31 - scale.x/2;

	if(abs(scale.x - desiredWidth) > 0.001)
	{
		float sign = desiredWidth-scale.x;
		sign/=abs(sign);

		scale.x += dt*sign*GROWTH_SPEED;

	}
	else
		scale.x = desiredWidth;

	if(spaceInvaderCooldown>0)
	{
		this->setGeometry(space);
		this->setScale(SHIP_SCALE);

		//rotation.y=PI/2;
		rotation.z = max(-PI/4,min(PI/4,rotation.z+-dt*PI*input.x));

		if(abs(rotation.z)>0.001)
			rotation.z += -rotation.z/abs(rotation.z)*dt*PI/3;
		else
			rotation.z=0;

		if(fireCooldown==0&&GetAsyncKeyState(VK_SPACE)){
			Vector3 pos(0,1,0);
			pos = rotateZ(pos,rotation.z);
			pos+=getPosition();
			Vector3 vel = rotateZ(Vector3(0,BallNS::SPEED,0),rotation.z);
			game->spawnBall(pos,vel);
			fireCooldown = FIRE_RATE;
		}
	}

	Actor::update(dt);
}
void Paddle::enterSpaceMode()
{
	spaceInvaderCooldown = PowerUpNS::SPACE_DURATION;
	this->setGeometry(space);
}
