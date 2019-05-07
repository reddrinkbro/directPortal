#pragma once
#include "particleEmitter.h"
class laser
{
private:
	LPDIRECT3DTEXTURE9  _laser;			//레이저 텍스쳐
	LPD3DXEFFECT		_effect;		//쉐이더 이펙트
public:
	transform _transform;
	HRESULT init(float x, float y, float z);
	void release(void);
	void render(void);
public:
	laser() {};
	~laser() {};
};

