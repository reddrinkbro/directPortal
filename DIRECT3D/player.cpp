#include "stdafx.h"
#include "player.h"
#include "portalGun.h"
#include "cube.h"
#include "button.h"
#include "switchButton.h"

HRESULT player::init(float x, float y, float z)
{
	_transform->setWorldPosition(x, y, z);
	_boundBox.setBound(&D3DXVECTOR3(0, 1, 0), &D3DXVECTOR3(0.4f, 1.0f, 0.4f));
	_menu = new menuScene;
	_menu->init();
	_isJump = false;
	_crosshair = new crosshair;
	_crosshair->init();
	return S_OK;
}

void player::release(void)
{
	_menu->release();
	SAFE_DELETE(_menu);
	_crosshair->release();
	SAFE_DELETE(_crosshair);
}

void player::update(float x, float y)
{
	if (!_menu->getIsOn())
	{
		this->control(x, y);
		_transform->movePositionSelf(0, -_gravity, 0);
		_transform->setRotateWorld(0, y, 0);
		if (!_isEquip)
		{
			//포탈건 바운드 박스 충돌
			if (PHYSICSMANAGER->isOverlap(_transform, &_boundBox, _gun->_transform, &_gun->_boundBox))
			{
				this->playerPortalGunCollision();
			}
		}
		else
		{
			_gun->_transform->setLocalPosition(0, 2, -0.3f);
		}
	}
	_menu->update();
}

void player::render(void)
{
	_menu->render();
	_crosshair->render();
}

void player::control(float x, float y)
{
	//왼쪽으로 움직일때
	if (KEYMANAGER->isStayKeyDown('A'))
	{
		float moveSpeed = -_speed * _deltaTime;
		_transform->movePositionSelf(moveSpeed, 0, 0);
	}
	//오른쪽으로 움직일때
	if (KEYMANAGER->isStayKeyDown('D'))
	{
		float moveSpeed = _speed * _deltaTime;
		_transform->movePositionSelf(moveSpeed, 0, 0);
	}
	//위로 움직일때
	if (KEYMANAGER->isStayKeyDown('W'))
	{
		float moveSpeed = _speed * _deltaTime;
		_transform->movePositionSelf(0, 0, moveSpeed);
	}
	//아래로 움직일때
	if (KEYMANAGER->isStayKeyDown('S'))
	{
		float moveSpeed = -_speed * _deltaTime;
		_transform->movePositionSelf(0, 0, moveSpeed);
	}

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		_isJump = true;
	}
	if (KEYMANAGER->isOnceKeyDown('E'))
	{
		if (_isCollide != C_SWITCH)
		{
			if (!_isCarry) _isCarry = true;
			else _isCarry = false;
			if (_isEquip)
			{
				if (_isCarry) _gun->setState(GUN_CARRY);
				else _gun->setState(GUN_IDLE);
			}
		}
	}
	if (KEYMANAGER->isOnceKeyDown(VK_ESCAPE))
	{
		_menu->setIsOn(true);
		ShowCursor(true);
	}
	if (_isEquip)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			if (_gun->getState() != GUN_FIRE) SOUNDMANAGER->play("portalgun_shoot_blue", 0.5f);
			_gun->setState(GUN_FIRE);
			_gun->getBlueParticle()->startEmission();
			_gun->getBullet()->fire(_transform->getWorldPosition(), x, y);
			_gun->setColor(BULLET_BLUE);
			_gun->setIsHit(false);
		}
		if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
		{
			if (_gun->getState() != GUN_FIRE) SOUNDMANAGER->play("portalgun_shoot_orange", 0.5f);
			_gun->setState(GUN_FIRE);
			_gun->getOrangeParticle()->startEmission();
			_gun->getBullet()->fire(_transform->getWorldPosition(), x, y);
			_gun->setColor(BULLET_ORANGE);
			_gun->setIsHit(false);
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		_transform->movePositionSelf(0, 0.1f, 0);
	}
	this->jump();	
}

void player::playerPortalGunCollision(void)
{
	_isEquip = true;
	_gun->_transform->setRotateWorld(_transform->getWorldRotateMatrix());
	_transform->addChild(_gun->_transform);
	_gun->_transform->setLocalPosition(0, 2.0f, -0.3f);
	if (_gun->getState() != GUN_IDLE) SOUNDMANAGER->play("gun_take", 1.0f);
	_gun->setState(GUN_IDLE);
}

void player::cubeCollision(cube* cube)
{
	setCubeMemoryLink(cube);
	_transform->addChild(_cube->_transform);
	_cube->setGravity(0.2f);
	_cube->setCollisionType(COLLISIONTYPE_PLAYER);
	_isCollide = C_CUBE;
	SOUNDMANAGER->stop("box_lay");
}

void player::jump(void)
{
	if (_isJump)
	{
		if (_jumpTime + 0.35f > TIMEMANAGER->getWorldTime())
		{
			_gravity = 0;
			D3DXVECTOR3 jumpVec(0, 1, 0);
			D3DXVECTOR3 target = jumpVec * _jumpMaxForce;
			D3DXVec3TransformNormal(&target, &target, &this->_transform->getFinalMatrix());

			// 타겟벡터와 현제Velocity 크기차
			float targetLength = D3DXVec3Length(&target); // D3DXVec3Length 벡터의 크기를 얻는다.
			float dist = abs(targetLength - 0.01f);

			// 타겟과 차이가 존재한다면..
			if (FLOATZERO(dist) == false)
			{
				// 변화 허용량
				float delta = _jumpAccelate * _deltaTime;
				// 보간값
				float t = clamp01(delta / dist);

				// 이번프레임에 타겟까지 변화 하는데 충분하다
				if (FLOATEQUAL(t, 1.0f))
				{
					_jumpVector = target;
				}
				else
				{
					_jumpVector = vecLerp(_jumpVector, target, t);
				}
			}

			// 이동 벡터는
			D3DXVECTOR3 moveDeltaVector = _jumpVector * _deltaTime;
			// 이동 해라
			_transform->movePositionWorld(moveDeltaVector);
		}
		else
		{
			_isJump = false;
		}
	}
	else
	{
		_jumpTime = TIMEMANAGER->getWorldTime();
		if (!_isUpStairs) _gravity = 0.2;
	}
}


player::player()
{
	_isCollide = C_NONE;
	_isEquip = false;
	_isUpStairs = false;
	_isCarry = false;
	_speed = 3.0f;
	_gravity = 0.2f;
	_jumpTime = 0;
	_jumpMaxForce = 3;
	_jumpAccelate = 30;
	_jumpVector = D3DXVECTOR3(0, 0, 0);
}


player::~player()
{
}
