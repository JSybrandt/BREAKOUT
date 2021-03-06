#include "Camera.h"
#include "BREAKOUT.h"

using namespace CameraNS;

#define Transform D3DXVec3TransformCoord
Camera::Camera()
{
	speed = 10;
	FoV = 0.5*PI;
	aspectRatio = 480.0f/640.0f;
	nearClippingPlane = NEAR_CLIPPING_DIST;
	farClippingPlane = FAR_CLIPPING_DIST;
	up = Vector3(0.0f, 1.0f, 0.0f);
	position = Vector3(0,0,0);
	desiredLookAt =currentLookAt= Vector3(0,0,0);
	uninit = false;
}

Camera::~Camera()
{
	
}

void Camera::setPerspective()
{
	aspectRatio = (float)game->mClientWidth/game->mClientHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, FoV, aspectRatio, nearClippingPlane,farClippingPlane); 
}



void Camera::update(float dt)
{
	setPerspective();

	Vector3 diff = desiredLookAt - currentLookAt;
	if(Length(&diff)>0.05)
	{
		Normalize(&diff,&diff);
		currentLookAt += diff*dt*4.5;
	}
	else
		currentLookAt = desiredLookAt;

	Vector3 forward = position - currentLookAt;
	forward.y=0;

	Vector3 right;

	D3DXVec3Cross(&right,&up,&forward);

	Normalize(&right,&right);
	Normalize(&forward,&forward);

	//game->audio->updateCamera(getPosition(),getDirection(),up,Vector3(0,0,0));
	
	//Generate new matrix
	D3DXMatrixLookAtLH(&mView, &position, &currentLookAt, &up);
}
