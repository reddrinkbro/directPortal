#include "stdafx.h"
#include "stage2Scene.h"


void stage2Scene::stage2Collision(void)
{
	//��Ȳ��Ż�̶� �÷��̾� �浹
	if (PHYSICSMANAGER->isOverlap(_oPortal->_transform, &_oPortal->_boundBox, _player->_transform, &_player->_boundBox) && _bPortal->getIsSet())
	{
		_player->_transform->setWorldPosition(_bPortal->getPlayerPos().getWorldPosition());
		SOUNDMANAGER->play("portal_exit", 1.0f);
		if (SOUNDMANAGER->isPlaySound("portal_enter")) SOUNDMANAGER->stop("portal_enter");
	}
	//�Ķ���Ż�̶� �÷��̾� �浹
	else if (PHYSICSMANAGER->isOverlap(_bPortal->_transform, &_bPortal->_boundBox, _player->_transform, &_player->_boundBox) && _oPortal->getIsSet())
	{
		_player->_transform->setWorldPosition(_oPortal->getPlayerPos().getWorldPosition());
		SOUNDMANAGER->play("portal_enter", 1.0f);
		if (SOUNDMANAGER->isPlaySound("portal_exit")) SOUNDMANAGER->stop("portal_exit");
	}
	//��Ȳ��Ż�̶� ť�� �浹
	else if (PHYSICSMANAGER->isOverlap(_oPortal->_transform, &_oPortal->_boundBox, _cube->_transform, &_cube->_boundBox) && _player->getIsCollide() != C_CUBE && _bPortal->getIsSet())
	{
		_cube->_transform->setWorldPosition(_bPortal->getPlayerPos().getWorldPosition());
		SOUNDMANAGER->play("portal_exit", 1.0f);
		if (SOUNDMANAGER->isPlaySound("portal_enter")) SOUNDMANAGER->stop("portal_enter");
	}
	//�Ķ���Ż�̶� ť�� �浹
	else if (PHYSICSMANAGER->isOverlap(_bPortal->_transform, &_bPortal->_boundBox, _cube->_transform, &_cube->_boundBox) && _player->getIsCollide() != C_CUBE && _oPortal->getIsSet())
	{
		_cube->_transform->setWorldPosition(_oPortal->getPlayerPos().getWorldPosition());
		SOUNDMANAGER->play("portal_enter", 1.0f);
		if (SOUNDMANAGER->isPlaySound("portal_exit")) SOUNDMANAGER->stop("portal_exit");
	}


	//�Ѿ� �� �浹
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
				//��Ż�� �������� �ʴ� ���� ����
				if (!_map->getBoundBox()[j].isPortalAble)
				{
					_gun->getBullet()->removeBullet(i);
					break;
				}
				else
				{
					//�Ķ� ��Ż�϶�
					if (_gun->getColor() == BULLET_BLUE)
					{
						if (PHYSICSMANAGER->isOverlap(_oPortal->_transform, &_oPortal->_boundBox, _gun->getBullet()->getBulletObejct()[i].object->_transform,
							&_gun->getBullet()->getBulletObejct()[i].object->_boundBox)) break;
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
					}
					//��Ȳ ��Ż�϶�
					else if (_gun->getColor() == BULLET_ORANGE)
					{
						if (PHYSICSMANAGER->isOverlap(_bPortal->_transform, &_bPortal->_boundBox, _gun->getBullet()->getBulletObejct()[i].object->_transform,
							&_gun->getBullet()->getBulletObejct()[i].object->_boundBox)) break;
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
					}
				}
				_gun->setIsHit(true);
				_gun->getBullet()->removeBullet(i);
				break;

			}
		}
	}
	//���� ��Ż�� �浹 �ϸ� �ϳ��� �������� �ʵ��� ��
	if (PHYSICSMANAGER->isOverlap(_oPortal, _bPortal))
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
	//�Ѿ� ������Ʈ �浹
	for (int i = 0; i < _gun->getBullet()->getBulletObejct().size(); i++)
	{
		if (PHYSICSMANAGER->isOverlap(_button->_transform, &_button->_boundBox,
			_gun->getBullet()->getBulletObejct()[i].object->_transform, &_gun->getBullet()->getBulletObejct()[i].object->_boundBox) &&
			_gun->getBullet()->getBulletObejct().size() > 0)
		{
			_gun->getBullet()->removeBullet(i);
			break;
		}
		if (PHYSICSMANAGER->isOverlap(_cube->_transform, &_cube->_boundBox,
			_gun->getBullet()->getBulletObejct()[i].object->_transform, &_gun->getBullet()->getBulletObejct()[i].object->_boundBox) &&
			_gun->getBullet()->getBulletObejct().size() > 0)
		{
			_gun->getBullet()->removeBullet(i);
			break;
		}
		if (PHYSICSMANAGER->isOverlap(_switch->_transform, &_switch->_boundBox,
			_gun->getBullet()->getBulletObejct()[i].object->_transform, &_gun->getBullet()->getBulletObejct()[i].object->_boundBox) &&
			_gun->getBullet()->getBulletObejct().size() > 0)
		{
			_gun->getBullet()->removeBullet(i);
			break;
		}
		if (PHYSICSMANAGER->isOverlap(_panelMover->_transform, &_panelMover->_boundBox,
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
		}
	}

	//�Ա� �� �浹
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

	//�����̴� ���ڿ� �÷��̾� �浹
	if (PHYSICSMANAGER->isOverlap(_player->_transform, &_player->_boundBox, _panelMover->_transform, &_panelMover->_boundBox))
	{
		PHYSICSMANAGER->isBlocking(_panelMover->_transform, &_panelMover->getPlayerBoundBox(), _player->_transform, &_player->_boundBox);
		_player->_transform->setWorldPosition(_player->_transform->getWorldPosition().x, _panelMover->_transform->getWorldPosition().y - 2.8f, _player->_transform->getWorldPosition().z);
		_player->setGravity(0);
		_panelMover->_transform->addChild(_player->_transform);
	}
	else
	{
		_player->_transform->releaseParent();
	}
	//�����̴� ���ڿ� ť�� �浹
	if (PHYSICSMANAGER->isOverlap(_cube->_transform, &_cube->_boundBox, _panelMover->_transform, &_panelMover->getPlayerBoundBox()))
	{
		PHYSICSMANAGER->isBlocking(_panelMover->_transform, &_panelMover->getPlayerBoundBox(), _cube->_transform, &_cube->_boundBox);
		_cube->_transform->setWorldPosition(_panelMover->_transform->getWorldPosition().x +1, _panelMover->_transform->getWorldPosition().y - 2.6f, _panelMover->_transform->getWorldPosition().z);
		_cube->setGravity(0);
		_panelMover->_transform->addChild(_cube->_transform);
	}
	else
	{
		_cube->_transform->releaseParent();
		_cube->setGravity(0.2f);
	}
	// ���������� ���� ��ȭ
	if (PHYSICSMANAGER->isOverlap(_elevator[0]->_transform, &_elevator[0]->getElevatorEventBox(), _player->_transform, &_player->_boundBox))
	{
		if (_elevator[0]->getState() != ELEVATOR_LEAVE) SOUNDMANAGER->play("elevator_door_close", 0.2f);
		_elevator[0]->setState(ELEVATOR_LEAVE);
	}
	else if (PHYSICSMANAGER->isOverlap(_elevator[1]->_transform, &_elevator[1]->getElevatorEventBox(), _player->_transform, &_player->_boundBox))
	{
		_elevator[1]->setState(ELEVATOR_ARRIVE);
	}
	//���������Ϳ� �غκ� �浹
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

	//ť�� �÷��̾� , ť�� �� �浹
	//�÷��̾ EŰ�� ������ ť�긦 �鶧
	if (_player->getIsCarry())
	{
		//�ٿ��ڽ��� ��ġ��
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

	//ť��� ���浹
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
	//ť�� �÷��̾� �浹
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

	//�ʰ� �÷��̾� �浹
	if (PHYSICSMANAGER->isOverlap(_player, _map->getObject()))
	{
		for (int i = 0; i < _map->getBoundBox().size() - 1; i++)
		{
			PHYSICSMANAGER->isBlocking(_map->getObject()->_transform, &_map->getBoundBox()[i].box, _player->_transform, &_player->_boundBox);
		}
	}
	//�ʰ� �÷��̾� �浹
	if (PHYSICSMANAGER->isOverlap(_player, _map->getObject()))
	{
		for (int i = 0; i < _map->getBoundBox().size(); i++)
		{
			PHYSICSMANAGER->isBlocking(_map->getObject()->_transform, &_map->getBoundBox()[i].box, _player->_transform, &_player->_boundBox);
		}
	}
	//ť�� ��ư �浹
	if (PHYSICSMANAGER->isOverlap(_button->_transform, &_button->_boundBox, _cube->_transform, &_cube->_boundBox))
	{
		_button->_transform->releaseParent();
		_button->setState(BUTTON_DOWN);
		_cube->_transform->setWorldPosition(_button->_transform->getWorldPosition().x, _button->_transform->getWorldPosition().y + 0.7f, _button->_transform->getWorldPosition().z);
		_panelMover->setState(MOVERSTATE_NONE);
		SOUNDMANAGER->stop("panelmover_moving");
		if (_door[1]->getState() != DOOR_OPEN)
		{
			SOUNDMANAGER->stop("door_open");
			SOUNDMANAGER->stop("switch_click");
			SOUNDMANAGER->play("door_close", 0.5f);
		}
		_door[1]->setState(DOOR_OPEN);
	}
	else
	{
		_button->setState(BUTTON_UP);
		if (_door[1]->getState() != DOOR_CLOSE)
		{
			SOUNDMANAGER->play("door_close", 0.2f);
			SOUNDMANAGER->stop("door_open");
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
	//���� ���̸� ���� �浹�� �Ѵ�
	if (_door[1]->getState() == DOOR_CLOSE)
	{
		PHYSICSMANAGER->isBlocking(_door[1]->_transform, &_door[1]->_boundBox, _player->_transform, &_player->_boundBox);
	}

	//����ġ �浹
	if (PHYSICSMANAGER->isOverlap(_switch->_transform, &_switch->getPlayerBoundBox(), _player->_transform, &_player->_boundBox))
	{
		PHYSICSMANAGER->isBlocking(_switch->_transform, &_switch->_boundBox, _player->_transform, &_player->_boundBox);
		_player->setIsCollide(C_SWITCH);
		if (KEYMANAGER->isOnceKeyDown('E'))
		{
			if (_switch->getState() != SWITCH_ON) SOUNDMANAGER->play("switch_click", 1.0f);
			_switch->setState(SWITCH_ON);
			if (_panelMover->getState() != MOVERSTATE_MOVE) SOUNDMANAGER->play("panelmover_moving", 0.5f);
			_panelMover->setState(MOVERSTATE_MOVE);
		}
	}
	else
	{
		_switch->setState(SWITCH_NONE);
	}
	//��ư�� �÷��̾� �浹
	if (PHYSICSMANAGER->isOverlap(_button->_transform, &_button->_boundBox, _player->_transform, &_player->_boundBox))
	{
		PHYSICSMANAGER->isBlocking(_button->_transform, &_button->_boundBox, _player->_transform, &_player->_boundBox);
		//�÷��̾ ��ư ���� �ö󰣴�
		_player->_transform->setWorldPosition(_player->_transform->getWorldPosition().x, _button->_transform->getWorldPosition().y + 0.4f, _player->_transform->getWorldPosition().z);
		_player->setGravity(0);
	}
	else
	{
		if (_player->getGravity() == 0.2f) return;
		_player->setGravity(0.2f);
	}


}
