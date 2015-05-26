#include"BREAKOUT.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif


	BREAKOUT theApp(hInstance);

	theApp.initApp();

	return theApp.run();
}



BREAKOUT::BREAKOUT(HINSTANCE hInstance)
	: D3DApp(hInstance), mFX(0), mTech(0), mVertexLayout(0),
	mfxWVPVar(0), mfxWorldVar(0),  mfxEyePosVar(0), 
	mfxLightVar(0), mfxLightBool(0), mfxDiffuseMapVar(0),
	mfxSpecMapVar(0), mfxTexMtxVar(0)
{
	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProj);
	activeLights = 0;
	flipCooldown = 0;
}

BREAKOUT::~BREAKOUT()
{
	if( md3dDevice )
		md3dDevice->ClearState();


	ReleaseCOM(mFX);
	ReleaseCOM(mVertexLayout);

}

DWORD WINAPI threadFunct(LPVOID lpParameter)
{
	static_cast<BREAKOUT*>(lpParameter)->threadInit();
	return 0;
}

void BREAKOUT::initApp()
{

	D3DApp::initApp();

	buildFX();

	threadComplete = false;
	loadingThread = CreateThread(0,0,threadFunct,this,0,0);

	buildVertexLayouts();
	menuLoad();

}

void BREAKOUT::threadInit()
{

	for(int i = 0; i < MAX_LIGHTS; i++) {
		rLights[i].init();
		rLights[i].pos = Vector3(0, -200, 0);
		lightType[i] = 0;
	}

	sky.init(md3dDevice, this, 5000.0f);

	loadingStatus = L"Creating Textures";
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, L".\\textures\\blue.png", 0, 0, &tBlue, 0 ));
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, L".\\textures\\green.png", 0, 0, &tGreen, 0 ));
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, L".\\textures\\orange.png", 0, 0, &tOrange, 0 ));
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, L".\\textures\\purple.png", 0, 0, &tPurple, 0 ));
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, L".\\textures\\yellow.png", 0, 0, &tYellow, 0 ));
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, L".\\textures\\rock.png", 0, 0, &tRock, 0 ));
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, L".\\textures\\white.png", 0, 0, &tWhite, 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, L".\\textures\\white.png", 0, 0, &tSpec, 0 ));
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, L".\\textures\\defaultspec.dds", 0, 0, &tNonSpec, 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, L".\\textures\\pFlip.png", 0, 0, &tPowFlip, 0 ));
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, L".\\textures\\pGrow.png", 0, 0, &tPowGrow, 0 ));
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, L".\\textures\\pShrink.png", 0, 0, &tPowShrink, 0 ));
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, L".\\textures\\pSpace.png", 0, 0, &tPowSpace, 0 ));
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, L".\\textures\\pSplit.png", 0, 0, &tPowSplit, 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, L".\\textures\\paddle.png", 0, 0, &tPaddle, 0 ));
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, L".\\textures\\enemy.png", 0, 0, &tEnemy, 0 ));

	loadingStatus = L"Creating Models";
	mCube.init(md3dDevice,".\\geometry\\cube.geo",tWhite,tNonSpec);
	mPowerUp.init(md3dDevice,".\\geometry\\powerUp.geo",tPowFlip,tSpec);
	mPaddle.init(md3dDevice,".\\geometry\\paddle.geo",tPaddle,tSpec);
	mPlayerShip.init(md3dDevice,".\\geometry\\playerShip.geo",tPaddle,tSpec);
	mEnemyShip.init(md3dDevice,".\\geometry\\enemyShip.geo",tEnemy,tSpec);
	mSphere.init(md3dDevice,".\\geometry\\sphere.geo",tWhite,tSpec);
	mShard.init(md3dDevice,".\\geometry\\shard.geo",tWhite,tSpec);

	loadingStatus = L"Instantiating Objects";
	TEXTURE* b[] = {tBlue,tGreen,tOrange,tPurple,tYellow};
	for(int i=0; i<BRK::NUM_BLOCKS;i++)
	{
		blocks[i].init(this,&mCube,b[i/15]);
		blocks[i].collisionType = AABBox;
	}
	for(int i = 0 ; i < BRK::NUM_WALLS; i++)
	{
		walls[i].init(this,&mCube,tRock);
		walls[i].collisionType = AABBox;
	}
	for(int i = 0; i < BRK::NUM_POWER_UPS; i++)
	{
		powerUps[i].init(this,&mPowerUp,tPowFlip,2,Vector3(1,1,1));
	}
	paddle.init(this,&mPaddle,&mPlayerShip);
	paddle.collisionType = AABBox;
	paddle.setPosition(BRK::PADDLE_START_POS);
	enemyShip.init(this,&mEnemyShip,tEnemy, 2);
	for(int i=0;i<BRK::NUM_BALLS;i++)
		balls[i].init(this,&mSphere,tWhite,0.5);
	for(int i=0;i<BRK::NUM_SCENERY;i++)
		scenery[i].init(this,&mShard,tWhite);
	
	loadingStatus = L"Finishing";
	camera.setGame(this);
	buildVertexLayouts();
	threadComplete = true;
}

