#include "stdafx.h"
#include "gameNode.h"

HRESULT gameNode::init(void)
{
	DEVICEMANAGER->CreateDevice();		// 디바이스 생성
	KEYMANAGER->init();					// 키매니져 초기화
	TIMEMANAGER->init();				// 타임매니져 초기화
	SCENEMANAGER->init();				// 씬매니져 초기화
	FONTMANAGER->init();				// 폰트매니져 초기화
	GIZMOMANAGER->init();				// 기즈모매니저 초기화
	SPRITEMANAGER->init();				// 스프라이트 매니저 초기화
	SOUNDMANAGER->init();				// 사운드 매니저 초기화
										//환경구 이펙트 초기화
	_environmentEffect = RM_SHADER->addResource("Resources/Shader/EnvironmentCube.fx");
	//메인카메라 초기화
	_mainCamera = new camera;

	//메인카메라 RenderToTexture 준비
	_mainCamera->readyRenderToTexture(WINSIZEX, WINSIZEY);

	//방향성 광원에 카메라 를 생성한다.
	_directionLightCamera = new camera;

	//방향성광원 자식에 붙인다.
	//_directionLightCamera->attachTo(_sceneBaseDirectionLight->_transform );

	//그림자 거리
	_shadowDistance = 100.0f;

	//카메라의 투영방식을 바꾼다...	
	_directionLightCamera->setOrtho(true);
	_directionLightCamera->setCamNear(0.1f);
	_directionLightCamera->setCamFar(_shadowDistance * 2.0f);
	_directionLightCamera->setAspect(1);
	_directionLightCamera->setOrthoSize(_shadowDistance * 1.5f);	//투영크기는 그림자크기로

	//방향성광원 카메라의 RenderToTexture 준비
	_directionLightCamera->readyShadowTexture(4096);

	_environmentTexture = NULL;
	this->addSound();
	return S_OK;
}

void gameNode::release(void)
{
	// 디바이스 및 싱글톤 해제
	DEVICEMANAGER->ReleaseDevice();
	DEVICEMANAGER->releaseSingleton();
	// 키매니져 싱글톤 해제
	KEYMANAGER->release();
	KEYMANAGER->releaseSingleton();
	// 타임매니져 싱글톤 해제
	TIMEMANAGER->release();
	TIMEMANAGER->releaseSingleton();
	// 씬매니져 싱글톤 해제
	SCENEMANAGER->release();
	SCENEMANAGER->releaseSingleton();
	// 폰트매니저 싱글톤해제
	FONTMANAGER->release();
	FONTMANAGER->releaseSingleton();
	// 기즈모매니저 싱글톤해제
	GIZMOMANAGER->release();
	GIZMOMANAGER->releaseSingleton();
	// 스프라이트 매니저 싱글톤 해제
	SPRITEMANAGER->release();
	SPRITEMANAGER->releaseSingleton();
	//사운드 매니저 싱글톤 해제
	SOUNDMANAGER->release();
	SOUNDMANAGER->releaseSingleton();
}

void gameNode::update(void)
{
	//_mainCamera->defaultControl();
}

void gameNode::render(void)
{
	//메인카메라 RTT준비
	//_mainCamera->renderTextureBegin(0x00101010);
	//_objectCamera->renderTextureBegin(0x00101010);
	//환경 렌더
	//this->renderEnvironment();

	/*가상함수 렌더*/
	//this->render();

	//메인카메라 RTT종료
	//_mainCamera->renderTextureEnd();
	//_objectCamera->renderTextureEnd();
	////포스트 이펙트 세팅
	//_postEffect->SetTechnique("Base");
	//_postEffect->SetTexture("screenTex", _mainCamera->getRenderTexture());
	//float pixelU = 1.0f / WINSIZEX;
	//float pixelV = 1.0f / WINSIZEY;
	//_postEffect->SetFloat("pixelSizeU", pixelU);
	//_postEffect->SetFloat("pixelSizeV", pixelV);
	//_postEffect->SetFloat("pixelHalfSizeU", pixelU * 0.5f);
	//_postEffect->SetFloat("pixelHalfSizeV", pixelV * 0.5f);

	////포스트 이펙트 렌더
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
	//환경 맵 로딩된게 없다면 리턴해라
	if (_environmentTexture == NULL) return;

	//환경 이펙트 세팅
	_environmentEffect->SetTexture("MyCube_Tex", _environmentTexture);

	//WVP 매트릭스
	D3DXMATRIXA16 matWorld = _mainCamera->getFinalMatrix();
	D3DXMATRIXA16 matViewProj = _mainCamera->getViewProjectionMatrix();
	D3DXMATRIXA16 matWVP = matWorld * matViewProj;

	//이펙트 세팅
	_environmentEffect->SetMatrix("matWVP", &matWVP);
	_environmentEffect->SetMatrix("matWorld", &matWorld);

	//셰이더 그려라
	UINT numPass;
	_environmentEffect->Begin(&numPass, 0);		//셰이더로 그린다는 것을 알리고 pass 수를 얻는다.
	for (UINT i = 0; i < numPass; i++)			//공정 수대로 돈다.
	{
		_environmentEffect->BeginPass(i);		//i번째 공정시작
		_environmemtSphereMesh->DrawSubset(0);	//구 그려라
		_environmentEffect->EndPass();			//i번째 공정끝
	}
	_environmentEffect->End();					//셰이더로 그리기 끝
}

