#include "FogFX.h"


FogFX::FogFX()
{
	_pVertexShader = nullptr;
	_pPixelShader = nullptr;
	_pInputLayout = nullptr;
	_pConstantBuffer = nullptr;
	_pFogBuffer = nullptr;
	_pSamplerState = nullptr;
}
FogFX::~FogFX()
{
	_pVertexShader->Release();
	_pPixelShader->Release();
	_pInputLayout->Release();
	_pConstantBuffer->Release();
	_pFogBuffer->Release();
	_pSamplerState->Release();
}

bool FogFX::Initialise(ID3D11Device* device, HWND hwnd){
	bool result;

	result = InitialiseShader(device, hwnd, L"DX11 Framework.fx");

	if (!result)
		return false;

	return true;
}
bool FogFX::InitialiseShader(ID3D11Device* device, HWND hwnd, WCHAR* sFilename)
{
	bool result;
	unsigned int numElements;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	D3D11_BUFFER_DESC constantBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC fogBufferDesc;

	numElements = 0;
	errorMessage = 0;
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer = nullptr;

	// Read Vertex Shader
	result = D3DCompileFromFile(sFilename, NULL, NULL, "VS", "vs_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
			OutputShaderErrorMessage(errorMessage, hwnd, sFilename);
		else
			MessageBox(hwnd, sFilename, L"Missing VS", MB_OK);

		return false;
	}
	
	//Create the vertex shader from the buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_pVertexShader);
	if (FAILED(result))
		return false;

	// Read Pixel Shader
	result = D3DCompileFromFile(sFilename, NULL, NULL, "PS", "ps_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
			OutputShaderErrorMessage(errorMessage, hwnd, sFilename);
		else
			MessageBox(hwnd, sFilename, L"Missing PS", MB_OK);

		return false;
	}

	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pPixelShader);
	if (FAILED(result))
		return false;

	// Create the Input Layour description
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = 0;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &_pInputLayout);

	if (FAILED(result))
		return false;

	vertexShaderBuffer->Release();
	pixelShaderBuffer->Release();

	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;

	if (FAILED(result))
		return false;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	result = device->CreateSamplerState(&samplerDesc, &_pSamplerState);

	if (FAILED(result))
		return false;

	fogBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	fogBufferDesc.ByteWidth = sizeof(FogConstantBuffer);
	fogBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	fogBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	fogBufferDesc.MiscFlags = 0;
	fogBufferDesc.StructureByteStride = 0;
	result = device->CreateBuffer(&fogBufferDesc, NULL, &_pFogBuffer);

	if (FAILED(result))
		return false;

	return true;
	
}
bool XM_CALLCONV FogFX::SetShaderParameters(ID3D11DeviceContext* immediateContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView* texture, float fogStart, float fogEnd)
{
	bool result;
	D3D11_MAPPED_SUBRESOURCE mappedRes;
	ConstantBuffer* cbuffer;
	unsigned int bufferNumber;
	FogConstantBuffer* fcbuffer;

	result = immediateContext->Map(_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes);
	
	if (FAILED(result))
		return false;

	cbuffer = (ConstantBuffer*)mappedRes.pData;

	XMMatrixTranspose(world);
	XMMatrixTranspose(view);
	XMMatrixTranspose(projection);

	cbuffer->mWorld = world;
	cbuffer->mView = view;
	cbuffer->mProjection = projection;

	immediateContext->Unmap(_pConstantBuffer, 0);
	bufferNumber = 0;
	immediateContext->VSSetConstantBuffers(bufferNumber, 1, &_pConstantBuffer);
	immediateContext->PSSetShaderResources(0, 1, &texture);

	result = immediateContext->Map(_pFogBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes);

	if (FAILED(result))
		return false;

	fcbuffer->fogStart = fogStart;
	fcbuffer->fogEnd = fogEnd;

	immediateContext->Unmap(_pFogBuffer, 0);
	bufferNumber = 1;

	immediateContext->VSSetConstantBuffers(bufferNumber, 1, &_pFogBuffer);

	return true;
}
bool XM_CALLCONV FogFX::Render(ID3D11DeviceContext* immediateContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView* texture, float fogStart, float fogEnd)
{
	bool result;

	result = SetShaderParameters(immediateContext, world, view, projection, texture, fogStart, fogEnd);

	if (FAILED(result))
		return false;

	Draw(immediateContext, indexCount);

	return true;
}
void FogFX::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	compileErrors = (char*)(errorMessage->GetBufferPointer());
	bufferSize = errorMessage->GetBufferSize();
	fout.open("shader-error.txt");

	for (i = 0; i < bufferSize; i++)
		fout << compileErrors[i];

	fout.close();

	errorMessage->Release();
	MessageBox(hwnd, L"Error Compiling Shader. Check shader-error.txt", shaderFilename, MB_OK);
	return;
}
void FogFX::Draw(ID3D11DeviceContext* immediateContext, int indexCount)
{
	immediateContext->IASetInputLayout(_pInputLayout);
	immediateContext->VSSetShader(_pVertexShader, NULL, 0);
	immediateContext->PSSetShader(_pPixelShader, NULL, 0);
	immediateContext->PSSetSamplers(0, 1, &_pSamplerState);
	immediateContext->DrawIndexed(indexCount, 0, 0);
}
