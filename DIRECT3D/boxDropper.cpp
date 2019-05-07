#include "stdafx.h"
#include "boxDropper.h"


HRESULT boxDropper::init(float x, float y, float z)
{
	_state = DROPPER_NONE;
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.02f, 0.02f, 0.02f);
	xMeshSkinned* mesh = RM_SKINNED->addResource("Resources/Model/box_dropper/box_dropper.X", matScale);
	_animation = new skinnedAnimation;
	_animation->init(mesh);
	setMesh(mesh);
	setActive(true);
	_boundBox.setBound(&D3DXVECTOR3(0, 1, 0), &D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	_transform->setWorldPosition(x, y, z);
	_closeTime = 0;
	return S_OK;
}

void boxDropper::release(void)
{
	SAFE_DELETE(_animation);
}

void boxDropper::update(void)
{
	
	if (_state != DROPPER_NONE)
	{
		_animation->update();
		_animation->SetPlaySpeed(1.2f);
		if (_state == DROPPER_OPEN)
		{
			_animation->PlayOneShotAfterHold("open");
			if (_closeTime + 1.5f< TIMEMANAGER->getWorldTime())
			{
				_state = DROPPER_CLOSE;
			}
		}
		if (_state == DROPPER_CLOSE)
		{
			_animation->PlayOneShotAfterHold("close");
		}
	}
	else
	{
		_closeTime = TIMEMANAGER->getWorldTime();
	}
}

void boxDropper::render(void)
{
	_animation->render(_transform);
}

