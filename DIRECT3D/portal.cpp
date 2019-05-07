#include "stdafx.h"
#include "portal.h"

//�Ķ� ��Ż
HRESULT bluePortal::init()
{
	_isSet = false;
	setActive(true);
	D3DCOLOR colorKey = D3DCOLOR_XRGB(0, 0, 0);
	D3DXCreateTextureFromFileEx(device, "Resources/Texture/bluePortal.png", 200, 200, 1, NULL,
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, colorKey, NULL, NULL, &_bluePortal);

	createQuadCube();
	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	_transform->addChild(&_playerPos);
	_playerPos.setLocalPosition(0, 0, -2);
	_transform->setRotateWorld(D3DXToRadian(60), D3DXToRadian(90), 0);
	_boundBox.setBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0.6f, 1, 0.01f));
	LPD3DXBUFFER	error = NULL;
	DWORD			shaderFlag = 0;

	// fx ���Ϸκ��� ����Ʈ ��ü �ε�	D3DXCreateEffectFromFile()
	D3DXCreateEffectFromFile(
		device,											// ����̽�
		"Resources/Shader/Texture.fx",					// ���� �̸�
		NULL,				// ���̴��� �������� �� �߰� ����� #define ����
		NULL,				// ���̴��� �������� �� #include ��� �� �÷���
		shaderFlag,			// ���̴� �÷���
		NULL,				// �޸� Ǯ
		&_effect,			// �ε� �� ���̴� ����Ʈ ������
		&error				// ���̴� �ε��� �����Ͻ� ������ ����� �������ۿ� �޽����� ��.
	);


	// ���� �߻� ��
	if (error != NULL)
	{
		SAFE_RELEASE(error);
		return E_FAIL;
	}
	return S_OK;
}

void bluePortal::release(void)
{
	releaseVI();
	SAFE_RELEASE(_bluePortal);
}

void bluePortal::render(void)
{
	if (!_isSet) return;
	_transform->setDeviceWorld();
	// fx ���� ���� �������� ����
	//float4x4		matWVP;			//���� �� ���� 3����ü ���
	D3DXMATRIXA16	matWorld, matView, matProjection;
	device->GetTransform(D3DTS_WORLD, &matWorld);
	device->GetTransform(D3DTS_VIEW, &matView);
	device->GetTransform(D3DTS_PROJECTION, &matProjection);

	// �Ѻ�ȯ ���
	D3DXMATRIXA16 matWVP = matWorld * matView * matProjection;
	_effect->SetMatrix("matWVP", &matWVP);

	// �÷� ���� //float4		vColor;			//��� �÷�
	D3DXVECTOR4		vColor(1, 1, 1, 1);
	_effect->SetVector("vColor", &vColor);

	// 
	//float		tilingX;		//���� Ÿ�ϼ�
	//float		tilingY;		//���� Ÿ�ϼ�
	_effect->SetFloat("tilingX", 1.0f);
	_effect->SetFloat("tilingY", 1.0f);

	// �ؽ��� 
	//texture MyTex;
	_effect->SetTexture("MyTex", _bluePortal);

	// ��ũ�� ����
	_effect->SetTechnique("MyShader");

	// ���̴��� ����
	UINT iPassNum = 0;
	// ����Ʈ�� ������ ����
	_effect->Begin(&iPassNum, 0);

	for (UINT i = 0; i < iPassNum; i++)
	{
		_effect->BeginPass(i);
		renderQuadCube();
		_effect->EndPass();
	}

	// ����Ʈ�� ���� ����
	_effect->End();
}
//��Ȳ ��Ż
HRESULT orangePortal::init()
{
	_isSet = false;
	setActive(true);
	D3DCOLOR colorKey = D3DCOLOR_XRGB(0, 0, 0);
	D3DXCreateTextureFromFileEx(device, "Resources/Texture/orangePortal.png", 200, 200, 1, NULL,
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, colorKey, NULL, NULL, &_orangePortal);

	createQuadCube();
	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	_transform->addChild(&_playerPos);
	_playerPos.setLocalPosition(0, 0, -2);
	_transform->setRotateWorld(D3DXToRadian(60), D3DXToRadian(90), 0);
	_boundBox.setBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0.6f, 1, 0.01f));
	LPD3DXBUFFER	error = NULL;
	DWORD			shaderFlag = 0;

	// fx ���Ϸκ��� ����Ʈ ��ü �ε�	D3DXCreateEffectFromFile()
	D3DXCreateEffectFromFile(
		device,											// ����̽�
		"Resources/Shader/Texture.fx",					// ���� �̸�
		NULL,				// ���̴��� �������� �� �߰� ����� #define ����
		NULL,				// ���̴��� �������� �� #include ��� �� �÷���
		shaderFlag,			// ���̴� �÷���
		NULL,				// �޸� Ǯ
		&_effect,			// �ε� �� ���̴� ����Ʈ ������
		&error				// ���̴� �ε��� �����Ͻ� ������ ����� �������ۿ� �޽����� ��.
	);


	// ���� �߻� ��
	if (error != NULL)
	{
		SAFE_RELEASE(error);
		return E_FAIL;
	}
	return S_OK;
}

void orangePortal::release(void)
{
	releaseVI();
	SAFE_RELEASE(_orangePortal);
}

void orangePortal::render(void)
{
	if (!_isSet) return;
	_transform->setDeviceWorld();
	// fx ���� ���� �������� ����
	//float4x4		matWVP;			//���� �� ���� 3����ü ���
	D3DXMATRIXA16	matWorld, matView, matProjection;
	device->GetTransform(D3DTS_WORLD, &matWorld);
	device->GetTransform(D3DTS_VIEW, &matView);
	device->GetTransform(D3DTS_PROJECTION, &matProjection);

	// �Ѻ�ȯ ���
	D3DXMATRIXA16 matWVP = matWorld * matView * matProjection;
	_effect->SetMatrix("matWVP", &matWVP);

	// �÷� ���� //float4		vColor;			//��� �÷�
	D3DXVECTOR4		vColor(1, 1, 1, 1);
	_effect->SetVector("vColor", &vColor);

	// 
	//float		tilingX;		//���� Ÿ�ϼ�
	//float		tilingY;		//���� Ÿ�ϼ�
	_effect->SetFloat("tilingX", 1.0f);
	_effect->SetFloat("tilingY", 1.0f);

	// �ؽ��� 
	//texture MyTex;
	_effect->SetTexture("MyTex", _orangePortal);

	// ��ũ�� ����
	_effect->SetTechnique("MyShader");

	// ���̴��� ����
	UINT iPassNum = 0;
	// ����Ʈ�� ������ ����
	_effect->Begin(&iPassNum, 0);

	for (UINT i = 0; i < iPassNum; i++)
	{
		_effect->BeginPass(i);
		renderQuadCube();
		_effect->EndPass();
	}

	// ����Ʈ�� ���� ����
	_effect->End();
}