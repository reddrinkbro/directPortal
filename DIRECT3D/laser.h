#pragma once
#include "particleEmitter.h"
class laser
{
private:
	LPDIRECT3DTEXTURE9  _laser;			//������ �ؽ���
	LPD3DXEFFECT		_effect;		//���̴� ����Ʈ
public:
	transform _transform;
	HRESULT init(float x, float y, float z);
	void release(void);
	void render(void);
public:
	laser() {};
	~laser() {};
};

