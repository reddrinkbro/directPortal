#pragma once
struct bullet
{
	baseObject* object;
	float	   speed;
};
class portalBullet
{
private:
	vector<bullet> _vBullet;
	vector<bullet>::iterator _viBullet;
public:
	HRESULT init(void);
	void fire(D3DXVECTOR3 pos, float angleX, float angleY);
	void release(void);
	void update(void);
	void render(void);
	void removeBullet(int num);
public:
	vector<bullet> getBulletObejct() { return _vBullet; }
	portalBullet() {};
	~portalBullet() {};
};

