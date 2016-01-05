#pragma once
#include "Structs.h"
#include "GameObject.h"
#include "Camera.h"

class Bilboard : public GameObject
{
protected:
	float height;
	float width;

	HRESULT initVertexData(ID3D11Device * pd3dDevice, float height, float width);
	HRESULT initIndexData(ID3D11Device * pd3dDevice);
public:
	ID3D11Buffer*	_vertexBuffer;
	ID3D11Buffer*	_indexBuffer;
	static UINT	VBOffset;
	static UINT	VBStride;
	static UINT	IndexCount;

	Bilboard();
	~Bilboard();
	HRESULT Update(float elapsedTime, ID3D11Device * pd3dDevice, Camera currCamera);

	HRESULT Initialise(ID3D11Device * pd3dDevice, float height, float width);
	void Draw(ID3D11Device * pd3dDevice, ID3D11DeviceContext * pImmediateContext);
};

