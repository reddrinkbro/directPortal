#include "stdafx.h"
#include "tutorialScene_1.h"

HRESULT tutorialScene_1::init(void)
{
	this->save();
	ShowCursor(FALSE);
	_beforeScene = RM_TEXTURE->addResource("Resources/Texture/loadingscreenAfter.png");
	_afterScene = RM_TEXTURE->addResource("Resources/Texture/loadingscreenBefore.png");

	_endRotateAngle = 0.0f;
	_endCount = 0;
	_startCount = 0;
	_sceneStart = false;
	_sceneEnd = false;
	//기본 값
	//기본 광원 생성
	_sceneBaseDirectionLight->_color = D3DXCOLOR(1, 1, 1, 1);
	_sceneBaseDirectionLight->_intensity = 2.0f;
	//맵초기화
	_map = new tutorialMap;
	_map->init();
	//스위치 초기화
	for (int i = 0; i < 3; i++)
	{
		_switch[i] = new switchButton;
	}
	_switch[0]->init(-1, 0.17f, 0);
	_switch[0]->_transform->setRotateWorld(0, D3DXToRadian(180), 0);
	_switch[1]->init(4, 0.17f, 4);
	_switch[1]->_transform->setRotateWorld(0, D3DXToRadian(90), 0);
	_switch[2]->init(0, 0.17f, 28);
	//큐브초기화
	_cube = new cube;
	_cube->init(-3, 0.8f, -5.5f);
	//버튼 초기화
	for (int i = 0; i < 2; i++)
	{
		_button[i] = new button;
	}
	_button[0]->init(7, 0.2f, 0);
	_button[1]->init(-8.5f, 0.2f, 22);
	//박스 드랍 초기화
	_dropper = new boxDropper;
	_dropper->init(-9, 4, 34);
	//엘레베이터 초기화
	_elevator = new elevator;
	_elevator->init(-5.8f, 1.1f, 47.5f);
	//문 초기화
	_door = new door;
	_door->init(-6, 3.3f, 39.6f);
	//포탈건 초기화
	_gun = new portalGun;
	_gun->init(5, 4.1f, 34);
	//플레이어 초기화
	_player = new player;
	_player->init(-12.5f, 0, 1.5f);
	_player->setPortalGunMemoryLink(_gun);
	_player->_transform->addChild(_mainCamera);
	_mainCamera->setLocalPosition(0, 2, 0);
	_mainCamera->setIsMouse(true);
	//파랑 포탈문 초기화
	_bPortal = new bluePortal;
	_bPortal->init();
	_bPortal->_transform->setWorldPosition(1, 1.55f, -6.4f);
	_bPortal->_transform->lookDirection(D3DXVECTOR3(0, 0, -1));
	_bPortal->setIsSet(true);
	//주황 포탈문 초기화
	_oPortal = new orangePortal;
	_oPortal->init();
	_oPortal->_transform->setWorldPosition(-6, 1.55f, -6.4f);
	_oPortal->_transform->lookDirection(D3DXVECTOR3(0, 0, -1));
	_oPortal->setIsSet(true);
	return S_OK;
}

void tutorialScene_1::release(void)
{
	_map->release();
	SAFE_DELETE(_map);
	for (int i = 0; i < 3; i++)
	{
		_switch[i]->release();
		SAFE_DELETE(_switch[i]);
	}
	_cube->release();
	SAFE_DELETE(_cube);
	for (int i = 0; i < 2; i++)
	{
		_button[i]->release();
		SAFE_DELETE(_button[i]);
	}
	_dropper->release();
	SAFE_DELETE(_dropper);
	_elevator->release();
	SAFE_DELETE(_dropper);
	_door->release();
	SAFE_DELETE(_door);
	_gun->release();
	SAFE_DELETE(_gun);
	_player->release();
	SAFE_DELETE(_player);
	_bPortal->release();
	SAFE_DELETE(_bPortal);
	_oPortal->release();
	SAFE_DELETE(_oPortal);
}

