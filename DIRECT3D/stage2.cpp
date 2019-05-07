#include "stdafx.h"
#include "stage2.h"

HRESULT stage2::init(void)
{
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.025f, 0.025f, 0.025f);

	_object = new baseObject;
	_object->setMesh(RM_XMESH->addResource("Resources/map/stage2.X", matScale));
	_object->setActive(true);

	_light = new lightDirection;
	_light->_color = D3DXCOLOR(1, 1, 1, 1);
	_light->_intensity = 1.2f;
	_light->_transform.setRotateWorld(0, D3DXToRadian(270), 0);
	this->setBoundBox();
	return S_OK;
}

void stage2::release(void)
{
	SAFE_DELETE(_light);
}

void stage2::render(void)
{
	xMeshStatic::setTechniqueName("Toon");
	xMeshStatic::setBaseLight(_light);
	_object->render();
}

void stage2::setBoundBox(void)
{
	//입구 엘베 뒤
	mapBoundBox box0;
	box0.isPortalAble = false;
	box0.box.setBound(&D3DXVECTOR3(-0.25f, 1.5f, 20.625f), &D3DXVECTOR3(1.875f, 1.5f, 0.15f));
	_collision.push_back(box0);

	//입구 엘베 앞 왼쪽
	mapBoundBox box1;
	box1.isPortalAble = false;
	box1.box.setBound(&D3DXVECTOR3(-2.625f, 1.5f, 16.25f), &D3DXVECTOR3(0.75f, 1.5f, 4.25f));
	_collision.push_back(box1);

	//입구 엘베 앞 오른쪽
	mapBoundBox box2;
	box2.isPortalAble = false;
	box2.box.setBound(&D3DXVECTOR3(2.125f, 1.5f, 16.25f), &D3DXVECTOR3(0.75f, 1.5f, 4.25f));
	_collision.push_back(box2);

	//입구 엘베 구멍 왼쪽
	mapBoundBox box3;
	box3.isPortalAble = false;
	box3.box.setBound(&D3DXVECTOR3(-3.3f, 1.5f, 10.5f), &D3DXVECTOR3(0.15f, 1.5f, 1.5f));
	_collision.push_back(box3);

	//입구 엘베 구멍 오른쪽
	mapBoundBox box4;
	box4.isPortalAble = false;
	box4.box.setBound(&D3DXVECTOR3(2.95f, 1.5f, 10.5f), &D3DXVECTOR3(0.15f, 1.5f, 1.5f));
	_collision.push_back(box4);

	//입구 엘베 바깥 왼쪽
	mapBoundBox box5;
	box5.isPortalAble = false;
	box5.box.setBound(&D3DXVECTOR3(-2.6f, 1.5f, 2.4f), &D3DXVECTOR3(0.7f, 1.5f, 6.5f));
	_collision.push_back(box5);

	//입구 엘베 바깥 오른쪽
	mapBoundBox box6;
	box6.isPortalAble = false;
	box6.box.setBound(&D3DXVECTOR3(2.875f, 1.5f, 4.125f), &D3DXVECTOR3(1.5f, 1.5f, 4.75f));
	_collision.push_back(box6);

	//ㄱ 자 벽
	mapBoundBox box7;
	box7.isPortalAble = false;
	box7.box.setBound(&D3DXVECTOR3(1.4f, 1.5f, -4), &D3DXVECTOR3(3.4f, 1.5f, 0.15f));
	_collision.push_back(box7);

	//입구 위
	mapBoundBox box8;
	box8.isPortalAble = false;
	box8.box.setBound(&D3DXVECTOR3(0, 3.6f, 5.5f), &D3DXVECTOR3(4.5f, 0.8f, 9.8f));
	_collision.push_back(box8);


	//입구 아래
	mapBoundBox box9;
	box9.isPortalAble = false;
	box9.box.setBound(&D3DXVECTOR3(0, 0, 5.5f), &D3DXVECTOR3(4.7f, 0.15f, 10.8f));
	_collision.push_back(box9);
	
	//문 밑
	mapBoundBox box10;
	box10.isPortalAble = true;
	box10.dir = BOUNDBOXDIR_UP;
	box10.trans.lookDirection(D3DXVECTOR3(0, -1, 0));
	box10.trans.setWorldPosition(6.4f, 0.05f, 5.6f);
	box10.box.setBound(&D3DXVECTOR3(6.4f, 0, -0.7f), &D3DXVECTOR3(1.6f, 0.15f, 3.2f));
	_collision.push_back(box10);

	//위
	mapBoundBox box11;
	box11.isPortalAble = true;
	box11.dir = BOUNDBOXDIR_DOWN;
	box11.trans.lookDirection(D3DXVECTOR3(0, 1, 0));
	box11.trans.setWorldPosition(11.3f, 6.13f, 5.7f);
	box11.box.setBound(&D3DXVECTOR3(11.3f, 6.13f, 5.7f), &D3DXVECTOR3(6.4f, 0.15f, 9.5f));
	_collision.push_back(box11);

	//문 위벽
	mapBoundBox box12;
	box12.isPortalAble = true;
	box12.dir = BOUNDBOXDIR_BACK;
	box12.trans.lookDirection(D3DXVECTOR3(-1, 0, 0));
	box12.trans.setWorldPosition(4.7f, 4.5f, -2.25f);
	box12.box.setBound(&D3DXVECTOR3(4.65f, 4.5f, -2.25f), &D3DXVECTOR3(0.15f, 1.45f, 1.6f));
	_collision.push_back(box12);

	//문 왼쪽 벽
	mapBoundBox box13;
	box13.isPortalAble = true;
	box13.dir = BOUNDBOXDIR_BACK;
	box13.trans.lookDirection(D3DXVECTOR3(-1, 0, 0));
	box13.trans.setWorldPosition(4.7f, 3.1f, 0.9f);
	box13.box.setBound(&D3DXVECTOR3(4.65f, 3.1f, 0.9f), &D3DXVECTOR3(0.15f, 2.9f, 1.6f));
	_collision.push_back(box13);

	//문 옆 노란벽
	mapBoundBox box14;
	box14.isPortalAble = false;
	box14.box.setBound(&D3DXVECTOR3(6.45f, 3.1f, 5.7f), &D3DXVECTOR3(1.6f, 2.9f, 3.2f));
	_collision.push_back(box14);

	//문 왼쪽 끝벽
	mapBoundBox box15;
	box15.isPortalAble = false;
	box15.box.setBound(&D3DXVECTOR3(4.65f, 3.1f, 12.1f), &D3DXVECTOR3(0.15f, 2.9f, 3.2f));
	_collision.push_back(box15);

	//문 오른쪽
	mapBoundBox box16;
	box16.isPortalAble = true;
	box16.dir = BOUNDBOXDIR_RIGHT;
	box16.trans.lookDirection(D3DXVECTOR3(0, 0, -1));
	box16.trans.setWorldPosition(11.2f, 3.1f, -3.95f);
	box16.box.setBound(&D3DXVECTOR3(11.2f, 3.1f, -4), &D3DXVECTOR3(6.4f, 2.9f, 0.15f));
	_collision.push_back(box16);

	//건너편 밑
	mapBoundBox box17;
	box17.isPortalAble = true;
	box17.dir = BOUNDBOXDIR_UP;
	box17.trans.lookDirection(D3DXVECTOR3(0, -1, 0));
	box17.trans.setWorldPosition(16, 0.05f, 5.6f);
	box17.box.setBound(&D3DXVECTOR3(16, 0, 5.6f), &D3DXVECTOR3(1.6f, 0.15f, 9.7f));
	_collision.push_back(box17);

	//건너편 벽
	mapBoundBox box18;
	box18.isPortalAble = true;
	box18.dir = BOUNDBOXDIR_FORWARD;
	box18.trans.lookDirection(D3DXVECTOR3(1, 0, 0));
	box18.trans.setWorldPosition(17.73f, 3.1f, 4.1f);
	box18.box.setBound(&D3DXVECTOR3(17.73f, 3.1f, 4.1f), &D3DXVECTOR3(0.15f, 2.9f, 7.9f));
	_collision.push_back(box18);

	//출구문 오른쪽
	mapBoundBox box19;
	box19.isPortalAble = false;
	box19.box.setBound(&D3DXVECTOR3(11.2f, 3.1f, 15.4f), &D3DXVECTOR3(6.4f, 2.9f, 0.15f));
	_collision.push_back(box19);
	
	//출구문 위
	mapBoundBox box20;
	box20.isPortalAble = true;
	box20.dir = BOUNDBOXDIR_FORWARD;
	box20.trans.lookDirection(D3DXVECTOR3(1, 0, 0));
	box20.trans.setWorldPosition(17.73f, 3.1f, 4.1f);
	box20.box.setBound(&D3DXVECTOR3(17.73f, 4.5f, 13.65f), &D3DXVECTOR3(0.15f, 1.45f, 1.6f));
	_collision.push_back(box20);
	
	
	//함정 문앞쪽
	mapBoundBox box21;
	box21.isPortalAble = false;
	box21.box.setBound(&D3DXVECTOR3(7.9f, -3.2f, 5.6f), &D3DXVECTOR3(0.15f, 3.2f, 9.7f));
	_collision.push_back(box21);

	//함정 출구 문앞쪽
	mapBoundBox box22;
	box22.isPortalAble = false;
	box22.box.setBound(&D3DXVECTOR3(14.5f, -3.2f, 5.6f), &D3DXVECTOR3(0.15f, 3.2f, 9.7f));
	_collision.push_back(box22);

	//함정 왼쪽
	mapBoundBox box23;
	box23.isPortalAble = false;
	box23.box.setBound(&D3DXVECTOR3(11.1f, -3.2f, -4), &D3DXVECTOR3(3.2f, 3.2f, 0.15f));
	_collision.push_back(box23);

	//함정 오른쪽
	mapBoundBox box24;
	box24.isPortalAble = false;
	box24.box.setBound(&D3DXVECTOR3(11.1f, -3.2f, 15.35f), &D3DXVECTOR3(3.2f, 3.2f, 0.15f));
	_collision.push_back(box24);

	//함정 밑부분
	mapBoundBox box25;
	box25.isPortalAble = false;
	box25.box.setBound(&D3DXVECTOR3(11.2f, -6.3f, 5.5f), &D3DXVECTOR3(3.2f, 0.15f, 9.7f));
	_collision.push_back(box25);

	//출구 오른쪽
	mapBoundBox box26;
	box26.isPortalAble = false;
	box26.box.setBound(&D3DXVECTOR3(22.5f, 1.6f, 11.9f), &D3DXVECTOR3(4.1f, 1.6f, 0.2f));
	_collision.push_back(box26);

	//출구 위
	mapBoundBox box27;
	box27.isPortalAble = false;
	box27.box.setBound(&D3DXVECTOR3(20, 3.9f, 13.5f), &D3DXVECTOR3(1.8f, 0.8f, 1.5f));
	_collision.push_back(box27);

	//출구 아래
	mapBoundBox box28;
	box28.isPortalAble = false;
	box28.box.setBound(&D3DXVECTOR3(21, 0, 13.5f), &D3DXVECTOR3(3.1f, 0.15f, 1.6f));
	_collision.push_back(box28);

	//출구 왼쪽
	mapBoundBox box29;
	box29.isPortalAble = false;
	box29.box.setBound(&D3DXVECTOR3(22.5f, 1.6f, 15.2f), &D3DXVECTOR3(4.1f, 1.6f, 0.2f));
	_collision.push_back(box29);

	mapBoundBox box30;
	box30.isPortalAble = false;
	box30.box.setBound(&D3DXVECTOR3(6.4f, 0, 8.8f), &D3DXVECTOR3(1.6f, 0.15f, 6.4f));
	_collision.push_back(box30);
}