void BREAKOUT::onResize()
{
	D3DApp::onResize();
}

void BREAKOUT::updateScene(float dt)
{
	if(!threadComplete)
		return;
	D3DApp::updateScene(dt);
	if(GetAsyncKeyState(VK_ESCAPE))
		PostQuitMessage(0);
	switch(state.level){
	case MENU:
	case SPLASH:
		menuUpdate(dt);
		break;
	default:
		levelsUpdate(dt);
		break;
	}
}


void BREAKOUT::menuUpdate(float dt, bool reset)
{
	if(!threadComplete)
		return;

	static bool isKeyDown = true;

	if(reset)
	{
		menuChoice = 1;
		isKeyDown = true;
	}

	if(GetAsyncKeyState(VK_RETURN)||GetAsyncKeyState(' '))
	{ 
		if(!isKeyDown)
		{
			switch(menuChoice)
			{
			case 1://play
				if(state.level==SPLASH)
					menuLoad();
				else {
					audio->playCue(BG);
					loadLevel();
				}
				break;
			case 2://quit
				PostQuitMessage(0);
				break;
			}
		}
	}
	else if(GetAsyncKeyState('W')||GetAsyncKeyState(VK_UP))
	{
		if(!isKeyDown)
		{
			menuChoice--;
			isKeyDown = true;
		}
	}
	else if(GetAsyncKeyState('S')||GetAsyncKeyState(VK_DOWN))
	{
		if(!isKeyDown)
		{
			menuChoice++;
			isKeyDown = true;
		}
	}
	else
		isKeyDown = false;

	if(menuChoice > 2) menuChoice = 1;
	if(menuChoice < 1) menuChoice = 2;
}

void BREAKOUT::levelsUpdate(float dt)
{	

	if(!hasStarted)
	{
		if(GetAsyncKeyState(VK_SPACE))
		{
			hasStarted=true;
			startingBall->setDirection(Vector3(0,1,0));
		}
		
	}

	checkGameState();

	flipCooldown = max(0,flipCooldown-dt);
	if(flipCooldown>0)
	{
		float x = (PowerUpNS::FLIP_DURATION-flipCooldown)/PowerUpNS::FLIP_DURATION;
		float percentChange = (-pow(2*x-1,20)+1);
		float rot = PI*percentChange;
		Vector3 newPos = cameraPos - camera.getLookAt();
		newPos = rotateY(newPos,rot);
		newPos += camera.getLookAt();
		camera.setPosition(newPos);
	}
	camera.update(dt);
	for(int i = 0 ; i < BRK::NUM_BLOCKS; i++)
		blocks[i].update(dt);
	for(int i = 0 ; i < BRK::NUM_WALLS; i++)
		walls[i].update(dt);

	paddle.update(dt);
	Vector3 averagePos = (paddle.getPosition()+Vector3(0,7,0))*4;
	int count = 4;
	for(int i=0;i<BRK::NUM_BALLS;i++)
	{
		balls[i].update(dt);
		if(balls[i].isActive)
		{
			count++;
			averagePos+=balls[i].getPosition();
		}
	}
	camera.setLookAt(averagePos/count);
	//camera.setLookAt(paddle.getPosition() + BRK::LOOK_AT_DISP);
	cameraPos.x = ((paddle.getPosition().x/BRK::WIDTH)-0.5)*BRK::WIDTH/2+BRK::WIDTH/2; 
	camera.setPosition(cameraPos);
	collisions();

	Vector3 rot = enemyShip.getRotation();
	rot.y += PI*dt;
	if(rot.y>2*PI)rot.y-=2*PI;
	rot.z=sin(rot.y/2)/2;
	enemyShip.setRotation(rot);
	enemyShip.update(dt);
	if(enemyShip.getPosition().x<-40)enemyShip.isActive=false;
	for(int i = 0; i < BRK::NUM_POWER_UPS; i++)
		powerUps[i].update(dt);
	for(int i=0;i<BRK::NUM_SCENERY;i++)
	{
		if(scenery[i].isActive)
		{
			scenery[i].setRotation(scenery[i].getRotation()+Vector3(1,2,0)*dt);
			scenery[i].update(dt);
			if(Length(&scenery[i].getPosition())>CameraNS::FAR_CLIPPING_DIST)
				scenery[i].isActive = false;
		}
	}
}

