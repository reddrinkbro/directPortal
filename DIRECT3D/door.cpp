#include "stdafx.h"
#include "door.h"


HRESULT door::init(float x, float y, float z)
{
	_state = DOOR_NONE;
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.026f, 0.026f, 0.026f);
	xMeshSkinned* mesh = RM_SKINNED->addResource("Resources/Model/door/door.X", matScale);
	_animation = new skinnedAnimation;
	_animation->init(mesh);
	setMesh(mesh);
	setActive(true);
	_transform->setWorldPosition(x, y, z);
	_boundBox.setBound(&D3DXVECTOR3(0, 1.5f, -0.3f), &D3DXVECTOR3(1.5f, 1.5f, 0.15f));
	_closeTrigger.setBound(&D3DXVECTOR3(0, 1.5f, 1.0f), &D3DXVECTOR3(0.8f, 0.5f, 0.3f));
	_openTrigger.setBound(&D3DXVECTOR3(0, 1.5f, -2.0f), &D3DXVECTOR3(0.8f, 0.5f, 0.1f));
	return S_OK;
}

void door::release(void)
{
	SAFE_DELETE(_animation);
}

void door::update(void)
{
	if (_state != DOOR_NONE)
	{
		_animation->update();
		if (_state == DOOR_OPEN)
		{
			_animation->PlayOneShotAfterHold("open");
		}
		if (_state == DOOR_CLOSE)
		{
			_animation->PlayOneShotAfterHold("close");
		}
	}
}

void door::render(void)
{
	_animation->render(_transform);
}