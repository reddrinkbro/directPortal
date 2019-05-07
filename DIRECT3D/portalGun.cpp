#include "stdafx.h"
#include "portalGun.h"
#include "player.h"


HRESULT portalGun::init(float x, float y, float z)
{
	_state = GUN_NONE;
	_color = BULLET_NONE;
	D3DXMATRIXA16 matScale, matRotate, matCombine;
	D3DXMatrixRotationY(&matRotate, D3DXToRadian(180));
	D3DXMatrixScaling(&matScale, 0.035f, 0.035f, 0.035f);
	matCombine = matScale * matRotate;
	xMeshSkinned* mesh = RM_SKINNED->addResource("Resources/Model/portalgun/portalGun.X", matCombine);
	_animation = new skinnedAnimation;
	_animation->init(mesh);
	setMesh(mesh);
	setActive(true);
	_boundBox.setBound(&D3DXVECTOR3(0.2f, -0.2f, 0.62f), &D3DXVECTOR3(0.2f, 0.1f, 0.3f));
	_transform->setWorldPosition(x, y, z);
	this->effectInit();
	_bullet = new portalBullet;
	_bullet->init();
	_transform->addChild(_blueBullet->_transform);
	_transform->addChild(_orangeBullet->_transform);
	_blueBullet->_transform->setLocalPosition(0.1f, -0.15f, 1.0f);
	_orangeBullet->_transform->setLocalPosition(0.1f, -0.15f, 1.0f);
	_idleChange = 0;
	_isHit = false;
	return S_OK;
}

HRESULT portalGun::effectInit(void)
{
	_blueBullet = new particleEmitter;
	_blueBullet->setActive(true);

	_orangeBullet = new particleEmitter;
	_orangeBullet->setActive(true);

	VEC_COLOR colorsBlue;
	colorsBlue.push_back(D3DXCOLOR(0.0f, 0.4f, 1.0f, 1.0f));
	colorsBlue.push_back(D3DXCOLOR(0.0f, 0.4f, 1.0f, 1.0f));
	colorsBlue.push_back(D3DXCOLOR(0.0f, 0.4f, 1.0f, 1.0f));

	VEC_SCALE scales;
	scales.push_back(0.3f);
	scales.push_back(0.3f);
	scales.push_back(0.3f);

	LPDIRECT3DTEXTURE9 pTex = RM_TEXTURE->addResource("Resources/Texture/particle_0.png");
	_blueBullet->init(
		1,							//최대 파티클수
		50.0f,						//초당 파티클 발생 량
		0.001f,						//하나의 파티클 입자 라이프 최소값
		0.001f,						//하나의 파티클 입자 라이프 최대값
		D3DXVECTOR3(0, 0, 0),		//파티클 입자 속도 최소값(로컬기준)
		D3DXVECTOR3(0, 0, 0),		//파티클 입자 속도 최대값(로컬기준)
		D3DXVECTOR3(0, 0, 0),		//파티클 입자 가속 최소값(로컬기준)
		D3DXVECTOR3(0, 0, 0),		//파티클 입자 가속 최대값(로컬기준)
		colorsBlue,					//컬러 배열
		scales,						//스케일 배열
		1.0f,						//입자크기 최소값
		1.0f,						//입자크기 최대값
		pTex,						//텍스쳐
		true						//로컬이니? 아니니?
	);

	VEC_COLOR colorsOrange;
	colorsOrange.push_back(D3DXCOLOR(0.8f, 0.5f, 0.0f, 1.0f));
	colorsOrange.push_back(D3DXCOLOR(0.8f, 0.5f, 0.0f, 1.0f));
	colorsOrange.push_back(D3DXCOLOR(0.8f, 0.5f, 0.0f, 1.0f));

	_orangeBullet->init(
		1,							//최대 파티클수
		50.0f,						//초당 파티클 발생 량
		0.001f,						//하나의 파티클 입자 라이프 최소값
		0.001f,						//하나의 파티클 입자 라이프 최대값
		D3DXVECTOR3(0, 0, 0),		//파티클 입자 속도 최소값(로컬기준)
		D3DXVECTOR3(0, 0, 0),		//파티클 입자 속도 최대값(로컬기준)
		D3DXVECTOR3(0, 0, 0),		//파티클 입자 가속 최소값(로컬기준)
		D3DXVECTOR3(0, 0, 0),		//파티클 입자 가속 최대값(로컬기준)
		colorsOrange,				//컬러 배열
		scales,						//스케일 배열
		1.0f,						//입자크기 최소값
		1.0f,						//입자크기 최대값
		pTex,						//텍스쳐
		true						//로컬이니? 아니니?
	);
	return S_OK;
}

void portalGun::release(void)
{
	SAFE_DELETE(_animation);
	_bullet->release();
	SAFE_DELETE(_bullet);
	_blueBullet->release();
	SAFE_DELETE(_blueBullet);
	_orangeBullet->release();
	SAFE_DELETE(_orangeBullet);
}

void portalGun::update(void)
{
	_bullet->update();
	_blueBullet->update();
	_orangeBullet->update();
	if (_state != GUN_NONE)
	{
		_animation->update();
		if (_state == GUN_IDLE)
		{
			_animation->Play("idle");
			_idleChange = TIMEMANAGER->getWorldTime();
			//총알이 0개 일때
			_blueBullet->stopEmission();
			_orangeBullet->stopEmission();
		}
		else if (_state == GUN_FIRE)
		{
			_animation->PlayOneShotAfterHold("fire");
			if (_idleChange + 0.5f< TIMEMANAGER->getWorldTime())
			{
				_state = GUN_IDLE;
			}
		}
		else  if (_state == GUN_CARRY)
		{
			_animation->PlayOneShotAfterHold("carry");
		}
	}
}

void portalGun::render(void)
{
	_animation->render(_transform);
	_bullet->render();
	_blueBullet->render();
	_orangeBullet->render();
}

