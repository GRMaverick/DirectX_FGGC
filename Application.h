#pragma once
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#include <dinput.h>

#include <vector>
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

#include "resource.h"

#include "DDSTextureLoader.h"
#include "OBJLoader.h"
#include "Structs.h"
#include "HMTerrain.h"
#include "Terrain.h"
#include "Camera.h"
#include "Aircraft.h"
#include "Bilboard.h"

using namespace std;
using namespace DirectX;

class Application
{
private:
	// ********************** PROPERTIES *********************** //
	HINSTANCE											_hInst;
	HWND													_hWnd;
	D3D_DRIVER_TYPE							_driverType;
	D3D_FEATURE_LEVEL						_featureLevel;
	ID3D11Device*										_pd3dDevice;
	ID3D11DeviceContext*						_pImmediateContext;
	IDXGISwapChain*								_pSwapChain;
	ID3D11DepthStencilView*					_depthStencilView;
	ID3D11Texture2D*								_depthStencilBuffer;
	ID3D11RenderTargetView*					_pRenderTargetView;
	ID3D11VertexShader*							_pVertexShader;
	ID3D11PixelShader*							_pPixelShader;
	ID3D11InputLayout*								_pVertexLayout;

	ID3D11ShaderResourceView *			_pTextureRV;
	ID3D11ShaderResourceView*			_pPlaneTexRV;
	ID3D11ShaderResourceView*			_pTerrainTexture;
	ID3D11ShaderResourceView*			_pTreeTexture;
	ID3D11ShaderResourceView*			_pRunwayTexture;

	ID3D11Buffer*										_pVertexBuffer;
	ID3D11Buffer*										_pVertexBufferPyramid;
	ID3D11Buffer*										_pIndexBuffer;
	ID3D11Buffer*										_pIndexBufferPyramid;
	ID3D11Buffer*										_pConstantBuffer; 

	XMFLOAT4X4										_world;
	XMFLOAT4X4										_planeWorld;
	XMFLOAT4X4										_world1;
	XMFLOAT4X4										_world2;
	XMFLOAT4X4										_world3;
	XMFLOAT4X4										_world4;
	XMFLOAT4X4										_view;
	XMFLOAT4X4										_projection;

	ID3D11RasterizerState*						_wireFrame;
	D3D11_RASTERIZER_DESC			_wfdesc;
	ID3D11RasterizerState*						_solidShape;
	vector<XMFLOAT4X4 *>						_objects;
	ID3D11SamplerState *							_pSamplerLinear;

	XMFLOAT3											lightDirection;
	XMFLOAT4											diffuseMaterial;
	XMFLOAT4											diffuseLight;
	XMFLOAT4											ambientMaterial;
	XMFLOAT4											ambientLight;
	float															specularPower;
	XMFLOAT4											specularMaterial;
	XMFLOAT4											specularLight;
	XMFLOAT3											eyePosW;

	UINT														_WindowHeight;
	UINT														_WindowWidth;

	Camera*													_camera1;
	Camera*													_camera2;
	Camera*													_camera3;
	Camera*													_camera4;

	int															_pKeyState;
	int															_cameraState;

	IDirectInputDevice8*								_DIKeyboard;
	IDirectInputDevice8*								_DIMouse;
	DIMOUSESTATE									_mouseLastState;
	LPDIRECTINPUT8								_DirectInput;

	// **************** UPDATE VARIABLES **************** //	
	float															_newTime;
	float															_oldTime;
	float															_deltaTime;
	float															_totalTime;
	float															_frameCounter;
	// ************************************************************//

	// *********** GAME OBJECT VARIABLES *********** //
	Terrain*													_pTerrain;
	Terrain*													_pRunway;
	OBJMesh												_planeMesh;
	OBJMesh												_pCityMesh;
	Aircraft*													_planeObject;
	GameObject*											_pPlane2;
	Bilboard*												_pTree;
	float															_rotationUD;
	float															_rotationLR;
	float															_rotationYaw;
	// ************************************************************//

	// ********************** METHODS *********************** //
	HRESULT												InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT												InitDevice();
	HRESULT												CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT												InitShadersAndInputLayout();
	HRESULT												InitVertexBuffer();
	HRESULT												InitIndexBuffer();
	bool															InitDirectInput(HINSTANCE hInstance);
	void															DetectInput(float elapsedTime);

	int															Keyboard();
	Camera*													GetCamera();

	void															UpdateCamState();
	void															UpdateCamera();
	void															Cleanup();
	// ************************************************************//

public:
	Application();
	~Application();

	HRESULT												Initialise(HINSTANCE hInstance, int nCmdShow);

	void															Update();
	void															Draw();
};