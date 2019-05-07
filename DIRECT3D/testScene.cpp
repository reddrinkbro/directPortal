#include "stdafx.h"
#include "testScene.h"

HRESULT testScene::init(void)
{
	
	//_effect = RM_SHADER->addResource("Resources/Shader/waterShader.fx");
	
	createQuadBumpNormal();
	_diffuseTex = RM_TEXTURE->loadResource("Resources/Texture/toxicslime_color_00_00_00.png");
	_normalTex = RM_TEXTURE->loadResource("Resources/Texture/water_river_normal_sharp_00_00_00.png");
	//���̴� �ε��� ������ ������ ���� ������ ������ �޾ƿ� error����
	_effect = RM_SHADER->addResource("Resources/Shader/TextureNormal2.fx");
	return S_OK;
}

void testScene::release(void)
{
	
}

void testScene::update(void)
{
	_t.defaultMyControl();
}

void testScene::render(void)
{
	//����Ʈ�� �������������� �����ϱ�
	D3DXMATRIXA16	matWorld, matView, matProjection;
	device->GetTransform(D3DTS_WORLD, &matWorld);
	device->GetTransform(D3DTS_VIEW, &matView);
	device->GetTransform(D3DTS_PROJECTION, &matProjection);

	//�Ѻ�ȯ ���
	D3DXMATRIXA16 matWVP = matWorld * matView * matProjection;
	_effect->SetMatrix("matWVP", &matWVP);
	//�������
	_effect->SetMatrix("matWorld", &matWorld);
	//��������
	D3DXVECTOR3 vLightDir = _t.getForward();
	_effect->SetVector("vLightDir", &D3DXVECTOR4(0,0,1, 1));
	//������ġ
	D3DXVECTOR3 vEyePos = _mainCamera->getWorldPosition();
	_effect->SetVector("vEyePos", &D3DXVECTOR4(0,0,1, 1));
	//����ŧ�� �Ŀ�
	_effect->SetFloat("specPower", 10);

	//�ؽ��� ����
	_effect->SetTexture("DiffuseTex", _diffuseTex);
	_effect->SetTexture("NormalTex", _normalTex);

	//��ũ�� ����
	_effect->SetTechnique("MyShader");

	//���̴��� ���巻��
	UINT iPassNum = 0;
	//����Ʈ�� ����������
	_effect->Begin(&iPassNum, 0);

	//�н� ���ڸ�ŭ for��������
	for (UINT i = 0; i < iPassNum; i++)
	{
		_effect->BeginPass(i);
		renderQuadBumpNormal();
		_effect->EndPass();
	}

	//����Ʈ�� ��������
	_effect->End();
	_t.renderGizmo();
	//_laser->render();

}
//ī�޶� ��ġ
//��Ż�� ����Ʈ