void tutorialScene_1::update(void)
{
	if (_sceneStart)
	{
		_mainCamera->defaultControl(!_player->getMenuScene()->getIsOn());
		_mainCamera->setIsMouse(!_player->getMenuScene()->getIsOn());
		_dropper->update();
		_elevator->update();
		_door->update();
		for (int i = 0; i < 2; i++)
		{
			_button[i]->update();
		}
		for (int i = 0; i < 3; i++)
		{
			_switch[i]->update();
		}
		_player->update(D3DXToRadian(_mainCamera->getNowAngleV()), D3DXToRadian(_mainCamera->getNowAngleH()));
		if (_player->getIsEquip())
		{
			_gun->update();
			_gun->_transform->setRotateWorld(D3DXToRadian(_mainCamera->getNowAngleV()), D3DXToRadian(_mainCamera->getNowAngleH()), 0);
		}
		_cube->update();
		this->tutorial1Collision();
		this->nextScene();
	}
	else
	{
		_startCount++;
		if (_startCount % 200 == 0)
		{
			_sceneStart = true;
		}
	}
}

void tutorialScene_1::render(void)
{
	if (_sceneStart)
	{
		//static render
		_bPortal->render();
		_oPortal->render();
		xMeshStatic::setCamera(_mainCamera);
		_map->render();
		_cube->render();
		//skinned render
		xMeshSkinned::setTechniqueName("Toon");
		xMeshSkinned::setBaseLight(_sceneBaseDirectionLight);
		xMeshSkinned::setCamera(_mainCamera);
		_dropper->render();
		_elevator->render();
		_door->render();
		
		for (int i = 0; i < 2; i++)
		{
			_button[i]->render();
		}
		for (int i = 0; i < 3; i++)
		{
			_switch[i]->render();
		}
		_gun->render();
		_player->render();
	}
	else if (_sceneEnd)
	{
		RECT rChange = { -100, 0, WINSIZEX, WINSIZEY + 100 };
		SPRITEMANAGER->spriteRender(
			_afterScene, &rChange, 0, -100, 1, 1, 0);
		_endCount++;
		if (_endCount % 60 == 39)
		{
			SOUNDMANAGER->stop("elevator_door_close");
			SCENEMANAGER->changeScene("튜토리얼2");
		}
	}
	else
	{
		RECT rChange = { -100, 0, WINSIZEX, WINSIZEY + 100 };
		SPRITEMANAGER->spriteRender(
			_beforeScene, &rChange, 0, -100, 1, 1, 0);
	}
}

void tutorialScene_1::clickSwitch(int num)
{
	_switch[num]->setState(SWITCH_ON);
	switchOn();
}

void tutorialScene_1::switchOn(void)
{
	if (_switch[2]->getState() == SWITCH_ON)
	{
		_dropper->setState(DROPPER_OPEN);
		_cube->_transform->setWorldPosition(-9, 5, 34);
	}
}

void tutorialScene_1::nextScene(void)
{
	if (PHYSICSMANAGER->isOverlap(_player, _elevator))
	{
		SOUNDMANAGER->stop("door_close");
		if (_elevator->_transform->getWorldPosition().z < _player->_transform->getWorldPosition().z)
		{
			if (_elevator->getState() != ELEVATOR_LEAVE) SOUNDMANAGER->play("elevator_door_close", 0.5f);
			_elevator->setState(ELEVATOR_LEAVE);
		}
		if (_elevator->getState() == ELEVATOR_LEAVE)
		{
			if (PHYSICSMANAGER->isOverlap(_elevator->_transform, &_elevator->_boundBox, _player->_transform, &_player->_boundBox))
			{
				PHYSICSMANAGER->isBlocking(_elevator->_transform, &_elevator->getCloseBox(), _player->_transform, &_player->_boundBox);
			}
			_endCount++;
			if (_endCount % 60 == 0)
			{
				_sceneEnd = true;
				_sceneStart = false;
			}
		}
	}
}

void tutorialScene_1::save(void)
{
	FILE* file;
	char name[] = "튜토리얼1";
	file = fopen("save/save.txt", "w");
	fprintf(file, "%s", &name);
	fclose(file);
}
