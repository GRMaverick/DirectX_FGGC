#include "Bilboard.h"


UINT Bilboard::VBOffset = 0;
UINT Bilboard::VBStride = 0;
UINT Bilboard::IndexCount = 0;

Bilboard::Bilboard()
{
	_vertexBuffer = nullptr;
	_indexBuffer = nullptr;
}
Bilboard::~Bilboard()
{
	if (_vertexBuffer) _vertexBuffer->Release();
	if (_indexBuffer) _indexBuffer->Release();
}

HRESULT Bilboard::Initialise(ID3D11Device * pd3dDevice, float height, float width)
{
	HRESULT hr;

	hr = initVertexData(pd3dDevice, height, width);
	hr = initIndexData(pd3dDevice);

	Bilboard::height = height;
	Bilboard::width = width;

	if (FAILED(hr)) {
		return hr;
	}
	else {
		return S_OK;
	}
}

HRESULT Bilboard::initVertexData(ID3D11Device * pd3dDevice, float height, float width)
{
	HRESULT hr;

	float halfHeight = height / 2;
	float halfWidth = width / 2;

	// Pine Tree
	// Create vertex buffer
	SimpleVertex vertices[] =
	{
		{ XMFLOAT3(-halfWidth, -halfHeight, 0.0f), XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-halfWidth, halfHeight, 0.0f), XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(halfWidth, halfHeight, 0.0f), XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(halfWidth, -halfHeight, 0.0f), XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex)* 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;

	// Update Stride and offset
	VBOffset = 0;
	VBStride = sizeof(SimpleVertex);

	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &_vertexBuffer);

	if (FAILED(hr)) {
		return hr;
	}
	else {
		return S_OK;
	}
}

HRESULT Bilboard::initIndexData(ID3D11Device * pd3dDevice)
{
	HRESULT hr;

	// Create index buffer
	WORD indices[] =
	{
		0, 1, 2,
		1, 3, 2,
	};
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD)* 6;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices;

	// Update Index count
	IndexCount = 6;

	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &_indexBuffer);

	if (FAILED(hr)) {
		return hr;
	}
	else {
		return S_OK;
	}
}

HRESULT Bilboard::Update(float elapsedTime, ID3D11Device * pd3dDevice, Camera currCamera)
{
	HRESULT hr;

	//// convert data into usable format
	XMFLOAT3 worldPos = XMFLOAT3(_world._41, _world._42, _world._43);
	XMFLOAT3 camEyePos = XMFLOAT3(currCamera.GetEye().x, currCamera.GetEye().y, currCamera.GetEye().z);

	// Calculate the normal vector which will make the billboard face the camera
	XMFLOAT3 planeNormal; // = worldPos - camEyePos;
	planeNormal.x = worldPos.x - camEyePos.x;
	planeNormal.y = 0.0f;
	planeNormal.z = worldPos.z - camEyePos.z;

	// Translate normal float to vector and normalize
	XMVECTOR vectorNormal = XMLoadFloat3(&planeNormal);
	vectorNormal = XMVector3Normalize(vectorNormal);

	//XMVECTOR vectorCam = XMLoadFloat3(&camEyePos);
	//XMVECTOR vectorWorld = XMLoadFloat3(&worldPos);
	//XMVECTOR vectorNormal = vectorWorld - vectorCam;
	//vectorNormal = XMVector3Normalize(vectorNormal);

	// Translate up float to vector
	XMFLOAT3 floatUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMVECTOR vectorUp = XMLoadFloat3(&floatUp);

	// For Width
	XMVECTOR vectorRight = XMVector3Normalize(XMVector3Cross(vectorNormal, vectorUp));
	vectorRight = vectorRight * (width / 2);

	// For Height
	floatUp = XMFLOAT3(0.0f, (height / 2), 0.0f);
	vectorUp = XMLoadFloat3(&floatUp);

	// Build Vertex buffer
	XMFLOAT3 vert[4];
	XMFLOAT3 vertToConvert[4] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) };

	XMStoreFloat3(&vertToConvert[0], (-vectorRight));
	XMStoreFloat3(&vertToConvert[1], vectorRight);
	XMStoreFloat3(&vertToConvert[2], (-vectorRight) + vectorUp);
	XMStoreFloat3(&vertToConvert[3], vectorRight + vectorUp);

	vert[0] = XMFLOAT3(worldPos.x + vertToConvert[0].x, worldPos.y + vertToConvert[0].y, worldPos.z + vertToConvert[0].z);
	vert[1] = XMFLOAT3(worldPos.x + vertToConvert[1].x, worldPos.y + vertToConvert[1].y, worldPos.z + vertToConvert[1].z);
	vert[2] = XMFLOAT3(worldPos.x + vertToConvert[2].x, worldPos.y + vertToConvert[2].y, worldPos.z + vertToConvert[2].z);
	vert[3] = XMFLOAT3(worldPos.x + vertToConvert[3].x, worldPos.y + vertToConvert[3].y, worldPos.z + vertToConvert[3].z);

	// Pine Tree
	// Create vertex buffer
	SimpleVertex vertices[] =
	{
		{ vert[0], XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ vert[1], XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ vert[2], XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ vert[3], XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex)* 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;

	// Update Stride and offset
	VBOffset = 0;
	VBStride = sizeof(SimpleVertex);

	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &_vertexBuffer);

	if (FAILED(hr)) {
		return hr;
	}
	else {
		return S_OK;
	}

	this->UpdateWorld();
}

void Bilboard::Draw(ID3D11Device * pd3dDevice, ID3D11DeviceContext * pImmediateContext)
{
	// NOTE: We are assuming that the constant buffers and all other draw setup has already taken place

	pImmediateContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &Bilboard::VBStride, &Bilboard::VBOffset);
	pImmediateContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pImmediateContext->DrawIndexed(Bilboard::IndexCount, 0, 0);
}