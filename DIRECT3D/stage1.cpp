#include "stdafx.h"
#include "stage1.h"


HRESULT stage1::init(void)
{
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.025f, 0.025f, 0.025f);

	_object = new baseObject;
	_object->setMesh(RM_XMESH->addResource("Resources/map/stage1.X", matScale));
	_object->setActive(true);

	_light = new lightDirection;
	_light->_color = D3DXCOLOR(1, 1, 1, 1);
	_light->_intensity = 1.2f;
	_light->_transform.setRotateWorld(0, D3DXToRadian(270), 0);
	this->setBoundBox();
	return S_OK;
}

void stage1::release(void)
{
	SAFE_DELETE(_light);
}

void stage1::render(void)
{
	xMeshStatic::setTechniqueName("Toon");
	xMeshStatic::setBaseLight(_light);
	_object->render();
}

void stage1::setBoundBox(void)
{
	//입구 엘베 뒤
	mapBoundBox box0;
	box0.isPortalAble = false;
	box0.box.setBound(&D3DXVECTOR3(0.25f, 1.5f, 21.25f), &D3DXVECTOR3(1.75f, 1.75f, 0.15f));
	_collision.push_back(box0);
	//입구 엘베 오른쪽
	mapBoundBox box1;
	box1.isPortalAble = false;
	box1.box.setBound(&D3DXVECTOR3(2.375f, 1.5f, 16.625f), &D3DXVECTOR3(0.75f, 1.75f, 4.25f));
	_collision.push_back(box1);
	//입구 엘베 왼쪽
	mapBoundBox box2;
	box2.isPortalAble = false;
	box2.box.setBound(&D3DXVECTOR3(-2.375f, 1.5f, 16.312f), &D3DXVECTOR3(0.75f, 1.75f, 4.25f));
	_collision.push_back(box2);

	//입구 오른쪽
	mapBoundBox box3;
	box3.isPortalAble = false;
	box3.box.setBound(&D3DXVECTOR3(2.375f, 1.5f, 3.875f), &D3DXVECTOR3(0.75f, 1.75f, 5.25f));
	_collision.push_back(box3);

	//입구 왼쪽
	mapBoundBox box4;
	box4.isPortalAble = false;
	box4.box.setBound(&D3DXVECTOR3(-2.375f, 1.5f, 3.875f), &D3DXVECTOR3(0.75f, 1.75f, 5.25f));
	_collision.push_back(box4);

	//입구 아래
	mapBoundBox box5;
	box5.isPortalAble = false;
	box5.box.setBound(&D3DXVECTOR3(0, 0, 8.375f), &D3DXVECTOR3(3.125f, 0.15f, 9.8f));
	_collision.push_back(box5);

	//입구 위
	mapBoundBox box6;
	box6.isPortalAble = false;
	box6.box.setBound(&D3DXVECTOR3(0, 3.5f, 7.5f), &D3DXVECTOR3(3.125f, 0.437f, 8.125f));
	_collision.push_back(box6);
	
	//문 밖 오른쪽
	mapBoundBox box7;
	box7.isPortalAble = true;
	box7.dir = BOUNDBOXDIR_FORWARD;
	box7.trans.lookDirection(D3DXVECTOR3(1, 0, 0));
	box7.trans.setWorldPosition(1.7f, 1.7, -9.375f);
	box7.box.setBound(&D3DXVECTOR3(1.75f, 1.7, -9.375f), &D3DXVECTOR3(0.15f, 4.7f, 8.125f));
	_collision.push_back(box7);

	//문 밖 왼쪽
	mapBoundBox box8;
	box8.isPortalAble = true;
	box8.dir = BOUNDBOXDIR_BACK;
	box8.trans.setWorldPosition(-4.937f, 1.7, -7.875f);
	box8.trans.lookDirection(D3DXVECTOR3(-1, 0, 0));
	box8.box.setBound(&D3DXVECTOR3(-4.937f, 1.7, -7.875f), &D3DXVECTOR3(0.15f, 4.7, 6.375f));
	_collision.push_back(box8);
	
	//문 옆 벽
	mapBoundBox box9;
	box9.isPortalAble = true;
	box9.dir = BOUNDBOXDIR_LEFT;
	box9.trans.setWorldPosition(-3.375f, 3.0f, -1.76f);
	box9.trans.lookDirection(D3DXVECTOR3(0, 0, 1));
	box9.box.setBound(&D3DXVECTOR3(-3.375f, 3.0f, -1.687f), &D3DXVECTOR3(1.625f, 3.125f, 0.15f));
	_collision.push_back(box9);
	
	//문 밖 아래
	mapBoundBox box10;
	box10.isPortalAble = true;
	box10.dir = BOUNDBOXDIR_UP;
	box10.trans.lookDirection(D3DXVECTOR3(0, -1, 0));
	box10.box.setBound(&D3DXVECTOR3(-1.625f, 0, -6.375f), &D3DXVECTOR3(3.125f, 0.15f, 4.75f));
	box10.trans.setWorldPosition(-1.625f, 0.05f, -6.375f);
	_collision.push_back(box10);

	//문 밖 위
	mapBoundBox box11;
	box11.isPortalAble = true;
	box11.dir = BOUNDBOXDIR_DOWN;
	box11.box.setBound(&D3DXVECTOR3(-6.5f, 6.462f, -15.875f), &D3DXVECTOR3(8.125f, 0.15f, 14.25f));
	box11.trans.setWorldPosition(-6.5f, 6.362f, -15.875f);
	box11.trans.lookDirection(D3DXVECTOR3(0, 1, 0));
	_collision.push_back(box11);

	//문 앞 함정 앞쪽
	mapBoundBox box12;
	box12.isPortalAble = true;
	box12.dir = BOUNDBOXDIR_LEFT;
	box12.trans.setWorldPosition(-1.587f, -1.562f, -11.375f);
	box12.box.setBound(&D3DXVECTOR3(-1.587f, -1.562f, -11.375f), &D3DXVECTOR3(3.175f, 1.625f, 0.15f));
	box12.trans.lookDirection(D3DXVECTOR3(0, 0, 1));
	_collision.push_back(box12);

	//문 앞 함정 뒤쪽
	mapBoundBox box13;
	box13.isPortalAble = true;
	box13.dir = BOUNDBOXDIR_RIGHT;
	box13.trans.setWorldPosition(-1.587f, -1.562f, -14.125f);
	box13.trans.lookDirection(D3DXVECTOR3(0, 0, -1));
	box13.box.setBound(&D3DXVECTOR3(-1.587f, -1.562f, -14.125f), &D3DXVECTOR3(3.175f, 1.625f, 0.15f));
	_collision.push_back(box13);

	//문 앞 함정 밑
	mapBoundBox box14;
	box14.isPortalAble = true;
	box14.dir = BOUNDBOXDIR_UP;
	box14.box.setBound(&D3DXVECTOR3(-1.587f, -2.925f, -12.75f), &D3DXVECTOR3(3.175f, 0.15f, 1.25f));
	box14.trans.setWorldPosition(-1.587f, -2.875f, -12.75f);
	box14.trans.lookDirection(D3DXVECTOR3(0, -1, 0));
	_collision.push_back(box14);

	//문 건너편 밑
	mapBoundBox box15;
	box15.isPortalAble = true;
	box15.dir = BOUNDBOXDIR_UP;
	box15.box.setBound(&D3DXVECTOR3(-7.837f, 0, -19.2f), &D3DXVECTOR3(9.425f, 0.15f, 4.75f));
	box15.trans.setWorldPosition(-7.837f, 0.05f, -19.2f);
	box15.trans.lookDirection(D3DXVECTOR3(0, -1, 0));
	_collision.push_back(box15);

	//문 건너편 맞은 벽
	mapBoundBox box16;
	box16.isPortalAble = true;
	box16.dir = BOUNDBOXDIR_RIGHT;
	box16.trans.setWorldPosition(-4.8f, 3.2f, -17.3f);
	box16.box.setBound(&D3DXVECTOR3(-4.8f, 3.2f, -17.3f), &D3DXVECTOR3(6.375f, 3.0f, 0.15f));
	box16.trans.lookDirection(D3DXVECTOR3(0, 0, -1));
	_collision.push_back(box16);

	//문 건너편 안쪽 벽
	mapBoundBox box17;
	box17.isPortalAble = true;
	box17.dir = BOUNDBOXDIR_LEFT;
	box17.trans.setWorldPosition(-9.437f, 3.2f, -14.2f);
	box17.box.setBound(&D3DXVECTOR3(-9.437f, 3.2f, -14.15f), &D3DXVECTOR3(4.625f, 3.0f, 0.15f));
	box17.trans.lookDirection(D3DXVECTOR3(0, 0, 1));
	_collision.push_back(box17);

	//ㄱ자 짧은 벽
	mapBoundBox box18;
	box18.isPortalAble = true;
	box18.dir = BOUNDBOXDIR_BACK;
	box18.trans.lookDirection(D3DXVECTOR3(1, 0, 0));
	box18.trans.setWorldPosition(-11.4f, 3.0f, -19);
	box18.box.setBound(&D3DXVECTOR3(-11.037f, 3.0f, -19), &D3DXVECTOR3(0.15f, 3.0f, 1.5f));
	_collision.push_back(box18);

	//ㄱ자 긴 벽
	mapBoundBox box19;
	box19.isPortalAble = true;
	box19.dir = BOUNDBOXDIR_FORWARD;
	box19.trans.lookDirection(D3DXVECTOR3(-1, 0, 0));
	box19.trans.setWorldPosition(-13.85f, 3.0f, -22);
	box19.box.setBound(&D3DXVECTOR3(-14.25f, 3.0f, -22), &D3DXVECTOR3(0.15f, 3.0f, 8));
	_collision.push_back(box19);

	//ㄱ자 왼편 벽
	mapBoundBox box20;
	box20.isPortalAble = true;
	box20.dir = BOUNDBOXDIR_LEFT;
	box20.trans.setWorldPosition(-4.775f, 1.5f, -20.642f);
	box20.box.setBound(&D3DXVECTOR3(-4.775f, 1.5f, -20.562f), &D3DXVECTOR3(6.375f, 1.25f, 0.15f));
	box20.trans.lookDirection(D3DXVECTOR3(0, 0, 1));
	_collision.push_back(box20);
	
	//출구 앞 오른쪽 벽
	mapBoundBox box21;
	box21.isPortalAble = true;
	box21.dir = BOUNDBOXDIR_BACK;
	box21.trans.lookDirection(D3DXVECTOR3(1, 0, 0));
	box21.trans.setWorldPosition(1.1f, 3.0f, -25.375f);
	box21.box.setBound(&D3DXVECTOR3(1.437f, 3.0f, -25.375f), &D3DXVECTOR3(0.15f, 3.0f, 4.75f));
	_collision.push_back(box21);

	//출구 앞 왼쪽 벽
	mapBoundBox box22;
	box22.isPortalAble = true;
	box22.dir = BOUNDBOXDIR_RIGHT;
	box22.trans.setWorldPosition(-7.875f, 3.2f, -30.105f);
	box22.box.setBound(&D3DXVECTOR3(-7.875f, 3.2f, -30.175f), &D3DXVECTOR3(6.25f, 3.0f, 0.15f));
	box22.trans.lookDirection(D3DXVECTOR3(0, 0, -1));
	_collision.push_back(box22);

	//출구 앞 아래 
	mapBoundBox box23;
	box23.isPortalAble = true;
	box23.dir = BOUNDBOXDIR_UP;
	box23.trans.lookDirection(D3DXVECTOR3(0, -1, 0));
	box23.box.setBound(&D3DXVECTOR3(-6.25f, 0.075f, -28.625f), &D3DXVECTOR3(7.875f, 0.15f, 1.5f));
	box23.trans.setWorldPosition(-1.3f, 0, -5.1f);
	_collision.push_back(box23);

	//함정 앞쪽 벽
	mapBoundBox box24;
	box24.isPortalAble = false;
	box24.box.setBound(&D3DXVECTOR3(-6.25, -3.125f, -23.75), &D3DXVECTOR3(7.875f, 3.125f, 0.15f));
	_collision.push_back(box24);

	//함정 뒤쪽 벽
	mapBoundBox box25;
	box25.isPortalAble = false;
	box25.box.setBound(&D3DXVECTOR3(-6.25, -3.125f, -27.25f), &D3DXVECTOR3(7.875f, 3.125f, 0.15f));
	_collision.push_back(box25);

	//함정 밑
	mapBoundBox box26;
	box26.dir = BOUNDBOXDIR_UP;
	box26.isPortalAble = true;
	box26.trans.lookDirection(D3DXVECTOR3(0, -1, 0));
	box26.box.setBound(&D3DXVECTOR3(-6.25, -6.375f, -25.5f), &D3DXVECTOR3(7.875f, 0.15f, 1.625f));
	box26.trans.setWorldPosition(-6.25, -6.375f, -25.5f);
	_collision.push_back(box26);
	
	//함정 오른쪽 벽
	mapBoundBox box27;
	box27.isPortalAble = false;
	box27.box.setBound(&D3DXVECTOR3(1.375f, -3.125f, -25.5f), &D3DXVECTOR3(0.15f, 3.125f, 1.625f));
	_collision.push_back(box27);

	//함정 왼쪽 벽
	mapBoundBox box28;
	box28.isPortalAble = false;
	box28.box.setBound(&D3DXVECTOR3(-14.25f, -3.125f, -25.5f), &D3DXVECTOR3(0.15f, 3.125f, 1.625f));
	_collision.push_back(box28);

	//출구 아래
	mapBoundBox box29;
	box29.isPortalAble = false;
	box29.box.setBound(&D3DXVECTOR3(0, 0, -34.875f), &D3DXVECTOR3(1.625f, 0.15f, 4.625f));
	_collision.push_back(box29);

	//출구 위
	mapBoundBox box30;
	box30.isPortalAble = false;
	box30.box.setBound(&D3DXVECTOR3(0, 3.5f, -34.875f), &D3DXVECTOR3(1.625f, 0.437f, 4.625f));
	_collision.push_back(box30);

	//출구 왼쪽
	mapBoundBox box31;
	box31.isPortalAble = false;
	box31.box.setBound(&D3DXVECTOR3(1.625f, 1.625f, -36.125f), &D3DXVECTOR3(0.15f, 1.625f, 5.75f));
	_collision.push_back(box31);

	//출구 오른쪽
	mapBoundBox box32;
	box32.isPortalAble = false;
	box32.box.setBound(&D3DXVECTOR3(-1.75f, 1.625f, -36.125f), &D3DXVECTOR3(0.15f, 1.625f, 5.75f));
	_collision.push_back(box32);

	//입구문 위쪽
	mapBoundBox box33;
	box33.isPortalAble = true;
	box33.dir = BOUNDBOXDIR_LEFT;
	box33.trans.setWorldPosition(-0.05f, 4.6f, -1.76f);
	box33.trans.lookDirection(D3DXVECTOR3(0, 0, 1));
	box33.box.setBound(&D3DXVECTOR3(-0.05f, 4.6f, -1.687f), &D3DXVECTOR3(1.625f, 1.5f, 0.15f));
	_collision.push_back(box33);
	
	//출구문 위쪽
	mapBoundBox box34;
	box34.isPortalAble = true;
	box34.dir = BOUNDBOXDIR_RIGHT;
	box34.trans.setWorldPosition(-0.05f, 4.6f, -30.105f);
	box34.trans.lookDirection(D3DXVECTOR3(0, 0, -1));
	box34.box.setBound(&D3DXVECTOR3(-0.2f, 4.6f, -30.175f), &D3DXVECTOR3(1.5f, 1.5f, 0.15f));
	_collision.push_back(box34);
}