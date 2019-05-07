#include "stdafx.h"
#include "gameNode.h"

HRESULT gameNode::init(void)
{
	DEVICEMANAGER->CreateDevice();		// ����̽� ����
	KEYMANAGER->init();					// Ű�Ŵ��� �ʱ�ȭ
	TIMEMANAGER->init();				// Ÿ�ӸŴ��� �ʱ�ȭ
	SCENEMANAGER->init();				// ���Ŵ��� �ʱ�ȭ
	FONTMANAGER->init();				// ��Ʈ�Ŵ��� �ʱ�ȭ
	GIZMOMANAGER->init();				// �����Ŵ��� �ʱ�ȭ
	SPRITEMANAGER->init();				// ��������Ʈ �Ŵ��� �ʱ�ȭ
	SOUNDMANAGER->init();				// ���� �Ŵ��� �ʱ�ȭ
										//ȯ�汸 ����Ʈ �ʱ�ȭ
	_environmentEffect = RM_SHADER->addResource("Resources/Shader/EnvironmentCube.fx");
	//����ī�޶� �ʱ�ȭ
	_mainCamera = new camera;

	//����ī�޶� RenderToTexture �غ�
	_mainCamera->readyRenderToTexture(WINSIZEX, WINSIZEY);

	//���⼺ ������ ī�޶� �� �����Ѵ�.
	_directionLightCamera = new camera;

	//���⼺���� �ڽĿ� ���δ�.
	//_directionLightCamera->attachTo(_sceneBaseDirectionLight->_transform );

	//�׸��� �Ÿ�
	_shadowDistance = 100.0f;

	//ī�޶��� ��������� �ٲ۴�...	
	_directionLightCamera->setOrtho(true);
	_directionLightCamera->setCamNear(0.1f);
	_directionLightCamera->setCamFar(_shadowDistance * 2.0f);
	_directionLightCamera->setAspect(1);
	_directionLightCamera->setOrthoSize(_shadowDistance * 1.5f);	//����ũ��� �׸���ũ���

	//���⼺���� ī�޶��� RenderToTexture �غ�
	_directionLightCamera->readyShadowTexture(4096);

	_environmentTexture = NULL;
	this->addSound();
	return S_OK;
}

void gameNode::release(void)
{
	// ����̽� �� �̱��� ����
	DEVICEMANAGER->ReleaseDevice();
	DEVICEMANAGER->releaseSingleton();
	// Ű�Ŵ��� �̱��� ����
	KEYMANAGER->release();
	KEYMANAGER->releaseSingleton();
	// Ÿ�ӸŴ��� �̱��� ����
	TIMEMANAGER->release();
	TIMEMANAGER->releaseSingleton();
	// ���Ŵ��� �̱��� ����
	SCENEMANAGER->release();
	SCENEMANAGER->releaseSingleton();
	// ��Ʈ�Ŵ��� �̱�������
	FONTMANAGER->release();
	FONTMANAGER->releaseSingleton();
	// �����Ŵ��� �̱�������
	GIZMOMANAGER->release();
	GIZMOMANAGER->releaseSingleton();
	// ��������Ʈ �Ŵ��� �̱��� ����
	SPRITEMANAGER->release();
	SPRITEMANAGER->releaseSingleton();
	//���� �Ŵ��� �̱��� ����
	SOUNDMANAGER->release();
	SOUNDMANAGER->releaseSingleton();
}

void gameNode::update(void)
{
	//_mainCamera->defaultControl();
}

