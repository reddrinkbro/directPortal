#pragma once
#include "gameNode.h"
#include "portal.h"
class testScene : public gameNode
{
private:
	bluePortal* _laser;
	LPD3DXEFFECT _effect;
	LPDIRECT3DTEXTURE9				_diffuseTex;	//일반텍스쳐
	LPDIRECT3DTEXTURE9				_normalTex;		//노말텍스쳐
	transform						_t;
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	testScene() {}
	~testScene() {}
};