void gameNode::setEnvironment(string cubeFilePath)
{
	//기존의 환경맵이 세팅되어있을지 모르니 먼저 릴리즈해주고 시작하자
	SAFE_RELEASE(_environmentTexture);

	//환경맵 로딩
	D3DXCreateCubeTextureFromFile(device, cubeFilePath.c_str(), &_environmentTexture);

	//환경구 로딩되어있지 않다면....
	if (_environmemtSphereMesh == NULL)
	{
		D3DXCreateSphere(device, 3.0f, 20, 20, &_environmemtSphereMesh, NULL);
	}
}

void gameNode::readyShadowMap(vector<baseObject*>* renderObjects, terrain * pTerrain)
{
	//방향성광원에 붙은 카메라의 Frustum 업데이트
	_directionLightCamera->updateMatrix();
	_directionLightCamera->updateFrustum();

	//다이렉션라이팅 카메라에 들어오는 애들만 그린다...
	static vector<baseObject*>	shadowCullObject;
	shadowCullObject.clear();

	for (int i = 0; i < renderObjects->size(); i++)
	{
		//프러스텀 안에 있니?
		if (_directionLightCamera->getFrustum().isInFrustum((*renderObjects)[i]))
		{
			shadowCullObject.push_back((*renderObjects)[i]);
		}
	}

	//쉐도우 맵 그린다.
	_directionLightCamera->renderTextureBegin(0xffffffff);

	//고정메쉬 세팅
	xMeshStatic::setCamera(this->_directionLightCamera);
	xMeshStatic::setTechniqueName("CreateShadow");
	//스킨드메쉬 세팅
	xMeshSkinned::setCamera(this->_directionLightCamera);
	xMeshSkinned::setTechniqueName("CreateShadow");

	for (int i = 0; i < shadowCullObject.size(); i++)
	{
		if (shadowCullObject[i]->_ignoreCreateShadow == false)
		{
			shadowCullObject[i]->render();
		}
	}

	//만약 Terrain 도 쉐도우 맵을 그려야한다면...
	if (pTerrain != NULL)
	{
		pTerrain->renderShadow(_directionLightCamera);
	}

	_directionLightCamera->renderTextureEnd();


	//만약 Terrain 도 쉐도우 맵을 세팅한다면
	if (pTerrain != NULL)
	{
		pTerrain->getTerrainEffect()->SetTexture("Shadow_Tex", _directionLightCamera->getRenderTexture());
		pTerrain->getTerrainEffect()->SetMatrix("matLightViewProjection", &_directionLightCamera->getViewProjectionMatrix());
	}

	//쉐도우 텍스쳐
	xMeshStatic::_xMeshEffect->SetTexture("Shadow_Tex", _directionLightCamera->getRenderTexture());
	//쉐도우 행렬
	xMeshStatic::_xMeshEffect->SetMatrix("matLightViewProjection", &_directionLightCamera->getViewProjectionMatrix());
	//쉐도우 텍스쳐
	xMeshSkinned::_sSkinnedMeshEffect->SetTexture("Shadow_Tex", _directionLightCamera->getRenderTexture());
	//쉐도우 행렬
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
