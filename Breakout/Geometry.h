#pragma once

#include "constants.h"
#include "d3dUtil.h"
#include "Vertex.h"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using std::fstream;
using std::stringstream;
using std::string;
using std::vector;

class Geometry{
public:

	Geometry();
	~Geometry();

	virtual void init(ID3D10Device* device, D3DXCOLOR color = WHITE, D3D_PRIMITIVE_TOPOLOGY topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	virtual void init(ID3D10Device* device, std::string objFile, LPCWSTR texFile = L".\\textures\\grey.dds", LPCWSTR specFile = L".\\textures\\defaultspec.dds");
	virtual void init(ID3D10Device* device, std::string objFile, TEXTURE* tex, TEXTURE* spec);
	virtual void draw(UINT offset = 0);
	
	Vector3 getAABBMin(){return min;}
	Vector3 getAABBMax(){return max;}

	void setCustomAABB(Vector3 min, Vector3 max){this->min = min; this->max = max;}

	void setTexture(ID3D10ShaderResourceView* tex){mDiffuseMapRV=tex;}

	ID3D10ShaderResourceView* mDiffuseMapRV;
	ID3D10ShaderResourceView* mSpecMapRV;

protected:

	D3DXCOLOR color;

	void initVectorBuffer(Vertex* vertices);
	void initIndexBuffer(DWORD * indices);
	void initRasterState();

	DWORD numVertices;
	DWORD numIndexes;

	D3D10_RASTERIZER_DESC rasterState;

	void calculateDefaultAABB(vector<Vector3>& verts);

private:
	D3D_PRIMITIVE_TOPOLOGY topology;
	ID3D10RasterizerState * g_pRasterState;
	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
	bool usesIndexBuffer;

	Vertex * verts;
	DWORD * indices;

	Vector3 min, max;
};