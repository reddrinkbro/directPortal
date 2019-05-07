#pragma once
#include "gameNode.h"
#include "portalHeader.h"

class stage3Scene : public gameNode
{
private:
	stage3 * _map;
	player*					 _player;
	portalGun*				 _gun;
	elevator*				 _elevator[2];
	door*					 _door[2];
	cube*					 _cube;
	button*					 _button;
	bluePortal *			 _bPortal;
	orangePortal*			 _oPortal;
	panelMover*				 _panelMover[2];
	laserEmitter*			 _emitter;
	laserCatcher*			 _catcher;
	laserPoint*				 _laser[2];

	LPDIRECT3DTEXTURE9  _beforeScene;		//시작했을때 전환
	bool				_sceneStart;		//씬이 시작했는지
	bool				_sceneEnd;			//씬이 끝났는지
	int					_startCount;		//시작카운트
	int					_endCount;			//엔드카운트, 씬이 끝날때 텍스쳐 띄울때 사용
	float				_endRotateAngle;	//엘리베이터에서 플레이어 움직이는 각도

	int					_laserCollideCount;	//레이저가 충돌중인 벽의 개수
	float				_laserScale;
	int					_laserCollideTime;	//레이저에 충돌중인 시간
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);
public:
	void stage3Collision(void);
	void panelStateChange(void);			//발판 움직임상태변화함수
	void laserSetting(BOUNDBOXDIR dir);		//포탈이 레이저에 닿고 있을때 설치함수
	void laserSetting(void);				//포탈이 둘다 설치된 상태에서 레이저가 닿을때 설치함수
	void nextScene(void);
	void save(void);
	void playerLaserCollision(void);		//플레이어 레이저 충돌
public:
	stage3Scene() {};
	~stage3Scene() {};
};