//COLLISIONS GIVE LOADS OF FALSE POSITIVES
void BREAKOUT::collisions()
{
	
	for(int i = 0; i < BRK::NUM_POWER_UPS; i++)
		if(powerUps[i].collided(&paddle)){
			powerUps[i].isActive = false;
			doPowerUp(powerUps[i].type);
		}

	for(int j=0;j<BRK::NUM_BALLS;j++)
	{
		if(balls[j].collided(&enemyShip)){
			balls[j].hitWall(&enemyShip);
			enemyShip.isActive=false;
		}
		if(!paddle.isShip() && balls[j].collided(&paddle)){
			balls[j].hitPaddle(&paddle);
		}
		for(int i = 0 ; i < BRK::NUM_WALLS; i++)
		{
			if(balls[j].collided(&walls[i]))
			{
				balls[j].hitWall(&walls[i]);
			}
		}
		for(int i = 0 ; i < BRK::NUM_BLOCKS; i++)
		{
			if(balls[j].collided(&blocks[i]))
			{
				blocks[i].isHit();
				balls[j].hitBlock(&blocks[i]);
			}
		}
	}
	
}

void BREAKOUT::drawScene()
{
	D3DApp::drawScene();

	// Restore default states, input layout and primitive topology 
	// because mFont->DrawText changes them.  Note that we can 
	// restore the default states by passing null.
	md3dDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
	md3dDevice->IASetInputLayout(mVertexLayout);

	mfxEyePosVar->SetRawValue(&camera.getPosition(), 0, sizeof(D3DXVECTOR3));
	mfxEyesVar->SetRawValue(&eyes, 0, sizeof(Light));
	mfxLightVar->SetRawValue(&fLight, 0, sizeof(Light));
	mfxAmbientVar->SetRawValue(&ambient, 0, sizeof(Light));
	for(int i = 0; i < MAX_LIGHTS; i++) { //Individually setting lights.
		mfxPLightsVar[i]->SetRawValue(&rLights[i], 0, sizeof(Light));
	}
	for(int i = 0; i < MAX_LIGHTS; i++) { //Individually setting lights.
		mfxLightType[i]->SetInt(lightType[i]);
	}
	mfxActiveLights->SetInt(activeLights);

	//mfxPLightsVar->SetRawValue(&rLights, 0, sizeof(Light));
	mfxPLightVar->SetRawValue(&pLight, 0, sizeof(Light));
	mfxNegaLightVar->SetRawValue(&negaLight, 0, sizeof(Light));
	mfxLightBool->SetBool(false);

	// set the point to the shader technique
	D3D10_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	if(!threadComplete)
	{
		RECT r; //its a point because DT_NOCLIP
		r.right = r.left = mClientWidth*0.5;
		r.top = r.bottom = mClientHeight*0.2;
		mFont->DrawText(0,L"LOADING",-1,&r,DT_NOCLIP|DT_CENTER,WHITE);
		r.top = r.bottom = mClientHeight*0.3;
		mFont->DrawText(0,loadingStatus.c_str(),-1,&r,DT_NOCLIP|DT_CENTER,RED);
	}
	else
	{
		switch(state.level){
		case MENU:
			menuDraw();
			break;
		case SPLASH:
			splashDraw();
			break;
		default:
			levelsDraw();
			break;
		}
	}
	mSwapChain->Present(1, 0);
}

