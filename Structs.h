#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <fstream>						//For loading in an external file
#include <vector>							//For storing the XMFLOAT3/2 variables
#include <map>	
#include <string>							//For fast searching when re-creating the index buffer

using namespace std;
using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 TexC;

	bool operator<(const SimpleVertex other) const
	{
		return memcmp((void*)this, (void*)&other, sizeof(SimpleVertex)) > 0;
	};
};
struct OBJMesh{
	ID3D11Buffer * VertexBuffer;
	ID3D11Buffer * IndexBuffer;
	UINT VBStride;
	UINT VBOffset;
	UINT IndexCount;
};
struct ConstantBuffer
{
	XMMATRIX												mWorld;
	XMMATRIX												mView;
	XMMATRIX												mProjection;
	XMFLOAT4												DiffuseMtrl;
	XMFLOAT4												DiffuseLight;
	XMFLOAT4												AmbientMtrl;
	XMFLOAT4												AmbientLight;
	XMFLOAT4												SpecularMtrl;
	XMFLOAT4												SpecularLight;
	XMFLOAT3												EyePosW;
	float															SpecularPower;
	XMFLOAT3												LightVecW;
};

//
// Height Mapping
//
struct VertexType
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};
struct HeightMapType
{
	float x, y, z;
};