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

	LPDIRECT3DTEXTURE9  _beforeScene;		//���������� ��ȯ
	bool				_sceneStart;		//���� �����ߴ���
	bool				_sceneEnd;			//���� ��������
	int					_startCount;		//����ī��Ʈ
	int					_endCount;			//����ī��Ʈ, ���� ������ �ؽ��� ��ﶧ ���
	float				_endRotateAngle;	//���������Ϳ��� �÷��̾� �����̴� ����

	int					_laserCollideCount;	//�������� �浹���� ���� ����
	float				_laserScale;
	int					_laserCollideTime;	//�������� �浹���� �ð�
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);
public:
	void stage3Collision(void);
	void panelStateChange(void);			//���� �����ӻ��º�ȭ�Լ�
	void laserSetting(BOUNDBOXDIR dir);		//��Ż�� �������� ��� ������ ��ġ�Լ�
	void laserSetting(void);				//��Ż�� �Ѵ� ��ġ�� ���¿��� �������� ������ ��ġ�Լ�
	void nextScene(void);
	void save(void);
	void playerLaserCollision(void);		//�÷��̾� ������ �浹
public:
	stage3Scene() {};
	~stage3Scene() {};
};

