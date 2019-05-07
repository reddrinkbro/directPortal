#include "stdafx.h"
#include "stage3Scene.h"


void stage3Scene::stage3Collision(void)
{
	for (int i = 0; i < 2; i++)
	{
		if (PHYSICSMANAGER->isOverlap(_player, _laser[i]))
		{
			this->playerLaserCollision();
			break;
		}
		else
		{
			_laserCollideTime = 0;
		}
	}
	//주황포탈이랑 플레이어 충돌
	if (PHYSICSMANAGER->isOverlap(_oPortal->_transform, &_oPortal->_boundBox, _player->_transform, &_player->_boundBox) && _bPortal->getIsSet())
	{
		_player->_transform->setWorldPosition(_bPortal->getPlayerPos().getWorldPosition());
		SOUNDMANAGER->play("portal_exit", 1.0f);
		if (SOUNDMANAGER->isPlaySound("portal_enter")) SOUNDMANAGER->stop("portal_enter");
	}
	//파랑포탈이랑 플레이어 충돌
	else if (PHYSICSMANAGER->isOverlap(_bPortal->_transform, &_bPortal->_boundBox, _player->_transform, &_player->_boundBox) && _oPortal->getIsSet())
	{
		_player->_transform->setWorldPosition(_oPortal->getPlayerPos().getWorldPosition());
		SOUNDMANAGER->play("portal_enter", 1.0f);
		if (SOUNDMANAGER->isPlaySound("portal_exit")) SOUNDMANAGER->stop("portal_exit");
	}
	//주황포탈이랑 큐브 충돌
	if (PHYSICSMANAGER->isOverlap(_oPortal->_transform, &_oPortal->_boundBox, _cube->_transform, &_cube->_boundBox) && _player->getIsCollide() != C_CUBE && _bPortal->getIsSet())
	{
		_cube->_transform->setWorldPosition(_bPortal->getPlayerPos().getWorldPosition());
		SOUNDMANAGER->play("portal_exit", 1.0f);
		if (SOUNDMANAGER->isPlaySound("portal_enter")) SOUNDMANAGER->stop("portal_enter");
	}
	//파랑포탈이랑 큐브 충돌
	else if (PHYSICSMANAGER->isOverlap(_bPortal->_transform, &_bPortal->_boundBox, _cube->_transform, &_cube->_boundBox) && _player->getIsCollide() != C_CUBE && _oPortal->getIsSet())
	{
		_cube->_transform->setWorldPosition(_oPortal->getPlayerPos().getWorldPosition());
		SOUNDMANAGER->play("portal_enter", 1.0f);
		if (SOUNDMANAGER->isPlaySound("portal_exit")) SOUNDMANAGER->stop("portal_exit");
	}
	//총알 오브젝트 충돌
	for (int i = 0; i < _gun->getBullet()->getBulletObejct().size(); i++)
	{
		if (PHYSICSMANAGER->isOverlap(_button->_transform, &_button->_boundBox,
			_gun->getBullet()->getBulletObejct()[i].object->_transform, &_gun->getBullet()->getBulletObejct()[i].object->_boundBox) &&
			_gun->getBullet()->getBulletObejct().size() > 0)
		{
			_gun->getBullet()->removeBullet(i);
			break;
		}
		else if (PHYSICSMANAGER->isOverlap(_cube->_transform, &_cube->_boundBox,
			_gun->getBullet()->getBulletObejct()[i].object->_transform, &_gun->getBullet()->getBulletObejct()[i].object->_boundBox) &&
			_gun->getBullet()->getBulletObejct().size() > 0)
		{
			_gun->getBullet()->removeBullet(i);
			break;
		}
		else if (PHYSICSMANAGER->isOverlap(_catcher->_transform, &_catcher->_boundBox,
			_gun->getBullet()->getBulletObejct()[i].object->_transform, &_gun->getBullet()->getBulletObejct()[i].object->_boundBox) &&
			_gun->getBullet()->getBulletObejct().size() > 0)
		{
			_gun->getBullet()->removeBullet(i);
			break;
		}
		else if (PHYSICSMANAGER->isOverlap(_emitter->_transform, &_emitter->_boundBox,
			_gun->getBullet()->getBulletObejct()[i].object->_transform, &_gun->getBullet()->getBulletObejct()[i].object->_boundBox) &&
			_gun->getBullet()->getBulletObejct().size() > 0)
		{
			_gun->getBullet()->removeBullet(i);
			break;
		}
		else if (PHYSICSMANAGER->isOverlap(_oPortal->_transform, &_oPortal->_boundBox,
			_gun->getBullet()->getBulletObejct()[i].object->_transform, &_gun->getBullet()->getBulletObejct()[i].object->_boundBox) &&
			_gun->getBullet()->getBulletObejct().size() > 0)
		{
			_gun->getBullet()->removeBullet(i);
			break;
		}
		else if (PHYSICSMANAGER->isOverlap(_bPortal->_transform, &_bPortal->_boundBox,
			_gun->getBullet()->getBulletObejct()[i].object->_transform, &_gun->getBullet()->getBulletObejct()[i].object->_boundBox) &&
			_gun->getBullet()->getBulletObejct().size() > 0)
		{
			_gun->getBullet()->removeBullet(i);
			break;
		}
		for (int j = 0; j < 2; j++)
		{
			if (_gun->getBullet()->getBulletObejct().size() <= 0) break;
			else if (PHYSICSMANAGER->isOverlap(_elevator[i]->_transform, &_elevator[i]->_boundBox,
				_gun->getBullet()->getBulletObejct()[i].object->_transform, &_gun->getBullet()->getBulletObejct()[i].object->_boundBox) &&
				_gun->getBullet()->getBulletObejct().size() > 0)
			{
				_gun->getBullet()->removeBullet(i);
				break;
			}
			else if (PHYSICSMANAGER->isOverlap(_door[j]->_transform, &_door[j]->_boundBox,
				_gun->getBullet()->getBulletObejct()[i].object->_transform, &_gun->getBullet()->getBulletObejct()[i].object->_boundBox))
			{
				_gun->getBullet()->removeBullet(i);
				break;
			}
			else if (PHYSICSMANAGER->isOverlap(_panelMover[i]->_transform, &_panelMover[i]->_boundBox,
				_gun->getBullet()->getBulletObejct()[i].object->_transform, &_gun->getBullet()->getBulletObejct()[i].object->_boundBox) &&
				_gun->getBullet()->getBulletObejct().size() > 0)
			{
				_gun->getBullet()->removeBullet(i);
				break;
			}
		}
	}
	
	//총알 벽 충돌
	for (int i = 0; i < _gun->getBullet()->getBulletObejct().size(); i++)
	{
		for (int j = 0; j < _map->getBoundBox().size(); j++)
		{
			if (PHYSICSMANAGER->isOverlap(_gun->getBullet()->getBulletObejct()[i].object->_transform,
				&_gun->getBullet()->getBulletObejct()[i].object->_boundBox,
				_map->getObject()->_transform,
				&_map->getBoundBox()[j].box))
			{
				if (SOUNDMANAGER->isPlaySound("portalgun_shoot_blue") || SOUNDMANAGER->isPlaySound("portalgun_shoot_orange"))
				{
					SOUNDMANAGER->play("portal_open", 0.5f);
					SOUNDMANAGER->stop("portalgun_shoot_blue");
					SOUNDMANAGER->stop("portalgun_shoot_orange");
				}
				//포탈이 생성되지 않는 벽은 무시
				if (!_map->getBoundBox()[j].isPortalAble)
				{
					_gun->getBullet()->removeBullet(i);
					break;
				}
				else
				{
					//파란 포탈일때
					if (_gun->getColor() == BULLET_BLUE)
					{
						_bPortal->setIsSet(true);
						switch (_map->getBoundBox()[j].dir)
						{
						case BOUNDBOXDIR_BACK:
						{
							_bPortal->_transform->setWorldPosition(_map->getBoundBox()[j].trans.getWorldPosition().x + 0.15f,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().y,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().z);
							_bPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward());
							break;
						}
						case BOUNDBOXDIR_FORWARD:
						{
							_bPortal->_transform->setWorldPosition(_map->getBoundBox()[j].trans.getWorldPosition().x - 0.15f,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().y,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().z);
							_bPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward());
							break;
						}
						case BOUNDBOXDIR_DOWN:
						{
							_bPortal->_transform->setWorldPosition(_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().x,
								_map->getBoundBox()[j].trans.getWorldPosition().y - 0.15f,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().z);
							_bPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward(), D3DXVECTOR3(0, 0, -1));
							break;
						}
						case BOUNDBOXDIR_UP:
						{
							_bPortal->_transform->setWorldPosition(_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().x,
								_map->getBoundBox()[j].trans.getWorldPosition().y + 0.15f,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().z);
							_bPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward(), D3DXVECTOR3(0, 0, -1));
							break;
						}
						case BOUNDBOXDIR_LEFT:
						{
							_bPortal->_transform->setWorldPosition(_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().x,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().y,
								_map->getBoundBox()[j].trans.getWorldPosition().z - 0.15f);
							_bPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward());
							break;
						}
						case BOUNDBOXDIR_RIGHT:
						{
							_bPortal->_transform->setWorldPosition(_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().x,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().y,
								_map->getBoundBox()[j].trans.getWorldPosition().z + 0.15f);
							_bPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward());
							break;
						}
						}
						if (PHYSICSMANAGER->isOverlap(_oPortal, _laser[0]))
						{
							_laserCollideCount = 0;
							_laserScale = 1;
							this->laserSetting(_map->getBoundBox()[j].dir);
						}
					}
					//주황 포탈일때
					else if (_gun->getColor() == BULLET_ORANGE)
					{
						_oPortal->setIsSet(true);
						switch (_map->getBoundBox()[j].dir)
						{
						case BOUNDBOXDIR_BACK:
						{
							_oPortal->_transform->setWorldPosition(_map->getBoundBox()[j].trans.getWorldPosition().x + 0.15f,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().y,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().z);
							_oPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward());
							break;
						}
						case BOUNDBOXDIR_FORWARD:
						{
							_oPortal->_transform->setWorldPosition(_map->getBoundBox()[j].trans.getWorldPosition().x - 0.15f,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().y,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().z);
							_oPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward());
							break;
						}
						case BOUNDBOXDIR_DOWN:
						{
							_oPortal->_transform->setWorldPosition(_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().x,
								_map->getBoundBox()[j].trans.getWorldPosition().y - 0.15f,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().z);
							_oPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward(), D3DXVECTOR3(0, 0, -1));
							break;
						}
						case BOUNDBOXDIR_UP:
						{
							_oPortal->_transform->setWorldPosition(_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().x,
								_map->getBoundBox()[j].trans.getWorldPosition().y + 0.15f,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().z);
							_oPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward(), D3DXVECTOR3(0, 0, -1));
							break;
						}
						case BOUNDBOXDIR_LEFT:
						{
							_oPortal->_transform->setWorldPosition(_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().x,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().y,
								_map->getBoundBox()[j].trans.getWorldPosition().z - 0.15f);
							_oPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward());
							break;
						}
						case BOUNDBOXDIR_RIGHT:
						{
							_oPortal->_transform->setWorldPosition(_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().x,
								_gun->getBullet()->getBulletObejct()[i].object->_transform->getWorldPosition().y,
								_map->getBoundBox()[j].trans.getWorldPosition().z + 0.15f);
							_oPortal->_transform->lookDirection(_map->getBoundBox()[j].trans.getForward());
							break;
						}
						}
						if (PHYSICSMANAGER->isOverlap(_bPortal, _laser[0]))
						{
							_laserCollideCount = 0;
							_laserScale = 1;
							this->laserSetting(_map->getBoundBox()[j].dir);
						}
					}

				}
				_gun->setIsHit(true);
				_gun->getBullet()->removeBullet(i);
				break;

			}
			
		}
	}
	//서로 포탈이 충돌 하면 하나를 보여주지 않도록 함
	if (PHYSICSMANAGER->isOverlap(_oPortal,_bPortal))
	{
		if (_gun->getColor() == BULLET_ORANGE)
		{
			_oPortal->setIsSet(false);
		}
		else if (_gun->getColor() == BULLET_BLUE)
		{
			_bPortal->setIsSet(false);
		}
	}
	//입구 문 충돌
	if (PHYSICSMANAGER->isOverlap(_door[0]->_transform, &_door[0]->getOpenTrigger(), _player->_transform, &_player->_boundBox))
	{
		if (_door[0]->getState() != DOOR_OPEN) SOUNDMANAGER->play("door_open", 0.2f);
		_door[0]->setState(DOOR_OPEN);
	}
	else if (PHYSICSMANAGER->isOverlap(_door[0]->_transform, &_door[0]->getCloseTrigger(), _player->_transform, &_player->_boundBox))
	{
		if (_door[0]->getState() != DOOR_CLOSE)
		{
			SOUNDMANAGER->stop("door_open");
			SOUNDMANAGER->play("door_close", 0.5f);
		}
		_door[0]->setState(DOOR_CLOSE);
	}
	if (_door[0]->getState() == DOOR_CLOSE)
	{
		PHYSICSMANAGER->isBlocking(_door[0]->_transform, &_door[0]->_boundBox, _player->_transform, &_player->_boundBox);
	}
	//레이저 캐쳐와 레이저 충돌
	if (PHYSICSMANAGER->isOverlap(_catcher->_transform, &_catcher->getHitBox(), _laser[1]->_transform, &_laser[1]->_boundBox) && _laser[1]->getIsSet())
	{
		if (_panelMover[0]->getState() != MOVERSTATE_STOP)
		{
			if (_panelMover[0]->getState() != MOVERSTATE_MOVE) SOUNDMANAGER->play("panelmover_moving", 0.5f);
			_panelMover[0]->setState(MOVERSTATE_MOVE);
		}
		_catcher->setState(CATCHERSTATE_ON);
	}
	else
	{
		_catcher->setState(CATCHERSTATE_NONE);
		SOUNDMANAGER->pause("panelmover_moving");
		_panelMover[0]->setState(MOVERSTATE_NONE);
	}
	//움직이는 판자와 플레이어 충돌
	if (PHYSICSMANAGER->isOverlap(_player->_transform, &_player->_boundBox, _panelMover[0]->_transform, &_panelMover[0]->_boundBox))
	{
		PHYSICSMANAGER->isBlocking(_panelMover[0]->_transform, &_panelMover[0]->getPlayerBoundBox(), _player->_transform, &_player->_boundBox);
		_player->_transform->setWorldPosition(_player->_transform->getWorldPosition().x, _panelMover[0]->_transform->getWorldPosition().y - 2.5f, _player->_transform->getWorldPosition().z);
		_player->setGravity(0);
		_panelMover[0]->_transform->addChild(_player->_transform);
	}
	else if (PHYSICSMANAGER->isOverlap(_player->_transform, &_player->_boundBox, _panelMover[1]->_transform, &_panelMover[1]->_boundBox))
	{
		PHYSICSMANAGER->isBlocking(_panelMover[1]->_transform, &_panelMover[1]->getPlayerBoundBox(), _player->_transform, &_player->_boundBox);
		_player->_transform->setWorldPosition(_player->_transform->getWorldPosition().x, _panelMover[1]->_transform->getWorldPosition().y - 2.5f, _player->_transform->getWorldPosition().z);
		_player->setGravity(0);
		if (_panelMover[1]->getState() != MOVERSTATE_STOP)
		{
			if (_panelMover[1]->getState() != MOVERSTATE_MOVE) SOUNDMANAGER->resume("panelmover_moving");
			_panelMover[1]->setState(MOVERSTATE_MOVE);
		}
		_panelMover[1]->_transform->addChild(_player->_transform);
	}
	else
	{
		_player->_transform->releaseParent();
	}
	//움직이는 판자와 큐브 충돌
	if (PHYSICSMANAGER->isOverlap(_cube->_transform, &_cube->_boundBox, _panelMover[0]->_transform, &_panelMover[0]->getPlayerBoundBox()))
	{
		PHYSICSMANAGER->isBlocking(_panelMover[0]->_transform, &_panelMover[0]->getPlayerBoundBox(), _cube->_transform, &_cube->_boundBox);
		_cube->_transform->setWorldPosition(_panelMover[0]->_transform->getWorldPosition().x - 1, _panelMover[0]->_transform->getWorldPosition().y - 2.6f, _panelMover[0]->_transform->getWorldPosition().z);
		_cube->setGravity(0);
		_panelMover[0]->_transform->addChild(_cube->_transform);
	}
	else if (PHYSICSMANAGER->isOverlap(_cube->_transform, &_cube->_boundBox, _panelMover[1]->_transform, &_panelMover[1]->getPlayerBoundBox()))
	{
		PHYSICSMANAGER->isBlocking(_panelMover[1]->_transform, &_panelMover[1]->getPlayerBoundBox(), _cube->_transform, &_cube->_boundBox);
		_cube->_transform->setWorldPosition(_panelMover[1]->_transform->getWorldPosition().x , _panelMover[1]->_transform->getWorldPosition().y - 2.6f, _panelMover[1]->_transform->getWorldPosition().z + 1);
		_cube->setGravity(0);
		_panelMover[1]->_transform->addChild(_cube->_transform);
	}
	else
	{
		_cube->_transform->releaseParent();
		_cube->setGravity(0.2f);
	}
	// 엘리베이터 상태 변화
	if (PHYSICSMANAGER->isOverlap(_elevator[0]->_transform, &_elevator[0]->getElevatorEventBox(), _player->_transform, &_player->_boundBox))
	{
		if (_elevator[0]->getState() != ELEVATOR_LEAVE) SOUNDMANAGER->play("elevator_door_close", 0.2f);
		_elevator[0]->setState(ELEVATOR_LEAVE);
	}
	else if (PHYSICSMANAGER->isOverlap(_elevator[1]->_transform, &_elevator[1]->getElevatorEventBox(), _player->_transform, &_player->_boundBox))
	{
		_elevator[1]->setState(ELEVATOR_ARRIVE);
	}
	//엘리베이터와 밑부분 충돌
	if (_elevator[0]->getState() != ELEVATOR_LEAVE)
	{
		if (PHYSICSMANAGER->isOverlap(_elevator[0]->_transform, &_elevator[0]->_boundBox, _player->_transform, &_player->_boundBox))
		{
			_player->_transform->setWorldPosition(_player->_transform->getWorldPosition().x, _elevator[0]->_transform->getWorldPosition().y + 2.5f, _player->_transform->getWorldPosition().z);
			_player->setGravity(0);
		}
		else
		{
			_player->setGravity(0.2f);
		}
	}
	if (PHYSICSMANAGER->isOverlap(_elevator[1]->_transform, &_elevator[1]->_boundBox, _player->_transform, &_player->_boundBox))
	{
		_player->_transform->setWorldPosition(_player->_transform->getWorldPosition().x, _elevator[1]->_transform->getWorldPosition().y + 2.5f, _player->_transform->getWorldPosition().z);
		_player->setGravity(0);
	}
	//큐브 플레이어 , 큐브 맵 충돌
	//플레이어가 E키를 눌러서 큐브를 들때
	if (_player->getIsCarry())
	{
		//바운드박스가 겹치면
		if (PHYSICSMANAGER->isOverlap(_cube->_transform, &_cube->getPlayerBoundBox(), _player->_transform, &_player->_boundBox))
		{
			_player->setIsCollide(C_CUBE);
			_player->cubeCollision(_cube);
			_cube->_transform->setLocalPosition(0, 2.0f + _mainCamera->getForward().y * 2.5f, 1.9f);
		}
	}
	else
	{
		_cube->_transform->releaseParent();
		if (_player->getIsCollide() == C_CUBE) SOUNDMANAGER->play("box_lay", 1.0f);
		_player->setIsCollide(C_NONE);
	}

	if (PHYSICSMANAGER->isOverlap(_cube->_transform, &_cube->getPlayerBoundBox(), _player->_transform, &_player->_boundBox))
	{
		PHYSICSMANAGER->isBlocking(_player->_transform, &_player->_boundBox, _cube->_transform, &_cube->_boundBox);
	}

	//큐브와 맵충돌
	if (PHYSICSMANAGER->isOverlap(_cube, _map->getObject()))
	{
		for (int i = 0; i < _map->getBoundBox().size(); i++)
		{
			PHYSICSMANAGER->isBlocking(_map->getObject()->_transform, &_map->getBoundBox()[i].box, _cube->_transform, &_cube->_boundBox);
			if (_map->getBoundBox()[i].dir != BOUNDBOXDIR_UP)
			{
				_cube->setCollisionType(COLLISIONTYPE_WALL);
			}
			else
			{
				_cube->setCollisionType(COLLISIONTYPE_NONE);
			}
		}
	}
	//큐브 플레이어 충돌
	if (PHYSICSMANAGER->isOverlap(_cube->_transform, &_cube->_boundBox, _player->_transform, &_player->_boundBox))
	{
		if (_cube->getCollisionType() == COLLISIONTYPE_WALL)
		{
			PHYSICSMANAGER->isBlocking(_cube->_transform, &_cube->_boundBox, _player->_transform, &_player->_boundBox);
			_player->setIsCarry(false);
			_cube->_transform->releaseParent();
		}
	}
	else if (PHYSICSMANAGER->isOverlap(_cube->_transform, &_cube->_boundBox, _player->_transform, &_player->_boundBox))
	{
		PHYSICSMANAGER->isBlocking(_player->_transform, &_player->_boundBox, _cube->_transform, &_cube->_boundBox);
	}

	//맵과 플레이어 충돌
	if (PHYSICSMANAGER->isOverlap(_player, _map->getObject()))
	{
		for (int i = 0; i < _map->getBoundBox().size() - 1; i++)
		{
			PHYSICSMANAGER->isBlocking(_map->getObject()->_transform, &_map->getBoundBox()[i].box, _player->_transform, &_player->_boundBox);
		}
	}
	//맵과 플레이어 충돌
	if (PHYSICSMANAGER->isOverlap(_player, _map->getObject()))
	{
		for (int i = 0; i < _map->getBoundBox().size(); i++)
		{
			PHYSICSMANAGER->isBlocking(_map->getObject()->_transform, &_map->getBoundBox()[i].box, _player->_transform, &_player->_boundBox);
		}
	}
	//큐브 버튼 충돌
	if (PHYSICSMANAGER->isOverlap(_button->_transform, &_button->_boundBox, _cube->_transform, &_cube->_boundBox))
	{
		_button->_transform->releaseParent();
		_button->setState(BUTTON_DOWN);
		_cube->_transform->setWorldPosition(_button->_transform->getWorldPosition().x, _button->_transform->getWorldPosition().y + 0.7f, _button->_transform->getWorldPosition().z);
		SOUNDMANAGER->stop("panelmover_moving");
		_panelMover[1]->setState(MOVERSTATE_NONE);

		if (_door[1]->getState() != DOOR_OPEN)
		{
			SOUNDMANAGER->stop("door_open");
			SOUNDMANAGER->play("door_close", 0.5f);
		}
		_door[1]->setState(DOOR_OPEN);
	}
	else
	{
		_button->setState(BUTTON_UP);
		if (_door[1]->getState() != DOOR_CLOSE)
		{
			SOUNDMANAGER->stop("door_open");
			SOUNDMANAGER->play("door_close", 0.2f);
		}
		_door[1]->setState(DOOR_CLOSE);
	}
	if (PHYSICSMANAGER->isOverlap(_door[1]->_transform, &_door[1]->getCloseTrigger(), _player->_transform, &_player->_boundBox))
	{
		_cube->_transform->movePositionSelf(0, 2, 0);
		if (_door[1]->getState() != DOOR_CLOSE)
		{
			SOUNDMANAGER->stop("door_open");
			SOUNDMANAGER->play("door_close", 0.5f);
		}
		_door[1]->setState(DOOR_CLOSE);
	}
	//문이 닫이면 문과 충돌을 한다
	if (_door[1]->getState() == DOOR_CLOSE)
	{
		PHYSICSMANAGER->isBlocking(_door[1]->_transform, &_door[1]->_boundBox, _player->_transform, &_player->_boundBox);
	}
	//버튼과 플레이어 충돌
	if (PHYSICSMANAGER->isOverlap(_button->_transform, &_button->_boundBox, _player->_transform, &_player->_boundBox))
	{
		PHYSICSMANAGER->isBlocking(_button->_transform, &_button->_boundBox, _player->_transform, &_player->_boundBox);
		//플레이어가 버튼 위로 올라간다
		_player->_transform->setWorldPosition(_player->_transform->getWorldPosition().x, _button->_transform->getWorldPosition().y + 0.2f, _player->_transform->getWorldPosition().z);
		_player->setGravity(0);
	}
	else
	{
		if (_player->getGravity() == 0.2f) return;
		_player->setGravity(0.2f);
	}
}