#pragma once
#include "laser.h"

class laserPoint : public baseObject
{
private:
	laser * _laser[6];
	BOUNDBOXDIR _laserBoxDir;	//레이저가 어느방향으로 길어질지 정하기위한 방향
	bool		_isSet;
public:
	HRESULT init(float x, float y, float z);
	void release(void);
	void render(void);
	void setLaser(D3DXVECTOR3 pos, BOUNDBOXDIR boxDir, int scale);
public:
	BOUNDBOXDIR getLaserBoxDir(void) { return _laserBoxDir; }
	bool getIsSet(void) { return _isSet; }
	void setIsSet(bool isSet) { _isSet = isSet; }
	laserPoint() {};
	~laserPoint() {};
};

