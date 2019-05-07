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
	void tutorial2Collision(void);
	void nextScene(void);
	void save(void);
	tutorialScene_2() {};
	~tutorialScene_2() {};
};

//���� 2�� , �� 2�� , ť�� 1��, ���� 1��