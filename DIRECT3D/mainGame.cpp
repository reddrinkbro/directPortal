#include "stdafx.h"
#include "mainGame.h"
//==================================================================
//		## 초기화 ## init(void)
//==================================================================
HRESULT mainGame::init(void)
{
	gameNode::init();


	//씬추가	
	SCENEMANAGER->addScene("메인화면", new mainScene);
	SCENEMANAGER->addScene("튜토리얼1", new tutorialScene_1);
	SCENEMANAGER->addScene("튜토리얼2", new tutorialScene_2);
	SCENEMANAGER->addScene("스테이지1", new stage1Scene);
	SCENEMANAGER->addScene("스테이지2", new stage2Scene);
	SCENEMANAGER->addScene("스테이지3", new stage3Scene);
	SCENEMANAGER->addScene("엔딩", new endingScene);
	//씬초기화
	SCENEMANAGER->changeScene("메인화면");

	return S_OK;
}

//==================================================================
//		## 해제 ## release(void)
//==================================================================
void mainGame::release(void)
{
	gameNode::release();
}

//==================================================================
//		## 업데이트 ## update(void)
//==================================================================
void mainGame::update(void)
{
	//씬매니져 업데이트
	SCENEMANAGER->update();
}

//==================================================================
//		## 렌더 ## render(void)
//==================================================================
void mainGame::render(void)
{
	//디바이스 버퍼를 청소하기
	HRESULT result = device->Clear(
		0,				//청소할 영역의 D3D렉트 배열 개수 (전체 클리어 0)
		NULL,			//청소할 영역의 D3D렉트 배열 포인터 (전체 클리어 NULL)
		D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, //청소할 버퍼 플래그(컬러, 스텐실, 깊이버퍼)
		0xff202020,		//청소한 후 채워질 색상 (0xAARRGGBB)
		1.0f,			//깊이버퍼를 청소할 값 (0 ~ 1, 0이 카메라에서 젤 가깝고, 1이 카메라에서 제일 멀다)
		0);				//스텐실 버퍼를 채울값

						//디바이스 버퍼를 깨끗이 청소했다면
	if(_isEnding) SCENEMANAGER->render();
	else if (SUCCEEDED(result))
	{
		//디바이스 렌더링 시작
		device->BeginScene();
		//씬매니져 렌더
		SCENEMANAGER->render();
		//디바이스 렌더링 종료
		device->EndScene();
		//렌더링된 버퍼내욜을 화면에 전송
		device->Present(NULL, NULL, NULL, NULL);
	}
}