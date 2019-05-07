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
	LPDIRECT3DTEXTURE9  _beforeScene;		//시작했을때 전환
	LPDIRECT3DTEXTURE9  _afterScene;		//엘리베이터에 갔을때 전환
	menuScene*			_menu;
	bool				_sceneStart;		//씬이 시작했는지
	bool				_sceneEnd;			//씬이 끝났는지
	int					_startCount;		//시작카운트
	int					_endCount;			//엔드카운트, 씬이 끝날때 텍스쳐 띄울때 사용
	float				_endRotateAngle;	//엘리베이터에서 플레이어 움직이는 각도
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

