#pragma once
#include "gameNode.h"
#include "portalHeader.h"

class tutorialScene_1 : public gameNode
{
private:
	tutorialMap * _map;
	switchButton*		_switch[3];
	cube*				_cube;
	button*				_button[2];
	boxDropper*			_dropper;
	elevator*			_elevator;
	door*				_door;
	portalGun*			_gun;
	player*				_player;
	bluePortal*			_bPortal;
	orangePortal*		_oPortal;
	LPDIRECT3DTEXTURE9  _beforeScene;		//���������� ��ȯ
	LPDIRECT3DTEXTURE9  _afterScene;		//���������Ϳ� ������ ��ȯ
	menuScene*			_menu;
	bool				_sceneStart;		//���� �����ߴ���
	bool				_sceneEnd;			//���� ��������
	int					_startCount;		//����ī��Ʈ
	int					_endCount;			//����ī��Ʈ, ���� ������ �ؽ��� ��ﶧ ���
	float				_endRotateAngle;	//���������Ϳ��� �÷��̾� �����̴� ����
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);
public:
	void clickSwitch(int num);
	void tutorial1Collision(void);
	void switchOn(void);
	void nextScene(void);
	void save(void);
	tutorialScene_1() {};
	~tutorialScene_1() {};
};

