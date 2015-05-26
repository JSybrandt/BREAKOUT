#pragma once

#include "d3dApp.h"
#include "Geometry.h"
#include "Actor.h"
#include "input.h"
#include <d3dx9math.h>
#include "Camera.h"
#include "Light.h"
#include "sharedDefines.h"
#include "GameState.h"
#include "Sky.h"
#include "GUI.h"
#include "Sprite.h"

#include "Paddle.h"
#include "Ball.h"
#include "Powerup.h"
#include "Block.h"


using std::wstring;

namespace BRK{
	const int NUM_MENU_ITEMS = 6;//title, play, quit, "", instructions 1/2
	const int NUM_SPLASH_MENU_ITEMS = 3;//title, continue, quit	

	const int NUM_BLOCKS = 100;
	

	const int NUM_WALLS = 3;
	const int NUM_POWER_UPS = 100;
	const int NUM_BALLS = 100;
	const int NUM_SCENERY = 1000;

	const Vector3 PADDLE_START_POS(16,0.25,0);
	const Vector3 BALL_START_POS(16,3,0);
	const Vector3 LOOK_AT_DISP(0,10,0);

	const float LOWEST_POINT = -5;
	
	const float WIDTH = 32;
};




class BREAKOUT : public D3DApp{

public:
	BREAKOUT(HINSTANCE hInstance);
	~BREAKOUT();

	void initApp();
	void threadInit();
	
	void initBackground();
	void onResize();
	void updateScene(float dt);
	void drawScene(); 
	void collisions();

	void menuLoad();
	void menuUpdate(float dt,bool reset = false);
	void menuDraw();

	void loadSplashScreen(bool status);
	void splashDraw();

	void clearLevel();
	void levelsUpdate(float dt);
	void levelsDraw();

	void resetLevel();
	void loadLevel();

	GameState state;

	Controls controls;

	void onPlayerDeath();

	ID3D10Device* getDevice(){return md3dDevice;}

	Camera camera;

	Sky sky;

	

	bool waiting;
	
	Light* spawnLight(Vector3 pos, int type = 0);
	Light* spawnLight(Vector3 pos, Vector3 dir, int type = 0);

	Powerup* spawnPowerUp(Vector3 pos, PowerUpType t);
	Ball* spawnBall(Vector3 pos, Vector3 vel);
	Actor* spawnScenery(Vector3 pos, Vector3 vel, Geometry* geo, TEXTURE* tex);

	//TEXTURE RELATED ITEMS
	ID3D10ShaderResourceView* mDiffuseMapRV;
	ID3D10ShaderResourceView* mSpecMapRV;

	ID3D10EffectShaderResourceVariable* mfxDiffuseMapVar;
	ID3D10EffectShaderResourceVariable* mfxSpecMapVar;
	ID3D10EffectMatrixVariable* mfxTexMtxVar;
	ID3D10Effect* skyFX;

	GUI gui;
	Sprite sprite;
	Sprite trees;
	
	Light fLight;
	Light ambient;
	Light pLight;
	Light negaLight;
	Light rLights[MAX_LIGHTS];		//Room Lights, point lights
	Light eyes;
	int activeLights;
	int lightType[MAX_LIGHTS];

	ID3D10EffectMatrixVariable* mfxWVPVar;
	ID3D10EffectMatrixVariable* mfxWorldVar;
	ID3D10EffectVariable* mfxEyePosVar;
	ID3D10EffectVariable* mfxLightVar;
	ID3D10EffectVariable* mfxPLightsVar[MAX_LIGHTS];
	ID3D10EffectVariable* mfxPLightVar;
	ID3D10EffectVariable* mfxNegaLightVar;
	ID3D10EffectVariable* mfxEyesVar;
	ID3D10EffectVariable* mfxAmbientVar;
	
	ID3D10EffectScalarVariable* mfxLightBool;
	ID3D10EffectScalarVariable* mfxHighlightBool;
	ID3D10EffectScalarVariable* mfxActiveLights;
	ID3D10EffectScalarVariable* mfxLightType[MAX_LIGHTS];

	Geometry mCube, mSphere, mPaddle,mPlayerShip,mEnemyShip,mPowerUp,mShard;

private:
	void buildFX();
	void buildVertexLayouts();

	wstring loadingStatus;
	HANDLE loadingThread;
	bool threadComplete;

	TEXTURE* tBlue,*tGreen,*tOrange,*tPurple,*tYellow,*tRock,*tWhite,*tPowFlip,*tPowGrow,*tPowShrink,*tPowSpace,*tPowSplit, *tSpec, *tNonSpec,*tPaddle, *tEnemy;
	Block blocks[BRK::NUM_BLOCKS];
	Actor walls[BRK::NUM_WALLS];
	Powerup powerUps[BRK::NUM_POWER_UPS]; 
	Paddle paddle;
	Ball balls[BRK::NUM_BALLS];
	Actor enemyShip;
	Actor scenery[BRK::NUM_SCENERY];

	void loadWalls();

	void checkGameState();

	void doPowerUp(PowerUpType t);

	float flipCooldown;
	Vector3 cameraPos;

	bool hasStarted;
	Ball* startingBall;

protected:

	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10InputLayout* mVertexLayout;


	D3DXMATRIX mView;
	D3DXMATRIX mProj;


	std::wstring menuText[BRK::NUM_MENU_ITEMS];
	int menuChoice;


};