#include "GUI.h"
#include <string>

GUI::GUI()
{
	ZeroMemory(this, sizeof(GUI));
	sprite = -1;
}

GUI::~GUI()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mFX);
	ReleaseCOM(mTreeMapArrayRV);
	ReleaseCOM(mVertexLayout);
}

void GUI::init(ID3D10Device* device, const D3DXVECTOR3 centers[], UINT numSprites, std::wstring filenames[])
{
	md3dDevice = device;

	//changing this does not change the buildshaderresource function.
	mNumSprites = numSprites;

	buildShaderResourceView(filenames);
	buildVB(centers);
	buildFX();
	buildVertexLayout();

	alpha = 1;
}

//Pass in project matrix only to draw specific items to the screen
void GUI::draw(const D3DXMATRIX& proj)
{
	mfxEyePosVar->SetRawValue((void*)&Vector3(0,0,0), 0, sizeof(D3DXVECTOR3));
	//mfxLightVar->SetRawValue((void*)&L, 0, sizeof(Light));
	mfxViewProjVar->SetMatrix((float*)&proj);
	mfxTreeMapArrayVar->SetResource(mTreeMapArrayRV);
	mfxSprite->SetInt(sprite);
	mfxAlpha->SetFloat(alpha);

	md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	md3dDevice->IASetInputLayout(mVertexLayout);

	D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );

    for(UINT i = 0; i < techDesc.Passes; ++i)
    {
        ID3D10EffectPass* pass = mTech->GetPassByIndex(i);
		pass->Apply(0);

		UINT stride = sizeof(TreeVertex);
		UINT offset = 0;
		md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
		md3dDevice->Draw(mNumSprites, 0);
	}
	alpha = 1;
}


void GUI::buildVB(const D3DXVECTOR3 centers[])
{
	TreeVertex* v = new TreeVertex[mNumSprites];

	for(UINT i = 0; i < mNumSprites; ++i)
	{
		v[i].centerW  = centers[i];
		v[i].sizeW    = D3DXVECTOR2(12.0f, 12.0f);
	}
     
    D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(TreeVertex) * mNumSprites;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = v;
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

	delete[] v;
}

void GUI::buildFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
  
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	hr = D3DX10CreateEffectFromFile(L"GUI.fx", 0, 0, 
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

	mTech = mFX->GetTechniqueByName("TreeBillboardTech");
	
	mfxViewProjVar     = mFX->GetVariableByName("gViewProj")->AsMatrix();
	mfxEyePosVar       = mFX->GetVariableByName("gEyePosW");
	mfxLightVar        = mFX->GetVariableByName("gLight");
	mfxTreeMapArrayVar = mFX->GetVariableByName("gDiffuseMapArray")->AsShaderResource();
	mfxSpriteMapVar = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSprite = mFX->GetVariableByName("sprite")->AsScalar();
	mfxAlpha  = mFX->GetVariableByName("alpha")->AsScalar();
}

void GUI::buildVertexLayout()
{
	// Create the vertex input layout.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"SIZE",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	// Create the input layout
    D3D10_PASS_DESC PassDesc;
    mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
    HR(md3dDevice->CreateInputLayout(vertexDesc, 2, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &mVertexLayout));
}

void GUI::buildShaderResourceView(std::wstring filenames[])
{
	//
	// Load the texture elements individually from file.  These textures
	// won't be used by the GPU (0 bind flags), they are just used to 
	// load the image data from file.  We use the STAGING usage so the
	// CPU can read the resource.
	//

	const int files = guiNS::SPRITES;

	ID3D10Texture2D* srcTex[files];
	for(UINT i = 0; i < files; ++i)
	{
		D3DX10_IMAGE_LOAD_INFO loadInfo;

        loadInfo.Width  = D3DX10_FROM_FILE;
        loadInfo.Height = D3DX10_FROM_FILE;
        loadInfo.Depth  = D3DX10_FROM_FILE;
        loadInfo.FirstMipLevel = 0;
        loadInfo.MipLevels = D3DX10_FROM_FILE;
        loadInfo.Usage = D3D10_USAGE_STAGING;
        loadInfo.BindFlags = 0;
        loadInfo.CpuAccessFlags = D3D10_CPU_ACCESS_WRITE | D3D10_CPU_ACCESS_READ;
        loadInfo.MiscFlags = 0;
        loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        loadInfo.Filter = D3DX10_FILTER_NONE;
        loadInfo.MipFilter = D3DX10_FILTER_NONE;
		loadInfo.pSrcInfo  = 0;

        HR(D3DX10CreateTextureFromFile(md3dDevice, filenames[i].c_str(), 
			&loadInfo, 0, (ID3D10Resource**)&srcTex[i], 0));
	}

	//
	// Create the texture array.  Each element in the texture 
	// array has the same format/dimensions.
	//

	D3D10_TEXTURE2D_DESC texElementDesc;
	srcTex[0]->GetDesc(&texElementDesc);

	D3D10_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width              = texElementDesc.Width;
	texArrayDesc.Height             = texElementDesc.Height;
	texArrayDesc.MipLevels          = texElementDesc.MipLevels;
	texArrayDesc.ArraySize          = files;
	texArrayDesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
	texArrayDesc.SampleDesc.Count   = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage              = D3D10_USAGE_DEFAULT;
	texArrayDesc.BindFlags          = D3D10_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags     = 0;
	texArrayDesc.MiscFlags          = 0;

	ID3D10Texture2D* texArray = 0;
	HR(md3dDevice->CreateTexture2D( &texArrayDesc, 0, &texArray));

	//
	// Copy individual texture elements into texture array.
	//

	// for each texture element...
	for(UINT i = 0; i < files; ++i)
	{
		// for each mipmap level...
		for(UINT j = 0; j < texElementDesc.MipLevels; ++j)
		{
			D3D10_MAPPED_TEXTURE2D mappedTex2D;
			srcTex[i]->Map(j, D3D10_MAP_READ, 0, &mappedTex2D);
                    
            md3dDevice->UpdateSubresource(texArray, 
				D3D10CalcSubresource(j, i, texElementDesc.MipLevels),
                0, mappedTex2D.pData, mappedTex2D.RowPitch, 0);

            srcTex[i]->Unmap(j);
		}
	}	

	//
	// Create a resource view to the texture array.
	//
	
	D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = files;

	HR(md3dDevice->CreateShaderResourceView(texArray, &viewDesc, &mTreeMapArrayRV));

	//
	// Cleanup--we only need the resource view.
	//

	ReleaseCOM(texArray);

	for(UINT i = 0; i < files; ++i)
		ReleaseCOM(srcTex[i]);
}