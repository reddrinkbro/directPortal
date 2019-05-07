#pragma once
#include "singletonBase.h"
//=============================================================
//	## spriteManager ## (��������Ʈ �Ŵ���)
//=============================================================

class spriteManager : public singletonBase <spriteManager>
{
private:
	LPD3DXSPRITE	_sprite;

public:
	HRESULT init(void);
	void release(void);

	//��������Ʈ ���� ���۽� ȣ��
	void beginSpriteRender();
	//��������Ʈ ���� ����� ȣ��
	void endSpriteRender();

	//��������ǥ�� ����
	//�Ϲݷ���(�ؽ���, �׸���Ʈ����, x, y, ���̴� ����, �̹����߽���)
	void spriteRender(
		LPDIRECT3DTEXTURE9 texture,
		LPRECT rcSour,
		float posX, float posY,
		DWORD color = 0xffffffff,
		D3DXVECTOR3* center = NULL);
	//�Ϲݷ���(�̹��� ũ��, ȸ������)
	void spriteRender(
		LPDIRECT3DTEXTURE9 texture,
		LPRECT rcSour,
		float posX, float posY,
		float scaleX, float scaleY,
		float rotate,
		DWORD color = 0xffffffff,
		D3DXVECTOR3* center = NULL);

	//��Ʈ�� ������ŭ ����
	void renderArea(
		LPDIRECT3DTEXTURE9 texture,
		LPRECT rcSour,
		LPRECT rcDest,
		DWORD color = 0xffffffff);

	//������ġ ����
	void ninePatchRender(
		LPDIRECT3DTEXTURE9 texture,
		LPRECT rcSour,
		LPRECT rcDest,
		LPRECT rcCenter,
		DWORD color = 0xffffffff,
		bool isDrawCenter = true);

	spriteManager() {}
	~spriteManager() {}
};

