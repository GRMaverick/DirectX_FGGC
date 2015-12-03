#pragma once

#include <vector>
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"
#include "DDSTextureLoader.h"

using namespace std;
using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 TexC;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 DiffuseMtrl;
	XMFLOAT4 DiffuseLight;
	XMFLOAT4 AmbientMtrl;
	XMFLOAT4 AmbientLight;
	XMFLOAT4 SpecularMtrl;
	XMFLOAT4 SpecularLight;
	XMFLOAT3 EyePosW;
	float SpecularPower;
	XMFLOAT3 LightVecW;
};

class Application
{
private:
	HINSTANCE												_hInst;
	HWND														_hWnd;
	D3D_DRIVER_TYPE								_driverType;
	D3D_FEATURE_LEVEL							_featureLevel;
	ID3D11Device*											_pd3dDevice;
	ID3D11DeviceContext*							_pImmediateContext;
	IDXGISwapChain*									_pSwapChain;
	ID3D11DepthStencilView*						_depthStencilView;
	ID3D11Texture2D*									_depthStencilBuffer;
	ID3D11RenderTargetView*						_pRenderTargetView;
	ID3D11VertexShader*								_pVertexShader;
	ID3D11PixelShader*							    _pPixelShader;
	ID3D11InputLayout*								    _pVertexLayout;
	ID3D11ShaderResourceView *				_pTextureRV = nullptr;
	ID3D11Buffer*											_pVertexBuffer;
	ID3D11Buffer*											_pVertexBufferPyramid;
	ID3D11Buffer*											_pIndexBuffer;
	ID3D11Buffer*											_pIndexBufferPyramid;
	ID3D11Buffer*											_pConstantBuffer;
	XMFLOAT4X4										    _world;
	//temp stuff
	XMFLOAT4X4											_world1;
	XMFLOAT4X4											_world2;
	XMFLOAT4X4											_view;
	XMFLOAT4X4											_projection;

	ID3D11RasterizerState*							_wireFrame;
	D3D11_RASTERIZER_DESC				wfdesc;
	vector<XMFLOAT4X4 *>							_objects;
	ID3D11SamplerState *								_pSamplerLinear = nullptr;

	//lighting stuff
	XMFLOAT3												lightDirection;
	XMFLOAT4												diffuseMaterial;
	XMFLOAT4												diffuseLight;
	XMFLOAT4												ambientMaterial;
	XMFLOAT4												ambientLight;
	float																specularPower;
	XMFLOAT4												specularMaterial;
	XMFLOAT4												specularLight;
	XMFLOAT3												eyePosW;

	bool																_frameState;

private:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();
	void Cleanup();
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT InitShadersAndInputLayout();
	HRESULT InitVertexBuffer();
	HRESULT InitIndexBuffer();

	UINT _WindowHeight;
	UINT _WindowWidth;

	int Keyboard();

	int _pKeyState;

public:
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	void Update();
	void Draw();
};