#include "stdafx.h"
#include "laser.h"


HRESULT laser::init(float x, float y, float z)
{
	D3DCOLOR colorKey = D3DCOLOR_XRGB(255, 255, 255);
	D3DXCreateTextureFromFileEx(device, "Resources/Texture/laser.png", 100, 100, 1, NULL,
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, colorKey, NULL, NULL, &_laser);

	createQuadCube();
	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	_transform.setWorldPosition(x, y, z);
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

void laser::release(void)
{
	releaseVI();
	SAFE_RELEASE(_laser);
}

void laser::render(void)
{
	//���� ���� ����
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Texture �� ���� Diffuse ���⼭�� �����÷��� ���İ� �� ���� ���� ����� �Ѵ�.
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	_transform.setDeviceWorld();
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
	_effect->SetTexture("MyTex", _laser);

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
	//��ƼŬ �׸��� ���� ��ó��
	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	device->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	device->SetRenderState(D3DRS_ZWRITEENABLE, true);
	device->SetTexture(0, NULL);
}