void gameNode::render(void)
{
	//����ī�޶� RTT�غ�
	//_mainCamera->renderTextureBegin(0x00101010);
	//_objectCamera->renderTextureBegin(0x00101010);
	//ȯ�� ����
	//this->renderEnvironment();

	/*�����Լ� ����*/
	//this->render();

	//����ī�޶� RTT����
	//_mainCamera->renderTextureEnd();
	//_objectCamera->renderTextureEnd();
	////����Ʈ ����Ʈ ����
	//_postEffect->SetTechnique("Base");
	//_postEffect->SetTexture("screenTex", _mainCamera->getRenderTexture());
	//float pixelU = 1.0f / WINSIZEX;
	//float pixelV = 1.0f / WINSIZEY;
	//_postEffect->SetFloat("pixelSizeU", pixelU);
	//_postEffect->SetFloat("pixelSizeV", pixelV);
	//_postEffect->SetFloat("pixelHalfSizeU", pixelU * 0.5f);
	//_postEffect->SetFloat("pixelHalfSizeV", pixelV * 0.5f);

	////����Ʈ ����Ʈ ����
	//UINT iPass;
	//_postEffect->Begin(&iPass, 0);
	//for (UINT i = 0; i < iPass; i++)
	//{
	//	_postEffect->BeginPass(i);
	//	device->SetFVF(SCENE_VERTEX::FVF);
	//	device->DrawIndexedPrimitiveUP(
	//		D3DPT_TRIANGLELIST,
	//		0,
	//		4,
	//		2,
	//		_scenePlaneIndex,
	//		D3DFMT_INDEX16,
	//		_scenePlaneVertex,
	//		sizeof(SCENE_VERTEX));
	//	_postEffect->EndPass();
	//}
	//_postEffect->End();

	//SPRITEMANAGER->beginSpriteRender();
	//this->renderSprite();
	//SPRITEMANAGER->endSpriteRender();
}

void gameNode::renderEnvironment(void)
{
	//ȯ�� �� �ε��Ȱ� ���ٸ� �����ض�
	if (_environmentTexture == NULL) return;

	//ȯ�� ����Ʈ ����
	_environmentEffect->SetTexture("MyCube_Tex", _environmentTexture);

	//WVP ��Ʈ����
	D3DXMATRIXA16 matWorld = _mainCamera->getFinalMatrix();
	D3DXMATRIXA16 matViewProj = _mainCamera->getViewProjectionMatrix();
	D3DXMATRIXA16 matWVP = matWorld * matViewProj;

	//����Ʈ ����
	_environmentEffect->SetMatrix("matWVP", &matWVP);
	_environmentEffect->SetMatrix("matWorld", &matWorld);

	//���̴� �׷���
	UINT numPass;
	_environmentEffect->Begin(&numPass, 0);		//���̴��� �׸��ٴ� ���� �˸��� pass ���� ��´�.
	for (UINT i = 0; i < numPass; i++)			//���� ����� ����.
	{
		_environmentEffect->BeginPass(i);		//i��° ��������
		_environmemtSphereMesh->DrawSubset(0);	//�� �׷���
		_environmentEffect->EndPass();			//i��° ������
	}
	_environmentEffect->End();					//���̴��� �׸��� ��
}

void gameNode::setEnvironment(string cubeFilePath)
{
	//������ ȯ����� ���õǾ������� �𸣴� ���� ���������ְ� ��������
	SAFE_RELEASE(_environmentTexture);

	//ȯ��� �ε�
	D3DXCreateCubeTextureFromFile(device, cubeFilePath.c_str(), &_environmentTexture);

	//ȯ�汸 �ε��Ǿ����� �ʴٸ�....
	if (_environmemtSphereMesh == NULL)
	{
		D3DXCreateSphere(device, 3.0f, 20, 20, &_environmemtSphereMesh, NULL);
	}
}

