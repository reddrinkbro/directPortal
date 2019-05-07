#include "stdafx.h"
#include "rmSkinnedMesh.h"

HRESULT rmSkinnedMesh::init(void)
{
	return S_OK;
}

void rmSkinnedMesh::release(void)
{
	this->clearResource();
}

xMeshSkinned * rmSkinnedMesh::loadResource(string filePath, void * pParam)
{
	xMeshSkinned* newMesh = new xMeshSkinned;

	if (FAILED(newMesh->init(filePath, (D3DXMATRIXA16*)pParam)))
	{
		SAFE_DELETE(newMesh);
		return NULL;
	}

	return newMesh;
}

void rmSkinnedMesh::releaseResource(xMeshSkinned * data)
{
	data->release();
	SAFE_DELETE(data);
}
