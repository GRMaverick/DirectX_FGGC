#include "Terrain.h"

ID3D11Buffer* Terrain::_vertexBuffer = nullptr;
ID3D11Buffer* Terrain::_indexBuffer = nullptr;

UINT Terrain::VBOffset = 0;
UINT Terrain::VBStride = 0;
UINT Terrain::IndexCount = 0;

Terrain::Terrain()
{
	_pTexture = nullptr;
}
Terrain::~Terrain()
{
	if (_vertexBuffer) _vertexBuffer->Release();
	if (_indexBuffer) _indexBuffer->Release();
	if (_pTexture) _pTexture->Release();
}

HRESULT Terrain::Initialise(ID3D11Device * pd3dDevice, ID3D11ShaderResourceView* texture, float width, float depth, int verticesCountX, int verticesCountZ)
{
	HRESULT hr;

	hr = InitialiseVB(pd3dDevice, width, depth, verticesCountX, verticesCountZ);
	hr = InitialiseIB(pd3dDevice, width, depth, verticesCountX, verticesCountZ);

	_pTexture = texture;

	if (FAILED(hr)) {
		return hr;
	}
	else {
		return S_OK;
	}
}

HRESULT Terrain::InitialiseVB(ID3D11Device * pd3dDevice, float width, float depth, UINT m, UINT n)
{
	HRESULT hr;

	UINT vertexCount = m * n;

	float dx = width / (n - 1);
	float dz = depth / (m - 1);

	float du = 50.0f / (n - 1);
	float dv = 50.0f / (m - 1);

	vector<SimpleVertex> vertices;
	vertices.resize(vertexCount);

	// Get heightmap
	std::vector<float> heightData;
	Loader::LoadHeightMap("terrainmap.raw", heightData, width, depth, 20.0f);

	for (UINT i = 0; i < m; i++) {
		float z = (depth * 0.5f) - (i * dz);

		for (int i2 = 0; i2 < n; i2++) {
			float x = 0.0f;
			x = -(width * 0.5f) + (i2 * dx);
			float y = 0.0f;
			y = heightData[i * n + i2];

			vertices[i * n + i2].Pos = XMFLOAT3(x, y, z);
			vertices[i * n + i2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertices[i * n + i2].TexC.x = i2 * du;
			vertices[i * n + i2].TexC.y = i * dv;
		}
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &vertices[0];

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
HRESULT Terrain::InitialiseIB(ID3D11Device * pd3dDevice, float width, float depth, UINT m, UINT n)
{
	HRESULT hr;

	UINT _faceCount = (m - 1) * (n - 1) * 2;

	std::vector<LONG> indices;
	indices.resize(_faceCount * 3);

	int currentIndex = 0;

	for (int i = 0; i < (m - 1); i++) {
		for (int i2 = 0; i2 < (n - 1); i2++) {
			indices[currentIndex] = i * n + i2;
			indices[currentIndex + 1] = i * n + i2 + 1;
			indices[currentIndex + 2] = (i + 1) * n + i2;
			indices[currentIndex + 3] = (i + 1) * n + i2;
			indices[currentIndex + 4] = i * n + i2 + 1;
			indices[currentIndex + 5] = (i + 1) * n + i2 + 1;

			currentIndex += 6;
		}
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(LONG) * indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &indices[0];

	// Update Index count
	IndexCount = indices.size();

	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &_indexBuffer);

	if (FAILED(hr)) {
		return hr;
	}
	else {
		return S_OK;
	}
}
HRESULT Terrain::InitialiseData(ID3D11Device * pd3dDevice, float width, float depth, UINT m, UINT n)
{
	HRESULT hr;

	UINT vertexCount = m * n;

	float dx = width / (n - 1);
	float dz = depth / (m - 1);

	float du = 50.0f / (n - 1);
	float dv = 50.0f / (m - 1);

	vector<SimpleVertex> vertices;
	vertices.resize(vertexCount);

	// Get heightmap
	std::vector<float> heightData;
	Loader::LoadHeightMap("terrainmap.raw", heightData, width, depth, 20.0f);

	for (UINT i = 0; i < m; i++) {
		float z = (depth * 0.5f) - (i * dz);

		for (int i2 = 0; i2 < n; i2++) {
			float x = 0.0f;
			x = -(width * 0.5f) + (i2 * dx);
			float y = 0.0f;
			y = heightData[i * n + i2];

			vertices[i * n + i2].Pos = XMFLOAT3(x, y, z);
			vertices[i * n + i2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertices[i * n + i2].TexC.x = i2 * du;
			vertices[i * n + i2].TexC.y = i * dv;
		}
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &vertices[0];

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

	UINT _faceCount = (m - 1) * (n - 1) * 2;

	std::vector<LONG> indices;
	indices.resize(_faceCount * 3);

	int currentIndex = 0;

	for (int i = 0; i < (m - 1); i++) {
		for (int i2 = 0; i2 < (n - 1); i2++) {
			indices[currentIndex] = i * n + i2;
			indices[currentIndex + 1] = i * n + i2 + 1;
			indices[currentIndex + 2] = (i + 1) * n + i2;

			XMVECTOR crossVector = XMVector3Cross(XMVectorSet(vertices[indices[currentIndex + 1]].Pos.x,
				vertices[indices[currentIndex + 1]].Pos.y,
				vertices[indices[currentIndex + 1]].Pos.z,
				0.0f),
				XMVectorSet(vertices[indices[currentIndex + 2]].Pos.x,
				vertices[indices[currentIndex + 2]].Pos.y,
				vertices[indices[currentIndex + 2]].Pos.z,
				0.0f));
			vertices[indices[currentIndex]].Normal = XMFLOAT3(XMVectorGetX(crossVector), XMVectorGetY(crossVector), XMVectorGetZ(crossVector));

			indices[currentIndex + 3] = (i + 1) * n + i2;
			indices[currentIndex + 4] = i * n + i2 + 1;
			indices[currentIndex + 5] = (i + 1) * n + i2 + 1;

			crossVector = XMVector3Cross(XMVectorSet(vertices[indices[currentIndex + 4]].Pos.x,
				vertices[indices[currentIndex + 4]].Pos.y,
				vertices[indices[currentIndex + 4]].Pos.z,
				0.0f),
				XMVectorSet(vertices[indices[currentIndex + 5]].Pos.x,
				vertices[indices[currentIndex + 5]].Pos.y,
				vertices[indices[currentIndex + 5]].Pos.z,
				0.0f));
			vertices[indices[currentIndex + 3]].Normal = XMFLOAT3(XMVectorGetX(crossVector), XMVectorGetY(crossVector), XMVectorGetZ(crossVector));

			currentIndex += 6;
		}
	}

	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(LONG) * indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &indices[0];

	// Update Index count
	IndexCount = indices.size();

	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &_indexBuffer);

	if (FAILED(hr)) {
		return hr;
	}
	else {
		return S_OK;
	}
}

void Terrain::Draw(ID3D11Device * pd3dDevice, ID3D11DeviceContext * pImmediateContext)
{
	pImmediateContext->PSSetShaderResources(0, 1, &_pTexture);
	pImmediateContext->IASetVertexBuffers(0, 1, &Terrain::_vertexBuffer, &Terrain::VBStride, &Terrain::VBOffset);
	pImmediateContext->IASetIndexBuffer(Terrain::_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pImmediateContext->DrawIndexed(Terrain::IndexCount, 0, 0);
}