void BREAKOUT::splashDraw()
{
	for(int i = 0; i < BRK::NUM_SPLASH_MENU_ITEMS; i++)
	{
		RECT r; //its a point because DT_NOCLIP
		if(i==0)
		{
			r.right = r.left = mClientWidth*0.5;
			r.top = r.bottom = mClientHeight*0.2;

		}
		else
		{
			r.right = r.left = mClientWidth*0.5;
			r.top = r.bottom = mClientHeight*0.4 + (i-1)*mClientHeight*0.07;
		}
		if(menuChoice == i)
			mFont->DrawText(0,menuText[i].c_str(),-1,&r,DT_NOCLIP|DT_CENTER,RED);
		else
			mFont->DrawText(0,menuText[i].c_str(),-1,&r,DT_NOCLIP|DT_CENTER,WHITE);
	}
}

void BREAKOUT::menuDraw()
{
	for(int i = 0; i < BRK::NUM_MENU_ITEMS; i++)
	{
		RECT r; //its a point because DT_NOCLIP
		if(i==0)
		{
			r.right = r.left = mClientWidth*0.5;
			r.top = r.bottom = mClientHeight*0.2;

		}
		else
		{
			r.right = r.left = mClientWidth*0.5;
			r.top = r.bottom = mClientHeight*0.4 + (i-1)*mClientHeight*0.07;
		}
		if(menuChoice == i)
			mFont->DrawText(0,menuText[i].c_str(),-1,&r,DT_NOCLIP|DT_CENTER,RED);
		else
			mFont->DrawText(0,menuText[i].c_str(),-1,&r,DT_NOCLIP|DT_CENTER,WHITE);
	}
}

void BREAKOUT::levelsDraw()
{
	//Get Camera viewMatrix
	mView = camera.getViewMatrix();
	mProj = camera.getProjectionMatrix();

	//Billboards screw with these settings, gotta reset them.
	md3dDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
	md3dDevice->IASetInputLayout(mVertexLayout);

	sky.draw(mView, mProj);

	for(int i = 0 ; i < BRK::NUM_BLOCKS; i++)
		blocks[i].draw(mfxWVPVar,mView,mProj,mTech);
	for(int i = 0 ; i < BRK::NUM_WALLS; i++)
		walls[i].draw(mfxWVPVar,mView,mProj,mTech);
	for(int i = 0; i < BRK::NUM_POWER_UPS; i++)
		powerUps[i].draw(mfxWVPVar,mView,mProj,mTech);
	paddle.draw(mfxWVPVar,mView,mProj,mTech);
	enemyShip.draw(mfxWVPVar,mView,mProj,mTech);
	for(int i=0;i<BRK::NUM_BALLS;i++)
		balls[i].draw(mfxWVPVar,mView,mProj,mTech);
	for(int i=0;i<BRK::NUM_SCENERY;i++)
		scenery[i].draw(mfxWVPVar,mView,mProj,mTech);
}

