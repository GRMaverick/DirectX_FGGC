#pragma once
#include "Structs.h"
#include <stdio.h>

class HMTerrain
{
private:
	int							_pTerrainWidth, _pTerrainHeight;
	int							_pVertexCount, _pIndexCount;
	ID3D11Buffer*		_pVertexBuffer, *_pIndexBuffer;

	HeightMapType*	_pHeightMap;


	bool							LoadHeightMap(char* filename);
	bool							InitialiseBuffers(ID3D11Device* pd3dDevice);

	void							NormalizeHeightMap();
	void							ShutdownHeightMap();
	void							ShutdownBuffers();
	void							RenderBuffers(ID3D11DeviceContext* immediateContext);

public:
	HMTerrain();
	~HMTerrain();

	bool							Initialise(ID3D11Device* pd3dDevice, char* filename);

	void							Cleanup();
	void							Draw(ID3D11DeviceContext* immediateContext);

	int							GetIndexCount();
};
