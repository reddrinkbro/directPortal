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
		1,							//�ִ� ��ƼŬ��
		50.0f,						//�ʴ� ��ƼŬ �߻� ��
		0.001f,						//�ϳ��� ��ƼŬ ���� ������ �ּҰ�
		0.001f,						//�ϳ��� ��ƼŬ ���� ������ �ִ밪
		D3DXVECTOR3(0, 0, 0),		//��ƼŬ ���� �ӵ� �ּҰ�(���ñ���)
		D3DXVECTOR3(0, 0, 0),		//��ƼŬ ���� �ӵ� �ִ밪(���ñ���)
		D3DXVECTOR3(0, 0, 0),		//��ƼŬ ���� ���� �ּҰ�(���ñ���)
		D3DXVECTOR3(0, 0, 0),		//��ƼŬ ���� ���� �ִ밪(���ñ���)
		colorsBlue,					//�÷� �迭
		scales,						//������ �迭
		1.0f,						//����ũ�� �ּҰ�
		1.0f,						//����ũ�� �ִ밪
		pTex,						//�ؽ���
		true						//�����̴�? �ƴϴ�?
	);

	VEC_COLOR colorsOrange;
	colorsOrange.push_back(D3DXCOLOR(0.8f, 0.5f, 0.0f, 1.0f));
	colorsOrange.push_back(D3DXCOLOR(0.8f, 0.5f, 0.0f, 1.0f));
	colorsOrange.push_back(D3DXCOLOR(0.8f, 0.5f, 0.0f, 1.0f));

	_orangeBullet->init(
		1,							//�ִ� ��ƼŬ��
		50.0f,						//�ʴ� ��ƼŬ �߻� ��
		0.001f,						//�ϳ��� ��ƼŬ ���� ������ �ּҰ�
		0.001f,						//�ϳ��� ��ƼŬ ���� ������ �ִ밪
		D3DXVECTOR3(0, 0, 0),		//��ƼŬ ���� �ӵ� �ּҰ�(���ñ���)
		D3DXVECTOR3(0, 0, 0),		//��ƼŬ ���� �ӵ� �ִ밪(���ñ���)
		D3DXVECTOR3(0, 0, 0),		//��ƼŬ ���� ���� �ּҰ�(���ñ���)
		D3DXVECTOR3(0, 0, 0),		//��ƼŬ ���� ���� �ִ밪(���ñ���)
		colorsOrange,				//�÷� �迭
		scales,						//������ �迭
		1.0f,						//����ũ�� �ּҰ�
		1.0f,						//����ũ�� �ִ밪
		pTex,						//�ؽ���
		true						//�����̴�? �ƴϴ�?
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
			//�Ѿ��� 0�� �϶�
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

