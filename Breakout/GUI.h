//=======================================================================================
// TreeSprites.h by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#ifndef GUI_H
#define GUI_H

#include "d3dUtil.h"
#include "constants.h"
#include "Light.h"
#include "sharedDefines.h"
#include <string>
#include "Vertex.h"

namespace guiNS {
	const int SPRITES = MAX_GUI; //Use to set how many files it should load
}

class GUI
{
public:

	GUI();
	~GUI();

	void init(ID3D10Device* device, const D3DXVECTOR3 centers[], UINT numSprites, std::wstring filenames[]);
	void draw(const D3DXMATRIX& proj); //Note: this draws images directly to the screen
	int sprite;
	float alpha;
private:
	void buildVB(const D3DXVECTOR3 centers[]);
	void buildFX();
	void buildVertexLayout();
	void buildShaderResourceView(std::wstring filenames[]);

private:

	UINT mNumSprites;

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;

	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10ShaderResourceView* mTreeMapArrayRV;
	ID3D10ShaderResourceView* mSpriteMapRV;

	ID3D10EffectMatrixVariable* mfxViewProjVar;
	ID3D10EffectVariable* mfxEyePosVar;
	ID3D10EffectVariable* mfxLightVar;
	ID3D10EffectScalarVariable* mfxSprite;
	ID3D10EffectScalarVariable* mfxAlpha;
	ID3D10EffectShaderResourceVariable* mfxTreeMapArrayVar;
	ID3D10EffectShaderResourceVariable* mfxSpriteMapVar;

	ID3D10InputLayout* mVertexLayout;
};

#endif // SPRITES_H
