//=============================================================================
// lighting.fx by Frank Luna (C) 2008 All Rights Reserved.
//
// Transforms and lights geometry.
//=============================================================================

#include "sharedDefines.h"
#include "lighthelper.fx"

cbuffer cbPerFrame
{
	Light pLight;
	Light ambient;
	Light negaLight;
	Light gLight;
	Light eyes;
	bool gLightType; 
	bool highlight;
	Light lights[MAX_LIGHTS];
	int type[MAX_LIGHTS];

	

	int activeLights; //Set this in-game: Number of ACTIVE lights
	float3 gEyePosW;

};

bool gSpecularEnabled;

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWVP; 
	float4x4 gTexMtx;
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;
Texture2D gSpecMap;

SamplerState gTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
};

struct VS_IN
{
	float3 posL    : POSITION;
	float3 normalL : NORMAL;
	float2 texC    : TEXCOORD;
};

struct VS_OUT
{
	float4 posH    : SV_POSITION;
    float3 posW    : POSITION;
    float3 normalW : NORMAL;
    float2 texC    : TEXCOORD;
};

VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;
	
	// Transform to world space space.
	vOut.posW    = mul(float4(vIn.posL, 1.0f), gWorld);
	vOut.normalW = mul(float4(vIn.normalL, 0.0f), gWorld);
		
	// Transform to homogeneous clip space.
	vOut.posH = mul(float4(vIn.posL, 1.0f), gWVP);
	
	// Output vertex attributes for interpolation across triangle.
	vOut.texC  = mul(float4(vIn.texC, 0.0f, 1.0f), gTexMtx);
	
	return vOut;
}


float4 PS(VS_OUT pIn) : SV_Target
{
	// Get materials from texture maps.
	float4 diffuse = gDiffuseMap.Sample( gTriLinearSam, pIn.texC );
	float4 spec    = gSpecMap.Sample( gTriLinearSam, pIn.texC );
	
	if(highlight)
		diffuse = diffuse * 2.0;
	
	// Map [0,1] --> [0,256]
	spec.a *= 256.0f;

	// Interpolating normal can make it not be of unit length so normalize it.
	pIn.normalW = normalize(pIn.normalW);


	SurfaceInfo v = {pIn.posW, pIn.normalW, diffuse, spec};
	float3 litColor = float3(0.0f, 0.0f, 0.0f);
	float3 newColor;


	//Calculations for point lights
	[loop]
	for( uint i = 0; i < activeLights; i++ )
	{
		//Calculate as Spotlight
		if(type[i] == 1) {
			newColor = Spotlight(v, lights[i], gEyePosW);
			litColor.x = max(newColor.x,litColor.x);
			litColor.y = max(newColor.y,litColor.y);
			litColor.z = max(newColor.z,litColor.z);
		}

		//Calculate as Pointlight
		else {
			newColor = PointLight(v, lights[i], gEyePosW);
			litColor.x = max(newColor.x,litColor.x);
			litColor.y = max(newColor.y,litColor.y);
			litColor.z = max(newColor.z,litColor.z);
		}

	}

	//Ambient lighting loaded first
	litColor += ParallelLight(v, ambient, gEyePosW);

	//Flashlight bool
	if(gLightType)
	{
		litColor += Spotlight(v, gLight, gEyePosW);
	}

	litColor += PointLight(v, eyes, gEyePosW);

	//Dark emitter
	litColor += PointLight(v, negaLight, gEyePosW);

	float4 retval = float4(litColor, diffuse.a);
	retval = saturate(retval);

	return retval;
}

BlendState SrcAlphaBlendingAdd
{
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
    BlendOp = ADD;
    SrcBlendAlpha = ZERO;
    DestBlendAlpha = ZERO;
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};

technique10 LightTech
{
	pass P0
	{
		SetBlendState( SrcAlphaBlendingAdd, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
	}
}
