#include "Paddle.h"

void Paddle::update(float dt){

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

	Actor::update(dt);
}
