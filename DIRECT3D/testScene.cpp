#include "stdafx.h"
#include "testScene.h"

HRESULT testScene::init(void)
{
	
	//_effect = RM_SHADER->addResource("Resources/Shader/waterShader.fx");
	
	createQuadBumpNormal();
	_diffuseTex = RM_TEXTURE->loadResource("Resources/Texture/toxicslime_color_00_00_00.png");
	_normalTex = RM_TEXTURE->loadResource("Resources/Texture/water_river_normal_sharp_00_00_00.png");
	//셰이더 로딩시 컴파일 에러가 나면 에러의 내용을 받아올 error버퍼
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
	//이펙트에 전역변수값들을 세팅하기
	D3DXMATRIXA16	matWorld, matView, matProjection;
	device->GetTransform(D3DTS_WORLD, &matWorld);
	device->GetTransform(D3DTS_VIEW, &matView);
	device->GetTransform(D3DTS_PROJECTION, &matProjection);

	//총변환 행렬
	D3DXMATRIXA16 matWVP = matWorld * matView * matProjection;
	_effect->SetMatrix("matWVP", &matWVP);
	//월드행렬
	_effect->SetMatrix("matWorld", &matWorld);
	//광원방향
	D3DXVECTOR3 vLightDir = _t.getForward();
	_effect->SetVector("vLightDir", &D3DXVECTOR4(0,0,1, 1));
	//눈의위치
	D3DXVECTOR3 vEyePos = _mainCamera->getWorldPosition();
	_effect->SetVector("vEyePos", &D3DXVECTOR4(0,0,1, 1));
	//스펙큘라 파워
	_effect->SetFloat("specPower", 10);

	//텍스쳐 세팅
	_effect->SetTexture("DiffuseTex", _diffuseTex);
	_effect->SetTexture("NormalTex", _normalTex);

	//테크닉 세팅
	_effect->SetTechnique("MyShader");

	//셰이더로 쿼드렌더
	UINT iPassNum = 0;
	//이펙트로 렌더링시작
	_effect->Begin(&iPassNum, 0);

	//패스 숫자만큼 for문돌린다
	for (UINT i = 0; i < iPassNum; i++)
	{
		_effect->BeginPass(i);
		renderQuadBumpNormal();
		_effect->EndPass();
	}

	//이펙트로 렌더종료
	_effect->End();
	_t.renderGizmo();
	//_laser->render();

}
//카메라 위치
//포탈건 라이트