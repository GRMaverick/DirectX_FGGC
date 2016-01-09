#pragma once
#include "Structs.h"
class FogFX
{
private:
	ID3D11VertexShader* _pVertexShader;
	ID3D11PixelShader* _pPixelShader;
	ID3D11InputLayout* _pInputLayout;
	ID3D11Buffer* _pConstantBuffer;
	ID3D11Buffer* _pFogBuffer;
	ID3D11SamplerState* _pSamplerState;
	
	bool InitialiseShader(ID3D11Device* immediateDevice, HWND hwnd, WCHAR* sFilename);
	bool XM_CALLCONV SetShaderParameters(ID3D11DeviceContext* immediateDevice, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView* texture, float fogStart, float fogEnd);
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);
	void Draw(ID3D11DeviceContext* immediateContext, int indexCount);

public:
	FogFX();
	~FogFX();

	bool Initialise(ID3D11Device* immediateDevice, HWND hwnd);
	bool XM_CALLCONV Render(ID3D11DeviceContext* immediateDevice, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView* texture, float fogStart, float fogEnd);

};

