#pragma once
#include "gameNode.h"
#include "portalHeader.h"
class stage1Scene : public gameNode
{
private:
	stage1 * _map;
	player*		  _player;
	portalGun*	  _gun;
	elevator*	  _elevator[2];
	door*		  _door[2];
	cube*		  _cube;
	button*		  _button;
	bluePortal *  _bPortal;
	orangePortal* _oPortal;

	LPDIRECT3DTEXTURE9  _beforeScene;		//���������� ��ȯ
	LPDIRECT3DTEXTURE9  _afterScene;		//���������Ϳ� ������ ��ȯ
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
	void stage1Collision(void);
	void nextScene(void);
	void save(void);
	stage1Scene() {};
	~stage1Scene() {};
};

