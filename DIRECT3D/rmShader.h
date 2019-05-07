#pragma once
#include "resourceManager.h"
// =============================================================
// 	## rmShader ## (���ҽ��Ŵ��� ���̴�)
// =============================================================
class rmShader : public resourceManager <LPD3DXEFFECT, rmShader>
{
public:
	HRESULT init(void);
	void release(void);

	// ���ҽ� �ε���ĸ� �������Ѵ�(���̴�)
	LPD3DXEFFECT loadResource(string fileName, void* param = NULL) override;
	// ���ҽ� ������ĸ� �������Ѵ�
	void releaseResource(LPD3DXEFFECT data) override;

	rmShader() {}
	~rmShader() {}
};