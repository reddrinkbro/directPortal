#pragma once
#include "gameNode.h"
#include "portalHeader.h"

class tutorialScene_2 : public gameNode
{
private:
	tutorialMap2 * _map;
	player*			 _player;
	cube*			 _cube[2];
	button*			 _button[2];
	elevator*		 _elevator[2];
	door*			 _door;
	portalGun*		 _gun[2];
	bluePortal*		 _bPortal;
	orangePortal*	 _oPortal;
	bool			_isGetOrangePortalGun;

	LPDIRECT3DTEXTURE9  _beforeScene;		//시작했을때 전환
	LPDIRECT3DTEXTURE9  _afterScene;		//엘리베이터에 갔을때 전환
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
	void tutorial2Collision(void);
	void nextScene(void);
	void save(void);
	tutorialScene_2() {};
	~tutorialScene_2() {};
};

//엘베 2개 , 문 2개 , 큐브 1개, 소켓 1개