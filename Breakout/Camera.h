#ifndef _CAMERA_H                // Prevent multiple definitions if this 
#define _CAMERA_H 

#include "constants.h"
#include "d3dUtil.h"
#include "d3dApp.h"
#include <d3dx9math.h>
#include "input.h"
#include "Light.h"
#include "Actor.h"
#include "Controls.h"
#include "mathUtil.h"

class BREAKOUT;

namespace CameraNS
{
	const float NEAR_CLIPPING_DIST = 0.1f;
	const float FAR_CLIPPING_DIST = 100;//originally 1000
};

class Camera
{
public:
	Camera();
	~Camera();

	void update(float dt);

	Matrix getViewMatrix() {return mView;}
	Matrix getProjectionMatrix() {return mProj;}
	
	void Camera::setPerspective();

	void setPosition(Vector3 pos) {position = pos;}
	Vector3 getPosition() {return position;}
	void setLookAt(Vector3 l){if(!uninit){uninit=true;currentLookAt=l;}else desiredLookAt = l;}
	Vector3 getLookAt(){return currentLookAt;}
	float getSpeed () {return speed;}
	void setSpeed(float s) {speed = s;}
	float getFoV() {return FoV;}
	void setFoV(float fov){FoV = fov;}

	void setGame(BREAKOUT* g){game=g;}

private:

	BREAKOUT * game;
	Matrix mView;
	Matrix mProj;
	float speed;
	float aspectRatio;
	float FoV;
	float nearClippingPlane;
	float farClippingPlane;
	Vector3 up;
	Vector3 right;
	Vector3 position;
	Vector3 currentLookAt;
	Vector3 desiredLookAt;
	bool uninit;
};
#endif