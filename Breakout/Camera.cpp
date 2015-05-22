#include "Camera.h"
#include "CanisMajor.h"

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
	direction = Vector3(0.0f, 0.0f, 0.0f);
}

Camera::~Camera()
{
	
}





void Camera::update(float dt)
{
	Vector3 forward = direction;
	forward.y=0;

	Vector3 right;

	D3DXVec3Cross(&right,&up,&forward);

	Normalize(&right,&right);
	Normalize(&forward,&forward);

	Normalize(&direction,&direction);

	Vector3 lookAt = position + direction;

	//game->audio->updateCamera(getPosition(),getDirection(),up,Vector3(0,0,0));
	
	//Generate new matrix
	D3DXMatrixLookAtLH(&mView, &position, &lookAt, &up);
}
