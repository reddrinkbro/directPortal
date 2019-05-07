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

	// fx 파일로부터 이펙트 객체 로딩	D3DXCreateEffectFromFile()
	D3DXCreateEffectFromFile(
		device,											// 디바이스
		"Resources/Shader/Texture.fx",					// 파일 이름
		NULL,				// 쉐이더를 컴파일할 때 추가 사용할 #define 정의
		NULL,				// 쉐이더를 컴파일할 때 #include 사용 할 플래그
		shaderFlag,			// 쉐이더 플래그
		NULL,				// 메모리 풀
		&_effect,			// 로딩 될 쉐이더 이펙트 포인터
		&error				// 쉐이더 로딩과 컴파일시 문제가 생기면 에러버퍼에 메시지가 들어감.
	);


	// 문제 발생 시
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
	//알파 블렌딩 세팅
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Texture 의 값과 Diffuse 여기서는 정점컬러의 알파값 을 섞어 최종 출력을 한다.
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	_transform.setDeviceWorld();
	// fx 파일 렌더 전역변수 전부
	//float4x4		matWVP;			//월드 뷰 투영 3단합체 행렬
	D3DXMATRIXA16	matWorld, matView, matProjection;
	device->GetTransform(D3DTS_WORLD, &matWorld);
	device->GetTransform(D3DTS_VIEW, &matView);
	device->GetTransform(D3DTS_PROJECTION, &matProjection);

	// 총변환 행렬
	D3DXMATRIXA16 matWVP = matWorld * matView * matProjection;
	_effect->SetMatrix("matWVP", &matWVP);

	// 컬러 셋팅 //float4		vColor;			//출력 컬러
	D3DXVECTOR4		vColor(1, 1, 1, 1);
	_effect->SetVector("vColor", &vColor);

	// 
	//float		tilingX;		//가로 타일수
	//float		tilingY;		//세로 타일수
	_effect->SetFloat("tilingX", 1.0f);
	_effect->SetFloat("tilingY", 1.0f);

	// 텍스쳐 
	//texture MyTex;
	_effect->SetTexture("MyTex", _laser);

	// 테크닉 셋팅
	_effect->SetTechnique("MyShader");

	// 쉐이더로 렌더
	UINT iPassNum = 0;
	// 이펙트로 렌더링 시작
	_effect->Begin(&iPassNum, 0);

	for (UINT i = 0; i < iPassNum; i++)
	{
		_effect->BeginPass(i);
		renderQuadCube();
		_effect->EndPass();
	}

	// 이펙트로 렌더 종료
	_effect->End();
	//파티클 그리고 난후 후처리
	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	device->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	device->SetRenderState(D3DRS_ZWRITEENABLE, true);
	device->SetTexture(0, NULL);
}
