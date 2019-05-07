#include "stdafx.h"
#include "stage3.h"


HRESULT stage3::init(void)
{
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.024f, 0.024f, 0.024f);
	
	_object = new baseObject;
	_object->setMesh(RM_XMESH->addResource("Resources/map/stage3.X", matScale));
	_object->setActive(true);
	_light = new lightDirection;
	_light->_color = D3DXCOLOR(1, 1, 1, 1);
	_light->_intensity = 1.2f;
	_light->_transform.setRotateWorld(0, D3DXToRadian(270), 0);
	this->setBoundBox();
	return S_OK;
}

void stage3::release(void)
{
	SAFE_DELETE(_light);
}

void stage3::render(void)
{
	xMeshStatic::setTechniqueName("Toon");
	xMeshStatic::setBaseLight(_light);
	_object->render();
}

void stage3::update(void)
{
}

void stage3::setBoundBox(void)
{
	//입구 엘베 뒤
	mapBoundBox box0;
	box0.isPortalAble = false;
	box0.box.setBound(&D3DXVECTOR3(-0.8f, 1.4f, 20), &D3DXVECTOR3(1.875f, 1.5f, 0.15f));
	_collision.push_back(box0);

	//입구 엘베 앞 왼쪽
	mapBoundBox box1;
	box1.isPortalAble = false;
	box1.box.setBound(&D3DXVECTOR3(-3.1f, 1.5f, 15.8f), &D3DXVECTOR3(0.75f, 1.5f, 4.25f));
	_collision.push_back(box1);

	//입구 엘베 앞 오른쪽
	mapBoundBox box2;
	box2.isPortalAble = false;
	box2.box.setBound(&D3DXVECTOR3(1.5f, 1.5f, 15.8f), &D3DXVECTOR3(0.75f, 1.5f, 4.25f));
	_collision.push_back(box2);
	
	//입구 엘베 왼쪽 구멍
	mapBoundBox box3;
	box3.isPortalAble = false;
	box3.box.setBound(&D3DXVECTOR3(-3.7f, 1.5f, 10.1f), &D3DXVECTOR3(0.15f, 1.5f, 1.5f));
	_collision.push_back(box3);

	//입구 엘베 오른쪽 구멍
	mapBoundBox box4;
	box4.isPortalAble = false;
	box4.box.setBound(&D3DXVECTOR3(2.4f, 1.5f, 10.1f), &D3DXVECTOR3(0.15f, 1.5f, 1.5f));
	_collision.push_back(box4);

	//입구 엘베 바깥 왼쪽
	mapBoundBox box5;
	box5.isPortalAble = false;
	box5.box.setBound(&D3DXVECTOR3(-6.6f, 3.0f, 2.1f), &D3DXVECTOR3(4.2f, 3.0f, 6.5f));
	_collision.push_back(box5);

	//입구 엘베 바깥 오른쪽
	mapBoundBox box6;
	box6.isPortalAble = false;
	box6.box.setBound(&D3DXVECTOR3(2.28f, 1.5f, 4.1f), &D3DXVECTOR3(1.5f, 1.5f, 4.6f));
	_collision.push_back(box6);

	//ㄱ 자 벽
	mapBoundBox box7;
	box7.isPortalAble = false;
	box7.box.setBound(&D3DXVECTOR3(1, 3.0f, -4), &D3DXVECTOR3(3.3f, 3.0f, 0.3f));
	_collision.push_back(box7);

	//입구 위
	mapBoundBox box8;
	box8.isPortalAble = false;
	box8.box.setBound(&D3DXVECTOR3(0.2f, 3.6f, 5.5f), &D3DXVECTOR3(3.8f, 0.8f, 9.4f));
	_collision.push_back(box8);

	//입구 아래
	mapBoundBox box9;
	box9.isPortalAble = false;
	box9.box.setBound(&D3DXVECTOR3(0.2f, 0, 5.5f), &D3DXVECTOR3(3.8f, 0.15f, 10.8f));
	_collision.push_back(box9);
	
	//문 밑
	mapBoundBox box10;
	box10.isPortalAble = true;
	box10.dir = BOUNDBOXDIR_UP;
	box10.trans.lookDirection(D3DXVECTOR3(0, -1, 0));
	box10.trans.setWorldPosition(10.4f, 0, 2.4f);
	box10.box.setBound(&D3DXVECTOR3(10.4f, 0 , 2.4f), &D3DXVECTOR3(6.1f, 0.15f, 6.1f));
	_collision.push_back(box10);

	//문 위
	mapBoundBox box11;
	box11.isPortalAble = true;
	box11.dir = BOUNDBOXDIR_DOWN;
	box11.trans.lookDirection(D3DXVECTOR3(0, 1, 0));
	box11.trans.setWorldPosition(10.4f, 6.13f, 3.9f);
	box11.box.setBound(&D3DXVECTOR3(10.4f, 6.13f, 3.9f), &D3DXVECTOR3(6.1f, 0.15f, 7.6f));
	_collision.push_back(box11);

	//문 위벽
	mapBoundBox box12;
	box12.isPortalAble = true;
	box12.dir = BOUNDBOXDIR_BACK;
	box12.trans.lookDirection(D3DXVECTOR3(-1, 0, 0));
	box12.trans.setWorldPosition(4.2f, 4.4f, -2.15f);
	box12.box.setBound(&D3DXVECTOR3(4.2f, 4.4f, -2.15f), &D3DXVECTOR3(0.15f, 1.45f, 1.5f));
	_collision.push_back(box12);

	//문 왼쪽 벽
	mapBoundBox box13;
	box13.isPortalAble = true;
	box13.dir = BOUNDBOXDIR_BACK;
	box13.trans.lookDirection(D3DXVECTOR3(-1, 0, 0));
	box13.trans.setWorldPosition(4.2f, 3.0f, 5.5f);
	box13.box.setBound(&D3DXVECTOR3(4.2f, 3.0f, 5.5f), &D3DXVECTOR3(0.15f, 2.9f, 6.1f));
	_collision.push_back(box13);

	//문 오른쪽
	mapBoundBox box14;
	box14.isPortalAble = true;
	box14.dir = BOUNDBOXDIR_RIGHT;
	box14.trans.lookDirection(D3DXVECTOR3(0, 0, -1));
	box14.trans.setWorldPosition(10.45f, 3.1f, -3.8f);
	box14.box.setBound(&D3DXVECTOR3(10.45f, 3.1f, -3.8f), &D3DXVECTOR3(6.1f, 2.9f, 0.15f));
	_collision.push_back(box14);

	//문 반대편 벽
	mapBoundBox box15;
	box15.isPortalAble = true;
	box15.dir = BOUNDBOXDIR_FORWARD;
	box15.trans.lookDirection(D3DXVECTOR3(1, 0, 0));
	box15.trans.setWorldPosition(16.7f, 3.0f, 2.4f);
	box15.box.setBound(&D3DXVECTOR3(16.7f, 3.0f, 2.4f), &D3DXVECTOR3(0.15f, 2.9f, 6.1f));
	_collision.push_back(box15);

	//계단 밑쪽 벽
	mapBoundBox box16;
	box16.isPortalAble = true;
	box16.dir = BOUNDBOXDIR_LEFT;
	box16.trans.lookDirection(D3DXVECTOR3(0, 0, 1));
	box16.trans.setWorldPosition(10.45f, 1.4f, 8.65f);
	box16.box.setBound(&D3DXVECTOR3(10.45f, 1.25f, 8.65f), &D3DXVECTOR3(6.1f, 1.5f, 0.15f));
	_collision.push_back(box16);

	//계단 밑
	mapBoundBox box17;
	box17.isPortalAble = true;
	box17.dir = BOUNDBOXDIR_UP;
	box17.trans.lookDirection(D3DXVECTOR3(0, -1, 0));
	box17.trans.setWorldPosition(11.95f, 2.8f, 10.05f);
	box17.box.setBound(&D3DXVECTOR3(11.95f, 2.8f, 10.05f), &D3DXVECTOR3(7.6f, 0.15f, 1.5f));
	_collision.push_back(box17);

	//계단 위쪽 벽
	mapBoundBox box18;
	box18.isPortalAble = true;
	box18.dir = BOUNDBOXDIR_LEFT;
	box18.trans.lookDirection(D3DXVECTOR3(0, 0, 1));
	box18.trans.setWorldPosition(11.95f, 4.4f, 11.72f);
	box18.box.setBound(&D3DXVECTOR3(11.95f, 4.4f, 11.72f), &D3DXVECTOR3(7.6f, 1.5f, 0.15f));
	_collision.push_back(box18);

	//외길 오른쪽 벽
	mapBoundBox box19;
	box19.isPortalAble = false;
	box19.box.setBound(&D3DXVECTOR3(17.02f, 3.0f, 2.4f), &D3DXVECTOR3(0.15f, 2.9f, 6.1f));
	_collision.push_back(box19);

	//외길 왼쪽 벽
	mapBoundBox box20;
	box20.isPortalAble = false;
	box20.box.setBound(&D3DXVECTOR3(19.78f, 3.0f, 2.4f), &D3DXVECTOR3(0.15f, 2.9f, 9.1f));
	_collision.push_back(box20);

	//외길 바깥 오른쪽
	mapBoundBox box21;
	box21.isPortalAble = false;
	box21.box.setBound(&D3DXVECTOR3(10.45f, 3.1f, -4.12f), &D3DXVECTOR3(6.1f, 2.9f, 0.15f));
	_collision.push_back(box21);

	//외길 바깥 왼쪽
	mapBoundBox box22;
	box22.isPortalAble = false;
	box22.box.setBound(&D3DXVECTOR3(12.05f, 3.1f, -6.93f), &D3DXVECTOR3(7.6f, 2.9f, 0.15f));
	_collision.push_back(box22);

	//외길 밑
	mapBoundBox box23;
	box23.isPortalAble = false;
	box23.box.setBound(&D3DXVECTOR3(18.1f, 0, 0.8f), &D3DXVECTOR3(1.5f, 0.15f, 7.6f));
	_collision.push_back(box23);

	//외길 위
	mapBoundBox box24;
	box24.isPortalAble = false;
	box24.box.setBound(&D3DXVECTOR3(18.1f, 6.13f, 0.8f), &D3DXVECTOR3(1.5f, 0.15f, 7.6f));
	_collision.push_back(box24);

	//외길 바깥 밑
	mapBoundBox box25;
	box25.isPortalAble = false;
	box25.box.setBound(&D3DXVECTOR3(11.95f, 0, -5.8f), &D3DXVECTOR3(7.6f, 0.15f, 1.5f));
	_collision.push_back(box25);
	
	//외길 입구 밑쪽 벽
	mapBoundBox box26;
	box26.isPortalAble = false;
	box26.box.setBound(&D3DXVECTOR3(18.45f, 1.4f, 8.37f), &D3DXVECTOR3(1.5f, 1.5f, 0.15f));
	_collision.push_back(box26);

	//외길 출구 밑쪽 벽
	mapBoundBox box27;
	box27.isPortalAble = false;
	box27.box.setBound(&D3DXVECTOR3(4.2f, 1.4f, -5.8f), &D3DXVECTOR3(0.15f, 1.5f, 1.5f));
	_collision.push_back(box27);

	//출구 밑쪽
	mapBoundBox box28;
	box28.isPortalAble = false;
	box28.box.setBound(&D3DXVECTOR3(-3.2f, 3.05f, -7.3f), &D3DXVECTOR3(7.6f, 0.15f, 3.0f));
	_collision.push_back(box28);

	//출구 위쪽 벽
	mapBoundBox box29;
	box29.isPortalAble = false;
	box29.box.setBound(&D3DXVECTOR3(-3.2f, 6.05f, -7.3f), &D3DXVECTOR3(7.6f, 0.15f, 3.0f));
	_collision.push_back(box29);

	//출구 왼쪽 외길쪽 벽
	mapBoundBox box30;
	box30.isPortalAble = false;
	box30.box.setBound(&D3DXVECTOR3(4.2f, 4.5f, -8.7f), &D3DXVECTOR3(0.15f, 1.5f, 1.5f));
	_collision.push_back(box30);

	//출구 왼쪽 출구쪽 벽
	mapBoundBox box31;
	box31.isPortalAble = false;
	box31.box.setBound(&D3DXVECTOR3(-6.2f, 4.5f, -8.7f), &D3DXVECTOR3(4.7f, 1.5f, 1.5f));
	_collision.push_back(box31);
	
	//출구 왼쪽 왼쪽벽
	mapBoundBox box32;
	box32.isPortalAble = false;
	box32.box.setBound(&D3DXVECTOR3(1.2f, 4.5f, -10.2f), &D3DXVECTOR3(3.0f, 1.5f, 0.15f));
	_collision.push_back(box32);
}
