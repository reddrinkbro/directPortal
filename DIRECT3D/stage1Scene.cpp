#include "stdafx.h"
#include "stage1Scene.h"

HRESULT stage1Scene::init(void)
{
	this->save();
	ShowCursor(FALSE);
	_beforeScene = RM_TEXTURE->addResource("Resources/Texture/loadingscreenAfter.png");
	_afterScene = RM_TEXTURE->addResource("Resources/Texture/loadingscreenBefore.png");

	_endRotateAngle = 45.0f;
	_endCount = 0;
	_startCount = 0;
	_sceneStart = false;
	_sceneEnd = false;
	//기본 값
	//기본 광원 생성
	_sceneBaseDirectionLight->_color = D3DXCOLOR(1, 1, 1, 1);
	_sceneBaseDirectionLight->_intensity = 2.0f;
	//맵초기화
	_map = new stage1;
	_map->init();
	//플레이어 초기화
	_player = new player;
	_player->init(0.062f, 1.7f, 20.25f);
	_player->_transform->addChild(_mainCamera);
	_player->setIsEquip(true);
	_mainCamera->setLocalPosition(0, 2, 0);
	_mainCamera->setIsMouse(true);
	//큐브 초기화
	_cube = new cube;
	_cube->init(0, 0, -25.25f);
	//버튼 초기화
	_button = new button;
	_button->init(-11.25, 0.125f, -28.75);
	//엘리베이터 ,문 초기화
	for (int i = 0; i < 2; i++)
	{
		_elevator[i] = new elevator;
		_door[i] = new door;
	}
	_elevator[0]->init(0.062f, -1.875f, 20.25f);
	_elevator[0]->setState(ELEVATOR_WAIT);
	_elevator[1]->init(-0.062f, -1.875f, -41.75f);
	_elevator[1]->_transform->setRotateWorld(0, D3DXToRadian(180), 0);
	_door[0]->init(0, 0, -1.437f);
	_door[0]->_transform->setScale(1.25, 1.25, 1.625f);
	_door[0]->_transform->setRotateWorld(0, D3DXToRadian(180), 0);
	_door[1]->init(0, 0, -34.125f);
	_door[1]->_transform->setScale(1.25, 1.25, 1.625f);
	_door[1]->_transform->setRotateWorld(0, D3DXToRadian(180), 0);

	//포탈건 초기화
	_gun = new portalGun;
	_gun->init(_player->_transform->getWorldPosition().x, _player->_transform->getWorldPosition().y + 2.0f, _player->_transform->getWorldPosition().z - 0.3f);
	_player->_transform->addChild(_gun->_transform);
	_player->setPortalGunMemoryLink(_gun);
	_player->getCrosshair()->setState(CROSSHAIRSTATE_ALL);
	//포탈 초기화
	_bPortal = new bluePortal;
	_bPortal->init();
	_bPortal->_transform->setWorldPosition(-5, 0, 0);
	_oPortal = new orangePortal;
	_oPortal->init();
	_oPortal->_transform->setWorldPosition(-5, 0, 0);

	return S_OK;
}

void stage1Scene::release(void)
{
	_map->release();
	SAFE_DELETE(_map);
	_player->release();
	SAFE_DELETE(_player);
	_gun->release();
	SAFE_DELETE(_gun);
	for (int i = 0; i < 2; i++)
	{
		_elevator[i]->release();
		SAFE_DELETE(_elevator[i]);
		_door[i]->release();
		SAFE_DELETE(_door[i]);
	}
	_cube->release();
	SAFE_DELETE(_cube);
	_button->release();
	SAFE_DELETE(_button);
	_bPortal->release();
	SAFE_DELETE(_bPortal);
	_oPortal->release();
	SAFE_DELETE(_oPortal);
}

void stage1Scene::update(void)
{
	if (_sceneStart)
	{
		_mainCamera->defaultControl(!_player->getMenuScene()->getIsOn());
		_mainCamera->setIsMouse(!_player->getMenuScene()->getIsOn());
		_player->update(D3DXToRadian(_mainCamera->getNowAngleV()), D3DXToRadian(_mainCamera->getNowAngleH()));
		_gun->update();
		_gun->_transform->setRotateWorld(D3DXToRadian(_mainCamera->getNowAngleV()), D3DXToRadian(_mainCamera->getNowAngleH()), 0);
		_cube->update();
		_button->update();
		for (int i = 0; i < 2; i++)
		{
			_elevator[i]->update();
			_door[i]->update();
		}
		_bPortal->update();
		_oPortal->update();
		this->stage1Collision();
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

void stage1Scene::render(void)
{
	if (_sceneStart)
	{
		xMeshStatic::setCamera(_mainCamera);
		_map->render();
		_cube->render();
		xMeshSkinned::setTechniqueName("Toon");
		xMeshSkinned::setBaseLight(_sceneBaseDirectionLight);
		xMeshSkinned::setCamera(_mainCamera);

		_gun->render();
		_button->render();
		for (int i = 0; i < 2; i++)
		{
			_elevator[i]->render();
			_door[i]->render();
		}
		_bPortal->render();
		_oPortal->render();
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
			SCENEMANAGER->changeScene("스테이지2");
		}
	}
	else
	{
		RECT rChange = { -100, 0, WINSIZEX, WINSIZEY + 100 };
		SPRITEMANAGER->spriteRender(
			_beforeScene, &rChange, 0, -100, 1, 1, 0);
	}
}

void stage1Scene::nextScene(void)
{
	if (PHYSICSMANAGER->isOverlap(_player, _elevator[1]))
	{
		SOUNDMANAGER->stop("door_close");
		if (_elevator[1]->_transform->getWorldPosition().z > _player->_transform->getWorldPosition().z)
		{
			if (_elevator[1]->getState() != ELEVATOR_LEAVE) SOUNDMANAGER->play("elevator_door_close", 0.5f);
			_elevator[1]->setState(ELEVATOR_LEAVE);
		}
		if (_elevator[1]->getState() == ELEVATOR_LEAVE)
		{
			if (PHYSICSMANAGER->isOverlap(_elevator[1]->_transform, &_elevator[1]->_boundBox, _player->_transform, &_player->_boundBox))
			{
				PHYSICSMANAGER->isBlocking(_elevator[1]->_transform, &_elevator[1]->getCloseBox(), _player->_transform, &_player->_boundBox);
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

void stage1Scene::save(void)
{
	FILE* file;
	char name[] = "스테이지1";
	file = fopen("save/save.txt", "w");
	fprintf(file, "%s", &name);
	fclose(file);
}
