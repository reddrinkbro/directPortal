#include "stdafx.h"
#include "mainGame.h"
//==================================================================
//		## �ʱ�ȭ ## init(void)
//==================================================================
HRESULT mainGame::init(void)
{
	gameNode::init();


	//���߰�	
	SCENEMANAGER->addScene("����ȭ��", new mainScene);
	SCENEMANAGER->addScene("Ʃ�丮��1", new tutorialScene_1);
	SCENEMANAGER->addScene("Ʃ�丮��2", new tutorialScene_2);
	SCENEMANAGER->addScene("��������1", new stage1Scene);
	SCENEMANAGER->addScene("��������2", new stage2Scene);
	SCENEMANAGER->addScene("��������3", new stage3Scene);
	SCENEMANAGER->addScene("����", new endingScene);
	//���ʱ�ȭ
	SCENEMANAGER->changeScene("����ȭ��");

	return S_OK;
}

//==================================================================
//		## ���� ## release(void)
//==================================================================
void mainGame::release(void)
{
	gameNode::release();
}

//==================================================================
//		## ������Ʈ ## update(void)
//==================================================================
void mainGame::update(void)
{
	//���Ŵ��� ������Ʈ
	SCENEMANAGER->update();
}

//==================================================================
//		## ���� ## render(void)
//==================================================================
void mainGame::render(void)
{
	//����̽� ���۸� û���ϱ�
	HRESULT result = device->Clear(
		0,				//û���� ������ D3D��Ʈ �迭 ���� (��ü Ŭ���� 0)
		NULL,			//û���� ������ D3D��Ʈ �迭 ������ (��ü Ŭ���� NULL)
		D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, //û���� ���� �÷���(�÷�, ���ٽ�, ���̹���)
		0xff202020,		//û���� �� ä���� ���� (0xAARRGGBB)
		1.0f,			//���̹��۸� û���� �� (0 ~ 1, 0�� ī�޶󿡼� �� ������, 1�� ī�޶󿡼� ���� �ִ�)
		0);				//���ٽ� ���۸� ä�ﰪ

						//����̽� ���۸� ������ û���ߴٸ�
	if(_isEnding) SCENEMANAGER->render();
	else if (SUCCEEDED(result))
	{
		//����̽� ������ ����
		device->BeginScene();
		//���Ŵ��� ����
		SCENEMANAGER->render();
		//����̽� ������ ����
		device->EndScene();
		//�������� ���۳����� ȭ�鿡 ����
		device->Present(NULL, NULL, NULL, NULL);
	}
}