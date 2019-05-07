#include "stdafx.h"
#include "portalBullet.h"

HRESULT portalBullet::init()
{
	return S_OK;
}

void portalBullet::fire(D3DXVECTOR3 pos, float angleX, float angleY)
{
	//총알의 개수가 1보다 크면 리턴
	if (_vBullet.size() > 0) return;
	bullet _bullet;
	ZeroMemory(&_bullet, sizeof(bullet));
	_bullet.object = new baseObject;
	_bullet.object->_transform->setWorldPosition(D3DXVECTOR3(pos.x, pos.y + 2, pos.z));
	_bullet.object->_transform->setRotateWorld(angleX, angleY, 0);
	_bullet.object->_boundBox.setBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0.1f, 0.1f, 0.1f));
	_bullet.speed = 30.0f * _deltaTime;
	_vBullet.push_back(_bullet);
}

void portalBullet::release(void)
{

}

void portalBullet::update(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->object->_transform->movePositionSelf(0, 0, _viBullet->speed);
	}
}

void portalBullet::render(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->object->_transform->renderGizmo(0.1f);
	}
}

void portalBullet::removeBullet(int num)
{
	_vBullet.erase(_vBullet.begin() + num);
}
