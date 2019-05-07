#pragma once
#include "resourceManager.h"
// =============================================================
// 	## rmTexture ## (���ҽ��Ŵ��� �ؽ���)
// =============================================================
class rmTexture : public resourceManager <LPDIRECT3DTEXTURE9, rmTexture>
{
public:
	HRESULT init(void);
	void release(void);

	// ���ҽ� �ε���ĸ� �������Ѵ�(�ؽ���)
	LPDIRECT3DTEXTURE9 loadResource(string fileName, void* param = NULL) override;
	// ���ҽ� ������ĸ� �������Ѵ�
	void releaseResource(LPDIRECT3DTEXTURE9 data) override;

	rmTexture() {}
	~rmTexture() {}
};