void BREAKOUT::buildFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	hr = D3DX10CreateEffectFromFile(L"lighting.fx", 0, 0, 
		"fx_4_0", shaderFlags, 0, md3dDevice, 0, 0, &mFX, &compilationErrors, 0);
	if(FAILED(hr))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			ReleaseCOM(compilationErrors);
		}
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX10CreateEffectFromFile", true);
	} 

	hr = D3DX10CreateEffectFromFile(L"sky.fx", 0, 0, 
		"fx_4_0", shaderFlags, 0, md3dDevice, 0, 0, &skyFX, &compilationErrors, 0);
	if(FAILED(hr))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			ReleaseCOM(compilationErrors);
		}
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX10CreateEffectFromFile", true);
	} 

	mTech = mFX->GetTechniqueByName("LightTech");

	mfxWVPVar    = mFX->GetVariableByName("gWVP")->AsMatrix();
	mfxWorldVar  = mFX->GetVariableByName("gWorld")->AsMatrix();
	mfxEyePosVar = mFX->GetVariableByName("gEyePosW");
	mfxLightVar  = mFX->GetVariableByName("gLight");

	for(int i = 0; i < MAX_LIGHTS; i++) {
		mfxPLightsVar[i] = mFX->GetVariableByName("lights")->GetElement(i);
	}
	for(int i = 0; i < MAX_LIGHTS; i++) {
		mfxLightType[i] = mFX->GetVariableByName("type")->GetElement(i)->AsScalar();
	}

	mfxActiveLights = mFX->GetVariableByName("activeLights")->AsScalar();
	mfxPLightVar = mFX->GetVariableByName("pLight");
	mfxNegaLightVar = mFX->GetVariableByName("negaLight");
	mfxEyesVar = mFX->GetVariableByName("eyes");
	mfxAmbientVar = mFX->GetVariableByName("ambient");
	mfxLightBool = mFX->GetVariableByName("gLightType")->AsScalar();
	mfxHighlightBool = mFX->GetVariableByName("highlight")->AsScalar();

	//TEXTURES
	mfxDiffuseMapVar = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecMapVar    = mFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxTexMtxVar     = mFX->GetVariableByName("gTexMtx")->AsMatrix();
}

void BREAKOUT::buildVertexLayouts()
{
	// Create the vertex input layout.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24,
		D3D10_INPUT_PER_VERTEX_DATA, 0},
	};


	// Create the input layout
	D3D10_PASS_DESC PassDesc;
	mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
	HR(md3dDevice->CreateInputLayout(vertexDesc, 3, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &mVertexLayout));
}

void BREAKOUT::clearLevel()
{
	for(int i = 0 ; i < BRK::NUM_BLOCKS; i++)
		blocks[i].isActive = false;
	for(int i = 0 ; i < BRK::NUM_WALLS; i++)
		walls[i].isActive = false;
	for(int i = 0; i < BRK::NUM_POWER_UPS; i++)
		powerUps[i].isActive = false;
	paddle.isActive = false;
	enemyShip.isActive=false;
	for(int i=0;i<BRK::NUM_BALLS;i++)
		balls[i].isActive = false;
	for(int i=0;i<BRK::NUM_SCENERY;i++)
		scenery[i].isActive = false;
	flipCooldown = 0;
	hasStarted=false;
}

void BREAKOUT::loadSplashScreen(bool status)
{
	state.level = SPLASH;
	clearLevel();
	menuUpdate(0,true);

	if(status)
		menuText[0] = L"VICTORY";
	else
		menuText[0] = L"DEFEAT";
	menuText[1] = L"CONTINUE";
	menuText[2] = L"QUIT";
}

void BREAKOUT::menuLoad()
{
	audio->stopCue(BG);
	state.level = MENU;
	clearLevel();
	menuUpdate(0,true);


	menuText[0] = L"SUPER BREAKOUT";
	menuText[1] = L"PLAY";
	menuText[2] = L"QUIT";
	menuText[3] = L"";
	menuText[4] = L"<-> Move Paddle";
	menuText[5] = L"Don't Miss the Ball!";
}



void BREAKOUT::onPlayerDeath()
{
}


void BREAKOUT::loadLevel()
{
	state.level = GAME;
	loadWalls();
	paddle.create(BRK::PADDLE_START_POS);
	startingBall = spawnBall(BRK::BALL_START_POS, Vector3(0,0,0));
	for(int i = 0 ; i < 75; i++)
	{
		blocks[i].create(Vector3((i%15)*2 + 2,(i/15)*1 + 12,0),BlockNS::BLOCK_SCALE);
	}
	Light* l = spawnLight(Vector3(0,0,100));
	l->range=1000;
	l->att = Vector3(1,0,0);
	l->ambient = D3DXCOLOR(1,1,1,1);

	camera.setPosition(Vector3(16,7,-15));
	cameraPos = Vector3(16,7,-15);
	camera.setLookAt(Vector3(16,7,0));
}

void BREAKOUT::loadWalls(){
	walls[0].create(Vector3(16,21,0),Vector3(0,0,0),Vector3(32,1,1));
	walls[1].create(Vector3(.5,10.25,0),Vector3(0,0,0),Vector3(1,21,1));
	walls[2].create(Vector3(31.5,10.25,0),Vector3(0,0,0),Vector3(1,21,1));
};

