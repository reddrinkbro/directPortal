#include "stdafx.h"
#include "rmShader.h"


HRESULT rmShader::init(void)
{
	return S_OK;
}

void rmShader::release(void)
{
	this->clearResource();
}

LPD3DXEFFECT rmShader::loadResource(string fileName, void * param)
{
	// ���̴� ����Ʈ ����
	LPD3DXEFFECT	effect;
	// ���̴� �ε��� ������ ������ ���� ������ ������ �޾ƿ� error����
	LPD3DXBUFFER	error = NULL;
	DWORD			shaderFlag = 0;

	// fx���Ϸ� ���� ����Ʈ ��ü�� �ε��ϱ�
	D3DXCreateEffectFromFile(
		device,
		fileName.c_str(),
		NULL, NULL, shaderFlag, NULL,
		&effect, &error);

	// ���� ����ó��
	if (error != NULL && effect == NULL)
	{
		SAFE_RELEASE(error);
		return NULL;
	}
	return effect;
}

void rmShader::releaseResource(LPD3DXEFFECT data)
{
	SAFE_RELEASE(data);
}
