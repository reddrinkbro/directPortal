#include "stdafx.h"
#include "tutorialMap.h"

HRESULT tutorialMap::init(void)
{
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.01f, 0.01f, 0.01f);
	
	_object = new baseObject;
	_object->setMesh(RM_XMESH->addResource("Resources/map/tutorial1.X", matScale));
	_object->setActive(true);

	_light = new lightDirection;
	_light->_color = D3DXCOLOR(1, 1, 1, 1);
	_light->_intensity = 1.2f;
	_light->_transform.setRotateWorld(0, D3DXToRadian(270), 0);
	this->setBoundBox();
	
	return S_OK;
}

void tutorialMap::release(void)
{
	SAFE_DELETE(_light);
}

void tutorialMap::render(void)
{
	xMeshStatic::setTechniqueName("Toon");
	xMeshStatic::setBaseLight(_light);
	_object->render();
}

void tutorialMap::setBoundBox(void)
{
	//위쪽
	mapBoundBox box0;
	box0.dir = BOUNDBOXDIR_DOWN;
	box0.box.setBound(&D3DXVECTOR3(-2.5f, 6.15f, 14.5f), &D3DXVECTOR3(12.0f, 0.25f, 21.5f));
	box0.trans.lookDirection(D3DXVECTOR3(0, 1, 0));
	box0.trans.setWorldPosition(-2.5f, 6.15f, 14.5f);
	box0.isPortalAble = true;

	//입구 뒤
	mapBoundBox box1;
	box1.box.setBound(&D3DXVECTOR3(-14.5f,2.0f,1.5f),&D3DXVECTOR3(0.5f,2.0f,1.5f));
	box1.isPortalAble = false;

	//입구 왼쪽
	mapBoundBox box2;
	box2.box.setBound(&D3DXVECTOR3(-12.5f, 2.0f, -0.1f), &D3DXVECTOR3(1.5f, 2.0f, 0.15f));
	box2.isPortalAble = false;

	//입구오른쪽
	mapBoundBox box3;
	box3.box.setBound(&D3DXVECTOR3(-12.5f, 2.0f, 3.4f), &D3DXVECTOR3(1.5f, 2.0f, 0.15f));
	box3.isPortalAble = false;

	//바닥
	mapBoundBox box4;
	box4.dir = BOUNDBOXDIR_UP;
	box4.box.setBound(&D3DXVECTOR3(-2.5f, -0.35f, 14.5f), &D3DXVECTOR3(12.0f, 0.5f, 21.5f));
	box4.trans.lookDirection(D3DXVECTOR3(0, -1, 0));
	box4.trans.setWorldPosition(-2.5f, -0.35f, 14.5f);
	box4.isPortalAble = true;

	//왼쪽
	mapBoundBox box5;
	box5.box.setBound(&D3DXVECTOR3(-2.5f, 2.0f, -6.5f), &D3DXVECTOR3(12.0f, 2.0f, 0.15f));
	box5.isPortalAble = false;

	//뒤편
	mapBoundBox box6;
	box6.dir = BOUNDBOXDIR_FORWARD;
	box6.box.setBound(&D3DXVECTOR3(8.8f, 3.15f, 14.5f), &D3DXVECTOR3(0.237f, 3.15f, 21.5f));
	box6.trans.lookDirection(D3DXVECTOR3(1, 0, 0));
	box6.trans.setWorldPosition(8.8f, 3.15f, 14.5f);
	box6.isPortalAble = true;

	//앞쪽 왼편
	mapBoundBox box7;
	box7.box.setBound(&D3DXVECTOR3(-10.8f, 2.0f, -3.5f), &D3DXVECTOR3(0.15f, 2.0f, 3.0f));
	box7.isPortalAble = false;

	//앞쪽 오른편
	mapBoundBox box8;
	box8.dir = BOUNDBOXDIR_BACK;
	box8.box.setBound(&D3DXVECTOR3(-10.8f, 3.15f, 20.1f), &D3DXVECTOR3(0.15f, 3.15f, 16.5f));
	box8.trans.lookDirection(D3DXVECTOR3(-1, 0, 0));
	box8.trans.setWorldPosition(-10.7f, 3.15f, 20.1f);
	box8.isPortalAble = true;

	//왼쪽 입구왼편
	mapBoundBox box9;
	box9.box.setBound(&D3DXVECTOR3(-7.5f, 2.0f, 9.8f), &D3DXVECTOR3(3.2f, 2.0f, 0.15f));
	box9.isPortalAble = false;

	//왼쪽 입구 오른편
	mapBoundBox box10;
	box10.box.setBound(&D3DXVECTOR3(3.9f, 2.0f, 9.8f), &D3DXVECTOR3(4.8f, 2.0f, 0.15f));
	box10.isPortalAble = false;

	//뒤쪽 방 왼쪽 벽
	mapBoundBox box11;
	box11.box.setBound(&D3DXVECTOR3(7.1f, 2.0f, 4.9f), &D3DXVECTOR3(1.5f, 2.0f, 0.15f));
	box11.isPortalAble = false;

	//뒤쪽 방 오른쪽 벽
	mapBoundBox box12;
	box12.box.setBound(&D3DXVECTOR3(7.1f, 2.0f, -1.65f), &D3DXVECTOR3(1.5f, 2.0f, 0.15f));
	box12.isPortalAble = false;

	//왼쪽 방 오른쪽 벽
	mapBoundBox box13;
	box13.box.setBound(&D3DXVECTOR3(-4.3f, 2.0f, -4.8f), &D3DXVECTOR3(0.15f, 2.0f, 1.5f));
	box12.isPortalAble = false;

	//왼쪽 방 왼쪽 벽
	mapBoundBox box14;
	box14.box.setBound(&D3DXVECTOR3(2.25f, 2.0f, -4.8f), &D3DXVECTOR3(0.15f, 2.0f, 1.5f));
	box14.isPortalAble = false;

	//오른쪽 방 왼쪽 벽
	mapBoundBox box15;
	box15.box.setBound(&D3DXVECTOR3(-4.3f, 2.0f, 8.0f), &D3DXVECTOR3(0.15f, 2.0f, 1.5f));
	box15.isPortalAble = false;

	//오른쪽 방 오른쪽 벽
	mapBoundBox box16;
	box16.box.setBound(&D3DXVECTOR3(2.25f, 2.0f, 8.0f), &D3DXVECTOR3(0.15f, 2.0f, 1.5f));
	box16.isPortalAble = false;

	//뒤쪽 방 입구
	mapBoundBox box17;
	box17.box.setBound(&D3DXVECTOR3(5.45f, 2.0f, 1.7f), &D3DXVECTOR3(0.15f, 2.0f, 3.0f));
	box17.isPortalAble = false;

	//왼쪽 방 입구
	mapBoundBox box18;
	box18.box.setBound(&D3DXVECTOR3(-1.0f, 2.0f, -3.15f), &D3DXVECTOR3(3.0f, 2.0f, 0.15f));
	box18.isPortalAble = false;

	//오른쪽 방 입구
	mapBoundBox box19;
	box19.box.setBound(&D3DXVECTOR3(-1.0f, 2.0f, 6.45f), &D3DXVECTOR3(3.0f, 2.0f, 0.15f));
	box19.isPortalAble = false;

	//연결다리 왼쪽
	mapBoundBox box20;
	box20.box.setBound(&D3DXVECTOR3(-4.3f, 1.5f, 14.7f), &D3DXVECTOR3(0.15f, 2.0f, 4.7f));
	box20.isPortalAble = false;

	//연결다리 오른쪽
	mapBoundBox box21;
	box21.box.setBound(&D3DXVECTOR3(-0.9f, 1.5f, 14.7f), &D3DXVECTOR3(0.15f, 2.0f, 4.7f));
	box21.trans.lookDirection(D3DXVECTOR3(0, 0, -1));
	box21.isPortalAble = false;

	//건너편 입구 왼쪽
	mapBoundBox box22;
	box22.dir = BOUNDBOXDIR_RIGHT;
	box22.box.setBound(&D3DXVECTOR3(-7.5f, 3.15f, 19.5f), &D3DXVECTOR3(3.2f, 3.15f, 0.2f));
	box22.trans.lookDirection(D3DXVECTOR3(0, 0, -1));
	box22.trans.setWorldPosition(-7.5f, 3.15f, 19.6f);
	box22.isPortalAble = true;

	//건너편 입구 오른쪽
	mapBoundBox box23;
	box23.dir = BOUNDBOXDIR_RIGHT;
	box23.box.setBound(&D3DXVECTOR3(3.9f, 3.15f, 19.5f), &D3DXVECTOR3(4.8f, 3.15f, 0.2f));
	box23.trans.lookDirection(D3DXVECTOR3(0, 0, -1));
	box23.trans.setWorldPosition(3.9f, 3.15f, 19.6f);
	box23.isPortalAble = true;

	//엘리베이터 입구 왼쪽
	mapBoundBox box24;
	box24.dir = BOUNDBOXDIR_LEFT;
	box24.box.setBound(&D3DXVECTOR3(-9.0f, 3.15f, 36.1f), &D3DXVECTOR3(1.5f, 3.15f, 0.25f));
	box24.trans.lookDirection(D3DXVECTOR3(0, 0, 1));
	box24.trans.setWorldPosition(-9.0f, 3.15f, 36);
	box24.isPortalAble = true;

	//엘리베이터 입구 오른쪽
	mapBoundBox box25;
	box25.dir = BOUNDBOXDIR_LEFT;
	box25.box.setBound(&D3DXVECTOR3(2.1f, 3.15f, 36.1f), &D3DXVECTOR3(6.5f, 3.15f, 0.25f));
	box25.trans.lookDirection(D3DXVECTOR3(0, 0, 1));
	box25.trans.setWorldPosition(2.1f, 3.15f, 36);
	box25.isPortalAble = true;

	//엘리베이터 안 입구 아래쪽
	mapBoundBox box26;
	box26.dir = BOUNDBOXDIR_UP;
	box26.box.setBound(&D3DXVECTOR3(-6.0f, 3.2f, 41.0f), &D3DXVECTOR3(1.5f, 0.15f, 5.0f));
	box26.trans.lookDirection(D3DXVECTOR3(0, -1, 0));
	box26.trans.setWorldPosition(-6.0f, 3.3f, 41.0f);
	box26.isPortalAble = true;

	//엘리베이터 안 입구 오른쪽
	mapBoundBox box27;
	box27.dir = BOUNDBOXDIR_FORWARD;
	box27.box.setBound(&D3DXVECTOR3(-4.3f, 4.8f, 39.4f), &D3DXVECTOR3(0.15f, 1.5f, 3.25f));
	box27.trans.lookDirection(D3DXVECTOR3(1, 0, 0));
	box27.trans.setWorldPosition(-4.4f, 4.8f, 39.4f);
	box27.isPortalAble = true;

	//엘리베이터 안 입구 왼쪽
	mapBoundBox box28;
	box28.dir = BOUNDBOXDIR_BACK;
	box28.box.setBound(&D3DXVECTOR3(-7.3f, 4.8f, 39.4f), &D3DXVECTOR3(0.15f, 1.5f, 3.25f));
	box28.trans.lookDirection(D3DXVECTOR3(-1, 0, 0));
	box28.trans.setWorldPosition(-7.3f, 4.8f, 39.4f);
	box28.isPortalAble = true;

	//엘리베이터 입구 위쪽
	mapBoundBox box29;
	box29.dir = BOUNDBOXDIR_DOWN;
	box29.box.setBound(&D3DXVECTOR3(-6.0f, 6.15f, 39.5f), &D3DXVECTOR3(1.5f, 0.2f, 3.25f));
	box29.trans.lookDirection(D3DXVECTOR3(0, 1, 0));
	box29.trans.setWorldPosition(-6.0f, 6.15f, 39.5f);
	box29.isPortalAble = true;

	//계단 오른쪽
	mapBoundBox box30;
	box30.box.setBound(&D3DXVECTOR3(6.5f, 1.58f, 30.0f), &D3DXVECTOR3(0.7f, 1.8f, 2.1595f));
	box30.isPortalAble = false;

	//계단 왼쪽
	mapBoundBox box31;
	box31.box.setBound(&D3DXVECTOR3(4.0f, 1.58f, 30.0f), &D3DXVECTOR3(0.7f, 1.8f, 2.1595f));
	box31.isPortalAble = false;

	//계단 왼쪽 난간
	mapBoundBox box32;
	box32.box.setBound(&D3DXVECTOR3(3.0f, 2.0f, 32.0f), &D3DXVECTOR3(0.07f, 3.0f, 4.5f));
	box32.isPortalAble = false;

	//계단 오른쪽 난간
	mapBoundBox box33;
	box33.box.setBound(&D3DXVECTOR3(7.5f, 2.0f, 32.0f), &D3DXVECTOR3(0.07f, 3.0f, 4.5f));
	box33.isPortalAble = false;

	//계단 가운데 난간
	mapBoundBox box34;
	box34.box.setBound(&D3DXVECTOR3(5.2f, 2.0f, 30.0f), &D3DXVECTOR3(0.2f, 3.0f, 3.0f));
	box34.isPortalAble = false;

	//계단 난간
	mapBoundBox box35;
	box35.box.setBound(&D3DXVECTOR3(5.2f, 1.99f, 34.5f), &D3DXVECTOR3(2.1f, 1.0f, 1.6f));
	box35.isPortalAble = false;

	//엘레베이터 왼쪽
	mapBoundBox box36;
	box36.box.setBound(&D3DXVECTOR3(-7.3f, 4.8f, 45.0f), &D3DXVECTOR3(0.15f, 1.5f, 2.0f));
	box36.isPortalAble = false;

	//엘레베이터 오른쪽
	mapBoundBox box37;
	box37.box.setBound(&D3DXVECTOR3(-4.3f, 4.8f, 45.0f), &D3DXVECTOR3(0.15f, 1.5f, 2.0f));
	box37.isPortalAble = false;

	//연결 다리 위쪽
	mapBoundBox box38;
	box38.dir = BOUNDBOXDIR_RIGHT;
	box38.box.setBound(&D3DXVECTOR3(-2.5f, 4.6f, 19.5f), &D3DXVECTOR3(1.6f, 1.5f, 0.2f));
	box38.trans.lookDirection(D3DXVECTOR3(0, 0, -1));
	box38.trans.setWorldPosition(-2.5f, 4.6f, 19.6f);
	box38.isPortalAble = true;

	//엘베입구 아래
	mapBoundBox box39;
	box39.dir = BOUNDBOXDIR_LEFT;
	box39.box.setBound(&D3DXVECTOR3(-5.8f, 1.5f, 36.1f), &D3DXVECTOR3(1.6f, 1.5f, 0.2f));
	box39.trans.lookDirection(D3DXVECTOR3(0, 0, 1));
	box39.trans.setWorldPosition(-5.8f, 1.5f, 36);
	box39.isPortalAble = true;
	
	_collision.push_back(box0);
	_collision.push_back(box1);
	_collision.push_back(box2);
	_collision.push_back(box3);
	_collision.push_back(box4);
	_collision.push_back(box5);
	_collision.push_back(box6);
	_collision.push_back(box7);
	_collision.push_back(box8);
	_collision.push_back(box9);
	_collision.push_back(box10);
	_collision.push_back(box11);
	_collision.push_back(box12);
	_collision.push_back(box13);
	_collision.push_back(box14);
	_collision.push_back(box15);
	_collision.push_back(box16);
	_collision.push_back(box17);
	_collision.push_back(box18);
	_collision.push_back(box19);
	_collision.push_back(box20);
	_collision.push_back(box21);
	_collision.push_back(box22);
	_collision.push_back(box23);
	_collision.push_back(box24);
	_collision.push_back(box25);
	_collision.push_back(box26);
	_collision.push_back(box27);
	_collision.push_back(box28);
	_collision.push_back(box29);
	_collision.push_back(box32);
	_collision.push_back(box33);
	_collision.push_back(box34);
	_collision.push_back(box35);
	_collision.push_back(box36);
	_collision.push_back(box37);
	_collision.push_back(box38);
	_collision.push_back(box39);

	_stairsCollision.push_back(box30);
	_stairsCollision.push_back(box31);
}

tutorialMap::tutorialMap()
{
}


tutorialMap::~tutorialMap()
{
}
