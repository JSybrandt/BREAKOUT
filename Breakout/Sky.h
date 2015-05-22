#ifndef SKYDOME_H
#define SKYDOME_H

#include "d3dUtil.h"
#include "constants.h"
#include <vector>

class BREAKOUT;

class Sky
{
public:
	Sky();
	~Sky();

	void init(ID3D10Device* device, BREAKOUT* g, float radius);

	void draw(Matrix& camera, Matrix& projection);

private:
	Sky(const Sky& rhs);
	Sky& operator=(const Sky& rhs);
	void Subdivide(std::vector<D3DXVECTOR3>& vertices, std::vector<DWORD>& indices);
	void BuildGeoSphere(UINT numSubdivisions, float radius, std::vector<D3DXVECTOR3>& vertices, std::vector<DWORD>& indices);
	BREAKOUT* game;

private:
	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;

	ID3D10ShaderResourceView* mCubeMap;

	UINT mNumIndices;
 
	ID3D10EffectTechnique* mTech;
	ID3D10EffectMatrixVariable* mfxWVPVar;
	ID3D10EffectShaderResourceVariable* mfxCubeMapVar;

	Matrix world;
};

#endif // SKY_H