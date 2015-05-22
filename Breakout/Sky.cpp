#include "Sky.h"
#include "Camera.h"
//#include "Effects.h"
#include "CanisMajor.h"

typedef std::vector<D3DXVECTOR3> VertexList;
typedef std::vector<DWORD> IndexList;

struct SkyVertex
{
	D3DXVECTOR3 pos;
};
 
Sky::Sky()
: md3dDevice(0), mVB(0), mIB(0), mCubeMap(0)
{
	mNumIndices = 0;
	Identity(&world);
}

Sky::~Sky()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}

void Sky::init(ID3D10Device* device, CanisMajor* g, float radius)
{
	md3dDevice = device;

	HR(D3DX10CreateShaderResourceViewFromFile(device, 
		L".\\textures\\skybox.dds", 0, 0, &mCubeMap, 0 ));

	game = g;

	mTech         = g->skyFX->GetTechniqueByName("SkyTech");
	mfxWVPVar     = g->skyFX->GetVariableByName("gWVP")->AsMatrix();
	mfxCubeMapVar = g->skyFX->GetVariableByName("gCubeMap")->AsShaderResource();


	std::vector<D3DXVECTOR3> vertices;
	std::vector<DWORD> indices;

	BuildGeoSphere(2, radius, vertices, indices);

	std::vector<SkyVertex> skyVerts(vertices.size());
	for(size_t i = 0; i < vertices.size(); ++i)
	{
		// Scale on y-axis to turn into an ellipsoid to make a flatter Sky surface
		skyVerts[i].pos = 0.5f*vertices[i];
	}

	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(SkyVertex) * (UINT)skyVerts.size();
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &skyVerts[0];
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

	mNumIndices = (UINT)indices.size();

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(DWORD) * mNumIndices;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indices[0];
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}

void Sky::draw(Matrix& camera, Matrix& projection)
{
	Vector3 eyePos = game->camera.getPosition();

	// center Sky about eye in world space
	D3DXMATRIX W;
	D3DXMatrixTranslation(&W, eyePos.x, eyePos.y, eyePos.z);

	D3DXMATRIX V = camera;
	D3DXMATRIX P = projection;

	D3DXMATRIX WVP = W*V*P;

	HR(mfxWVPVar->SetMatrix((float*)WVP));
	HR(mfxCubeMapVar->SetResource(mCubeMap));

	UINT stride = sizeof(SkyVertex);
    UINT offset = 0;
    md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	//md3dDevice->IASetInputLayout(InputLayout::Pos);
	md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );

    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        ID3D10EffectPass* pass = mTech->GetPassByIndex(p);

		pass->Apply(0);
		md3dDevice->DrawIndexed(mNumIndices, 0, 0);
	}
}
 

void Sky::BuildGeoSphere(UINT numSubdivisions, float radius, std::vector<D3DXVECTOR3>& vertices, std::vector<DWORD>& indices)
{
	// Put a cap on the number of subdivisions.
	numSubdivisions = Min(numSubdivisions, UINT(5));

	// Approximate a sphere by tesselating an icosahedron.

	const float X = 0.525731f; 
	const float Z = 0.850651f;

	D3DXVECTOR3 pos[12] = 
	{
		D3DXVECTOR3(-X, 0.0f, Z),  D3DXVECTOR3(X, 0.0f, Z),  
		D3DXVECTOR3(-X, 0.0f, -Z), D3DXVECTOR3(X, 0.0f, -Z),    
		D3DXVECTOR3(0.0f, Z, X),   D3DXVECTOR3(0.0f, Z, -X), 
		D3DXVECTOR3(0.0f, -Z, X),  D3DXVECTOR3(0.0f, -Z, -X),    
		D3DXVECTOR3(Z, X, 0.0f),   D3DXVECTOR3(-Z, X, 0.0f), 
		D3DXVECTOR3(Z, -X, 0.0f),  D3DXVECTOR3(-Z, -X, 0.0f)
	};

	DWORD k[60] = 
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,    
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,    
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0, 
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7 
	};

	vertices.resize(12);
	indices.resize(60);

	for(int i = 0; i < 12; ++i)
		vertices[i] = pos[i];

	for(int i = 0; i < 60; ++i)
		indices[i] = k[i];

	for(UINT i = 0; i < numSubdivisions; ++i)
		Subdivide(vertices, indices);

	// Project vertices onto sphere and scale.
	for(size_t i = 0; i < vertices.size(); ++i)
	{
		D3DXVec3Normalize(&vertices[i], &vertices[i]);
		vertices[i] *= radius;
	}
}

typedef std::vector<D3DXVECTOR3> VertexList;
typedef std::vector<DWORD> IndexList;

void Sky::Subdivide(std::vector<D3DXVECTOR3>& vertices, std::vector<DWORD>& indices)
{
	VertexList vin = vertices;
	IndexList  iin = indices;

	vertices.resize(0);
	indices.resize(0);

	UINT numTris = (UINT)iin.size()/3;
	for(UINT i = 0; i < numTris; ++i)
	{
		D3DXVECTOR3 v0 = vin[ iin[i*3+0] ];
		D3DXVECTOR3 v1 = vin[ iin[i*3+1] ];
		D3DXVECTOR3 v2 = vin[ iin[i*3+2] ];

		D3DXVECTOR3 m0 = 0.5f*(v0 + v1);
		D3DXVECTOR3 m1 = 0.5f*(v1 + v2);
		D3DXVECTOR3 m2 = 0.5f*(v0 + v2);

		vertices.push_back(v0); // 0
		vertices.push_back(v1); // 1
		vertices.push_back(v2); // 2
		vertices.push_back(m0); // 3
		vertices.push_back(m1); // 4
		vertices.push_back(m2); // 5
 
		indices.push_back(i*6+0);
		indices.push_back(i*6+3);
		indices.push_back(i*6+5);

		indices.push_back(i*6+3);
		indices.push_back(i*6+4);
		indices.push_back(i*6+5);

		indices.push_back(i*6+5);
		indices.push_back(i*6+4);
		indices.push_back(i*6+2);

		indices.push_back(i*6+3);
		indices.push_back(i*6+1);
		indices.push_back(i*6+4);
	}
}