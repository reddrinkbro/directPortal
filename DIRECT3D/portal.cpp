#include "stdafx.h"
#include "portal.h"

//파랑 포탈
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

void bluePortal::release(void)
{
	releaseVI();
	SAFE_RELEASE(_bluePortal);
}

void bluePortal::render(void)
{
	if (!_isSet) return;
	_transform->setDeviceWorld();
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
	_effect->SetTexture("MyTex", _bluePortal);

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
}
//주황 포탈
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

void orangePortal::release(void)
{
	releaseVI();
	SAFE_RELEASE(_orangePortal);
}

void orangePortal::render(void)
{
	if (!_isSet) return;
	_transform->setDeviceWorld();
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
	_effect->SetTexture("MyTex", _orangePortal);

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
}