void gameNode::readyShadowMap(vector<baseObject*>* renderObjects, terrain * pTerrain)
{
	//���⼺������ ���� ī�޶��� Frustum ������Ʈ
	_directionLightCamera->updateMatrix();
	_directionLightCamera->updateFrustum();

	//���̷��Ƕ����� ī�޶� ������ �ֵ鸸 �׸���...
	static vector<baseObject*>	shadowCullObject;
	shadowCullObject.clear();

	for (int i = 0; i < renderObjects->size(); i++)
	{
		//�������� �ȿ� �ִ�?
		if (_directionLightCamera->getFrustum().isInFrustum((*renderObjects)[i]))
		{
			shadowCullObject.push_back((*renderObjects)[i]);
		}
	}

	//������ �� �׸���.
	_directionLightCamera->renderTextureBegin(0xffffffff);

	//�����޽� ����
	xMeshStatic::setCamera(this->_directionLightCamera);
	xMeshStatic::setTechniqueName("CreateShadow");
	//��Ų��޽� ����
	xMeshSkinned::setCamera(this->_directionLightCamera);
	xMeshSkinned::setTechniqueName("CreateShadow");

	for (int i = 0; i < shadowCullObject.size(); i++)
	{
		if (shadowCullObject[i]->_ignoreCreateShadow == false)
		{
			shadowCullObject[i]->render();
		}
	}

	//���� Terrain �� ������ ���� �׷����Ѵٸ�...
	if (pTerrain != NULL)
	{
		pTerrain->renderShadow(_directionLightCamera);
	}

	_directionLightCamera->renderTextureEnd();


	//���� Terrain �� ������ ���� �����Ѵٸ�
	if (pTerrain != NULL)
	{
		pTerrain->getTerrainEffect()->SetTexture("Shadow_Tex", _directionLightCamera->getRenderTexture());
		pTerrain->getTerrainEffect()->SetMatrix("matLightViewProjection", &_directionLightCamera->getViewProjectionMatrix());
	}

	//������ �ؽ���
	xMeshStatic::_xMeshEffect->SetTexture("Shadow_Tex", _directionLightCamera->getRenderTexture());
	//������ ���
	xMeshStatic::_xMeshEffect->SetMatrix("matLightViewProjection", &_directionLightCamera->getViewProjectionMatrix());
	//������ �ؽ���
	xMeshSkinned::_sSkinnedMeshEffect->SetTexture("Shadow_Tex", _directionLightCamera->getRenderTexture());
	//������ ���
	xMeshSkinned::_sSkinnedMeshEffect->SetMatrix("matLightViewProjection", &_directionLightCamera->getViewProjectionMatrix());
}

void gameNode::addSound(void)
{
	SOUNDMANAGER->addSound("manu_backgroundMusic","Resources/sound/portal2_background02.wav", true, true);
	SOUNDMANAGER->addSound("ending_backgroundMusic", "Resources/sound/portal2_want_you_gone.wav", true, true);
	SOUNDMANAGER->addSound("manu_overlap", "Resources/sound/menu_focus.wav", false, false);
	SOUNDMANAGER->addSound("door_open", "Resources/sound/door_open.wav", false, false);
	SOUNDMANAGER->addSound("door_close", "Resources/sound/door_close.wav", false, false);
	SOUNDMANAGER->addSound("elevator_door_open", "Resources/sound/elevator_door_open.wav", false, false);
	SOUNDMANAGER->addSound("elevator_door_close", "Resources/sound/elevator_door_close.wav", false, false);
	SOUNDMANAGER->addSound("panelmover_moving", "Resources/sound/panelmover_moving.wav", false, true);
	SOUNDMANAGER->addSound("portal_enter", "Resources/sound/portal_enter.wav", false, false);
	SOUNDMANAGER->addSound("portal_exit", "Resources/sound/portal_exit.wav", false, false);
	SOUNDMANAGER->addSound("portal_open", "Resources/sound/portal_open.wav", false, false);
	SOUNDMANAGER->addSound("portalgun_shoot_blue", "Resources/sound/portalgun_shoot_blue.wav", false, false);
	SOUNDMANAGER->addSound("portalgun_shoot_orange", "Resources/sound/portalgun_shoot_orange.wav", false, false);
	SOUNDMANAGER->addSound("switch_click", "Resources/sound/switch_click.wav", false, false);
	SOUNDMANAGER->addSound("box_lay", "Resources/sound/metal_grenade_impact_soft.wav", false, false);
	SOUNDMANAGER->addSound("gun_take", "Resources/sound/ding_on.wav", false, false);
	SOUNDMANAGER->addSound("underground_wind", "Resources/sound/ding_on.wav", true, true);
}

LRESULT gameNode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	
	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
