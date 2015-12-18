#include "HMTerrain.h"


HMTerrain::HMTerrain()
{
	_pVertexBuffer = 0;
	_pIndexBuffer = 0;
	_pHeightMap = 0;
}
HMTerrain::~HMTerrain()
{
}

bool HMTerrain::Initialise(ID3D11Device* pd3dDevice, char* filename)
{
	bool result;

	// Load in the height map for the terrain.
	result = LoadHeightMap(filename);

	if (!result)
		return false;

	// Normalise the Height
	int i, j;
	for (j = 0; j<_pTerrainHeight; j++)
	{
		for (i = 0; i<_pTerrainWidth; i++)
		{
			_pHeightMap[(_pTerrainHeight * j) + i].y /= 3.5f;
		}
	}

	// Initialize the vertex and index buffer that hold the geometry for the terrain.
	result = InitialiseBuffers(pd3dDevice);

	if (!result)
		return false;

	return true;
}

void HMTerrain::Cleanup()
{
	if(_pVertexBuffer) _pVertexBuffer->Release();
	if (_pIndexBuffer) _pIndexBuffer->Release();

	delete[] _pHeightMap;
}
void HMTerrain::Draw(ID3D11DeviceContext* immediateContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	immediateContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
	immediateContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	immediateContext->DrawIndexed(_pIndexCount, 0, 0);
}

int HMTerrain::GetIndexCount()
{
	return _pIndexCount;
}

bool HMTerrain::LoadHeightMap(char* filename)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, i, j, k, index;
	unsigned char* bitmapImage;
	unsigned char height;
	//int _pTerrainWidth, _pTerrainHeight;
	//HeightMapType* hmType;
	// Open the height map file in binary.
	error = fopen_s(&filePtr, filename, "rb");

	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Read in the bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}
	// Save the dimensions of the terrain.
	_pTerrainWidth = bitmapInfoHeader.biWidth;
	_pTerrainHeight = bitmapInfoHeader.biHeight;

	// Calculate the size of the bitmap image data.
	imageSize = _pTerrainWidth * _pTerrainHeight * 3;

	// Allocate memory for the bitmap image data.
	bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}
	// Create the structure to hold the height map data.
	_pHeightMap = new HeightMapType[_pTerrainWidth * _pTerrainHeight];

	if (!_pHeightMap)
	{
		return false;
	}

	// Initialize the position in the image data buffer.
	k = 0;

	// Read the image data into the height map.
	for (j = 0; j<_pTerrainHeight; j++)
	{
		for (i = 0; i<_pTerrainWidth; i++)
		{
			height = bitmapImage[k];

			index = (_pTerrainHeight * j) + i;

			_pHeightMap[index].x = (float)i;
			_pHeightMap[index].y = (float)height;
			_pHeightMap[index].z = (float)j;

			k += 3;
		}
	}

	// Release the bitmap image data.
	delete[] bitmapImage;
	bitmapImage = 0;

	return true;
}
bool HMTerrain::InitialiseBuffers(ID3D11Device* pd3dDevice)
{
	VertexType* vertices;
	unsigned long* indices;
	int index, i, j;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int index1, index2, index3, index4;

	// Calculate the number of vertices in the terrain mesh.
	_pVertexCount = (_pTerrainWidth - 1) * (_pTerrainHeight - 1) * 12;

	// Set the index count to the same as the vertex count.
	_pIndexCount = _pVertexCount;

	// Create the vertex array.
	vertices = new VertexType[_pVertexCount];

	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[_pIndexCount];
	if (!indices)
	{
		return false;
	}

	// Initialize the index to the vertex buffer.
	index = 0;

	// Load the vertex and index array with the terrain data.
	for (j = 0; j<(_pTerrainHeight - 1); j++)
	{
		for (i = 0; i<(_pTerrainWidth - 1); i++)
		{
			index1 = (_pTerrainHeight * j) + i;          // Bottom left.
			index2 = (_pTerrainHeight * j) + (i + 1);      // Bottom right.
			index3 = (_pTerrainHeight * (j + 1)) + i;      // Upper left.
			index4 = (_pTerrainHeight * (j + 1)) + (i + 1);  // Upper right.

			// Upper left.
			vertices[index].position = XMFLOAT3(_pHeightMap[index3].x, _pHeightMap[index3].y, _pHeightMap[index3].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Upper right.
			vertices[index].position = XMFLOAT3(_pHeightMap[index4].x, _pHeightMap[index4].y, _pHeightMap[index4].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Upper right.
			vertices[index].position = XMFLOAT3(_pHeightMap[index4].x, _pHeightMap[index4].y, _pHeightMap[index4].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].position = XMFLOAT3(_pHeightMap[index1].x, _pHeightMap[index1].y, _pHeightMap[index1].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].position = XMFLOAT3(_pHeightMap[index1].x, _pHeightMap[index1].y, _pHeightMap[index1].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Upper left.
			vertices[index].position = XMFLOAT3(_pHeightMap[index3].x, _pHeightMap[index3].y, _pHeightMap[index3].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].position = XMFLOAT3(_pHeightMap[index1].x, _pHeightMap[index1].y, _pHeightMap[index1].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Upper right.
			vertices[index].position = XMFLOAT3(_pHeightMap[index4].x, _pHeightMap[index4].y, _pHeightMap[index4].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Upper right.
			vertices[index].position = XMFLOAT3(_pHeightMap[index4].x, _pHeightMap[index4].y, _pHeightMap[index4].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom right.
			vertices[index].position = XMFLOAT3(_pHeightMap[index2].x, _pHeightMap[index2].y, _pHeightMap[index2].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom right.
			vertices[index].position = XMFLOAT3(_pHeightMap[index2].x, _pHeightMap[index2].y, _pHeightMap[index2].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].position = XMFLOAT3(_pHeightMap[index1].x, _pHeightMap[index1].y, _pHeightMap[index1].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;
		}
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* _pVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = pd3dDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &_pVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* _pIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = pd3dDevice->CreateBuffer(&indexBufferDesc, &indexData, &_pIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}
