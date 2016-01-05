#pragma once
#include "Structs.h"
#include "Loader.h"
#include "GameObject.h"

using namespace std;

class Terrain: public GameObject
{
protected:
	static HRESULT InitialiseVB(ID3D11Device * pd3dDevice, float width, float depth, UINT m, UINT n);
	static HRESULT InitialiseIB(ID3D11Device * pd3dDevice, float width, float depth, UINT m, UINT n);
	static HRESULT InitialiseData(ID3D11Device * pd3dDevice, float width, float depth, UINT m, UINT n);
public:
	ID3D11ShaderResourceView* _pTexture;

	static ID3D11Buffer*	_vertexBuffer;
	static ID3D11Buffer*	_indexBuffer;
	static UINT	VBOffset;
	static UINT	VBStride;
	static UINT	IndexCount;

	static float _width;
	static float _depth;
	static float _vertexCountX;
	static float _vertexCountZ;

	Terrain();
	~Terrain();
		
	HRESULT Terrain::Initialise(ID3D11Device * pd3dDevice, ID3D11ShaderResourceView* texture, float width, float depth, int verticesCountX, int verticesCountZ);
	void Draw(ID3D11Device * pd3dDevice, ID3D11DeviceContext * pImmediateContext);
};

