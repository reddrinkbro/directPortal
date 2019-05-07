#include "stdafx.h"
#include "laserPoint.h"

HRESULT laserPoint::init(float x, float y, float z)
{
	_transform->setWorldPosition(x, y, z);
	for (int i = 0; i < 6; i++)
	{
		_laser[i] = new laser;
		_laser[i]->init(x, y, z);
		_laser[i]->_transform.setRotateWorld(0, D3DXToRadian(60 * i), 0);
		_transform->addChild(&_laser[i]->_transform);
	}
	_boundBox.setBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0.5f, 1.2f, 0.5f));
	_isSet = false;
	return S_OK;
}

void laserPoint::release(void)
{
	for (int i = 0; i < 6; i++)
	{
		_laser[i]->release();
		SAFE_DELETE(_laser[i]);
	}
}

void laserPoint::render(void)
{
	if (_isSet)
	{
		for (int i = 0; i < 6; i++)
		{
			_laser[i]->render();
		}
	}
}


void laserPoint::setLaser(D3DXVECTOR3 pos, BOUNDBOXDIR boxDir, int scale)
{
	_laserBoxDir = boxDir;
	//문 오른쪽
	switch (_laserBoxDir)
	{
	case BOUNDBOXDIR_UP:
	{
		if (scale > 3) break;
		_transform->lookDirection(D3DXVECTOR3(0, 0, 1));
		_transform->setScale(0.04f, scale - 0.3f, 0.04f);
		_transform->setWorldPosition(pos.x, pos.y + scale + 0.3f, pos.z);
		break;
	}
	case BOUNDBOXDIR_DOWN:
	{
		if (scale > 3) break;
		_transform->lookDirection(D3DXVECTOR3(0, 0, 1));
		_transform->setScale(0.04f, scale - 0.3f, 0.04f);
		_transform->setWorldPosition(pos.x, pos.y - scale - 0.3f, pos.z);
		break;
	}
	case BOUNDBOXDIR_FORWARD:
	{
		_transform->lookDirection(D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(1, 0, 0));
		_transform->setScale(0.04f, scale - 0.3f, 0.04f);
		_transform->setWorldPosition(pos.x - scale - 0.8f, pos.y, pos.z);
		break;
	}
	case BOUNDBOXDIR_BACK:
	{
		_transform->lookDirection(D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(1, 0, 0));
		_transform->setScale(0.04f, scale - 0.6f, 0.04f);
		_transform->setWorldPosition(pos.x + scale, pos.y, pos.z);
		break;
	}
	case BOUNDBOXDIR_LEFT:
	{
		_transform->lookDirection(D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 0, 1));
		_transform->setScale(0.04f, scale - 0.3f, 0.04f);
		_transform->setWorldPosition(pos.x, pos.y, pos.z - scale - 0.8f);
		break;
	}
	case BOUNDBOXDIR_RIGHT:
	{
		_transform->lookDirection(D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 0, 1));
		_transform->setScale(0.04f, scale - 0.3f, 0.04f);
		_transform->setWorldPosition(pos.x, pos.y, pos.z + scale + 1.2f);
		break;
	}
	}
}