Light* BREAKOUT::spawnLight(Vector3 pos, int type) {

	if(activeLights < MAX_LIGHTS) {
		lightType[activeLights] = rLights[activeLights].init(type);
		rLights[activeLights].pos = pos;
		activeLights++;
		return &rLights[activeLights-1];
	}

	return nullptr;
}

Light* BREAKOUT::spawnLight(Vector3 pos, Vector3 dir, int type) { 
	if(activeLights < MAX_LIGHTS) {
		lightType[activeLights] = rLights[activeLights].init(type);
		rLights[activeLights].pos = pos;
		rLights[activeLights].dir = dir;
		activeLights++;
		return &rLights[activeLights-1];
	}

	return nullptr;
}

void BREAKOUT::checkGameState()
{
	bool won = true;
	for(int i = 0 ; i < BRK::NUM_BLOCKS; i++)
		if(blocks[i].isActive){
			won=false;
			break;
		}
	if(won)
	{
		loadSplashScreen(true);
		return;
	}

	if(!paddle.isShip()){
		bool lost = true;
		for(int i = 0 ; i < BRK::NUM_BALLS; i++)
			if(balls[i].isActive){
				lost = false;
				break;
			}
		if(lost)
		{
			loadSplashScreen(false);
			return;
		}
	}
}

void BREAKOUT::doPowerUp(PowerUpType t)
{
	if(t==PowerUpType::SPLIT)
	{
		for(int i = 0; i<BRK::NUM_BALLS;i++)
		{
			if(balls[i].isActive){
				Vector3 vel = balls[i].getVelocity();
				spawnBall(balls[i].getPosition(),rotateZ(vel,PI/3));
				spawnBall(balls[i].getPosition(),rotateZ(vel,-PI/3));
				break;
			}
		}
	}
	if(t==GROW)
		paddle.grow();
	if(t==SHRINK)
		paddle.shrink();
	if(t==FLIP && flipCooldown == 0)
		flipCooldown=PowerUpNS::FLIP_DURATION;
	if(t==SPACE_INVADERS)
	{
		paddle.enterSpaceMode();
		if(!enemyShip.isActive){
			enemyShip.create(Vector3(50,19,0));
			enemyShip.setVelocity(Vector3(-5,0,0));
		}
	}
		

}

Powerup* BREAKOUT::spawnPowerUp(Vector3 pos, PowerUpType t)
{
	for(int i = 0 ; i < BRK::NUM_POWER_UPS; i++)
	{
		if(!powerUps[i].isActive)
		{
			powerUps[i].create(pos,t);
			powerUps[i].setScale(Vector3(1,1,1));

			switch (t)
			{
			case SPLIT:
				powerUps[i].tex=tPowSplit;
				break;
			case FLIP:
				powerUps[i].tex=tPowFlip;
				break;
			case GROW:
				powerUps[i].tex=tPowGrow;
				break;
			case SHRINK:
				powerUps[i].tex=tPowShrink;
				break;
			case SPACE_INVADERS:
				powerUps[i].tex=tPowSpace;
				break;
			default:
				break;
			}
			return &powerUps[i];
		}
	}
	return nullptr;
}


Ball* BREAKOUT::spawnBall(Vector3 pos, Vector3 vel)
{
	for(int i = 0 ; i < BRK::NUM_BALLS; i++)
	{
		if(!balls[i].isActive)
		{
			balls[i].create(pos);
			Normalize(&vel,&vel);
			balls[i].setDirection(vel);
			return &balls[i];
		}
	}
	return nullptr;
}


Actor* BREAKOUT::spawnScenery(Vector3 pos, Vector3 vel, Geometry* geo, TEXTURE* tex)
{
	for(int i = 0 ; i < BRK::NUM_SCENERY; i++)
	{
		if(!scenery[i].isActive)
		{
			scenery[i].create(pos);
			scenery[i].setVelocity(vel);
			scenery[i].tex = tex;
			scenery[i].setGeometry(geo);
			return &scenery[i];
		}
	}
	return nullptr;
}