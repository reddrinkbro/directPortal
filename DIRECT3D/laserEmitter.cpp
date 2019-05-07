#include "stdafx.h"
#include "laserEmitter.h"

HRESULT laserEmitter::init(float x, float y, float z)
{
	D3DXMATRIXA16  matScale;
	D3DXMatrixScaling(&matScale, 0.03f, 0.03f, 0.03f);

	setMesh(RM_XMESH->addResource("Resources/Model/laser_emitter/laser_emitter.X", matScale));
	setActive(true);
	_transform->setWorldPosition(x, y, z);
	return S_OK;
}

void laserEmitter::release(void)
{
}

void laserEmitter::render(void)
{
	baseObject::render();
}
