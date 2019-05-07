#include "stdafx.h"
#include "tutorialMap2.h"

HRESULT tutorialMap2::init(void)
{
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.01f, 0.01f, 0.01f);

	_object = new baseObject;
	_object->setMesh(RM_XMESH->addResource("Resources/map/tutorial2.X", matScale));
	_object->setActive(true);

	_light = new lightDirection;
	_light->_color = D3DXCOLOR(1, 1, 1, 1);
	_light->_intensity = 1.2f;
	_light->_transform.setRotateWorld(0, D3DXToRadian(270), 0);
	this->setBoundBox();
	return S_OK;
}

void tutorialMap2::release(void)
{
	SAFE_DELETE(_light);
}

void tutorialMap2::render(void)
{
	xMeshStatic::setTechniqueName("Toon");
	xMeshStatic::setBaseLight(_light);
	_object->render();
}

void tutorialMap2::setBoundBox(void)
{
	//�Ʒ�
	mapBoundBox box0;
	box0.isPortalAble = true;
	box0.trans.lookDirection(D3DXVECTOR3(0, -1, 0));
	box0.dir = BOUNDBOXDIR_UP;
	box0.trans.setWorldPosition(-1, - 3.2f, 0);
	box0.box.setBound(&D3DXVECTOR3(-1, -3.2f, 0), &D3DXVECTOR3(10, 0.15f, 10));
	_collision.push_back(box0);

	//��
	mapBoundBox box1;
	box1.isPortalAble = true;
	box1.trans.lookDirection(D3DXVECTOR3(0, 1, 0));
	box1.dir = BOUNDBOXDIR_DOWN;
	box1.trans.setWorldPosition(-1, 9.2f, 0);
	box1.box.setBound(&D3DXVECTOR3(-1, 9.2f, 0), &D3DXVECTOR3(10, 0.15f, 10));
	_collision.push_back(box1);

	//����
	mapBoundBox box2;
	box2.isPortalAble = true;
	box2.trans.lookDirection(D3DXVECTOR3(0, 0, -1));
	box2.dir = BOUNDBOXDIR_RIGHT;
	box2.trans.setWorldPosition(-1, 0, -6.5f);
	box2.box.setBound(&D3DXVECTOR3(-1, 0, -6.5f), &D3DXVECTOR3(10, 10, 0.15f));
	_collision.push_back(box2);

	//������
	mapBoundBox box3;
	box3.isPortalAble = true;
	box3.trans.lookDirection(D3DXVECTOR3(0, 0, 1));
	box3.dir = BOUNDBOXDIR_LEFT;
	box3.trans.setWorldPosition(-1, 0, 9.5f);
	box3.box.setBound(&D3DXVECTOR3(-1, 0, 9.5f), &D3DXVECTOR3(10, 10, 0.15f));
	_collision.push_back(box3);

	//���� ����
	mapBoundBox box4;
	box4.isPortalAble = true;
	box4.trans.lookDirection(D3DXVECTOR3(-1, 0, 0));
	box4.dir = BOUNDBOXDIR_BACK;
	box4.trans.setWorldPosition(-10.55f, 0, -3.5f);
	box4.box.setBound(&D3DXVECTOR3(-10.55f, 0, -3.5f), &D3DXVECTOR3(0.15f, 10, 3.2f));
	_collision.push_back(box4);

	//���� ���
	mapBoundBox box5;
	box5.isPortalAble = true;
	box5.trans.lookDirection(D3DXVECTOR3(-1, 0, 0));
	box5.dir = BOUNDBOXDIR_BACK;
	box5.trans.setWorldPosition(-10.55f, 6.3f, 1.5f);
	box5.box.setBound(&D3DXVECTOR3(-10.55f, 6.3f, 1.5f), &D3DXVECTOR3(0.15f, 2.9f, 1.8f));
	_collision.push_back(box5);

	//���� ������
	mapBoundBox box6;
	box6.isPortalAble = true;
	box6.trans.lookDirection(D3DXVECTOR3(-1, 0, 0));
	box6.dir = BOUNDBOXDIR_BACK;
	box6.trans.setWorldPosition(-10.55f, 0, 6.6f);
	box6.box.setBound(&D3DXVECTOR3(-10.55f, 0, 6.6f), &D3DXVECTOR3(0.15f, 10, 3.2f));
	_collision.push_back(box6);

	//���� ����
	mapBoundBox box7;
	box7.isPortalAble = true;
	box7.trans.lookDirection(D3DXVECTOR3(1, 0, 0));
	box7.dir = BOUNDBOXDIR_FORWARD;
	box7.trans.setWorldPosition(8.7f, 0, -3.5f);
	box7.box.setBound(&D3DXVECTOR3(8.7f, 0, -3.5f), &D3DXVECTOR3(0.15f, 10, 3.2f));
	_collision.push_back(box7);

	//���� ���
	mapBoundBox box8;
	box8.isPortalAble = true;
	box8.trans.lookDirection(D3DXVECTOR3(1, 0, 0));
	box8.dir = BOUNDBOXDIR_FORWARD;
	box8.trans.setWorldPosition(8.7f, 6.3f, 1.5f);
	box8.box.setBound(&D3DXVECTOR3(8.7f, 6.3f, 1.5f), &D3DXVECTOR3(0.15f, 2.9f, 1.6f));
	_collision.push_back(box8);

	//���� ������
	mapBoundBox box9;
	box9.isPortalAble = true;
	box9.trans.lookDirection(D3DXVECTOR3(1, 0, 0));
	box9.dir = BOUNDBOXDIR_FORWARD;
	box9.trans.setWorldPosition(8.7f, 0, 6.6f);
	box9.box.setBound(&D3DXVECTOR3(8.7f, 0, 6.6f), &D3DXVECTOR3(0.15f, 10, 3.2f));
	_collision.push_back(box9);

	//�Ա� ����
	mapBoundBox box10;
	box10.isPortalAble = true;
	box10.trans.lookDirection(D3DXVECTOR3(0, -1, 0));
	box10.dir = BOUNDBOXDIR_UP;
	box10.trans.setWorldPosition(-9.8f, 0.2f, 1.4f);
	box10.box.setBound(&D3DXVECTOR3(-9.8f, 0, 1.4f), &D3DXVECTOR3(5.5f, 0.15f, 1.5f));
	_collision.push_back(box10);

	//�Ա� ����
	mapBoundBox box11;
	box11.isPortalAble = true;
	box11.trans.lookDirection(D3DXVECTOR3(0, 0, 1));
	box11.dir = BOUNDBOXDIR_LEFT;
	box11.trans.setWorldPosition(-7.3f, -1.45f, -0.05f);
	box11.box.setBound(&D3DXVECTOR3(-7.3f, -1.45f, -0.05f), &D3DXVECTOR3(3.0f, 1.6f, 0.15f));
	_collision.push_back(box11);

	//�Ա� ������
	mapBoundBox box12;
	box12.isPortalAble = true;
	box12.trans.lookDirection(D3DXVECTOR3(0, 0, -1));
	box12.dir = BOUNDBOXDIR_RIGHT;
	box12.trans.setWorldPosition(-7.3f, -1.45f, 2.9f);
	box12.box.setBound(&D3DXVECTOR3(-7.3f, -1.45f, 2.9f), &D3DXVECTOR3(3.0f, 1.6f, 0.15f));
	_collision.push_back(box12);

	//�Ա� ����
	mapBoundBox box13;
	box13.isPortalAble = true;
	box13.trans.lookDirection(D3DXVECTOR3(-1, 0, 0));
	box13.trans.setWorldPosition(-3.95f, -1.45f, 1.45f);
	box13.dir = BOUNDBOXDIR_BACK;
	box13.box.setBound(&D3DXVECTOR3(-4.1f, -1.45f, 1.45f), &D3DXVECTOR3(0.15f, 1.6f, 1.6f));
	_basicBox.push_back(box13);

	//���� �ڽ� ����
	mapBoundBox box14;
	box14.isPortalAble = true;
	box14.trans.lookDirection(D3DXVECTOR3(-1, 0, 0)); 
	box14.trans.setWorldPosition(2.53f, 1.75f, -4.8f);
	box14.dir = BOUNDBOXDIR_BACK;
	box14.box.setBound(&D3DXVECTOR3(2.38f, 1.75f, -4.8f), &D3DXVECTOR3(0.15f, 1.6f, 1.8f));
	_basicBox.push_back(box14);

	//���� �ڽ� ����
	mapBoundBox box15;
	box15.isPortalAble = true;
	box15.trans.lookDirection(D3DXVECTOR3(1, 0, 0)); 
	box15.trans.setWorldPosition(-4.05f, 1.75f, -4.8f);
	box15.dir = BOUNDBOXDIR_FORWARD;
	box15.box.setBound(&D3DXVECTOR3(-3.9f, 1.75f, -4.8f), &D3DXVECTOR3(0.15f, 1.6f, 1.8f));
	_basicBox.push_back(box15);

	//���� �ڽ� ����
	mapBoundBox box16;
	box16.isPortalAble = true;
	box16.trans.lookDirection(D3DXVECTOR3(0, 0, -1));
	box16.trans.setWorldPosition(-0.7f, 1.65f, -2.92f);
	box16.dir = BOUNDBOXDIR_RIGHT;
	box16.box.setBound(&D3DXVECTOR3(-0.7f, 1.65f, -3.07f), &D3DXVECTOR3(3.2f, 1.6f, 0.15f));
	_collision.push_back(box16);

	//���� �ڽ� ����
	mapBoundBox box17;
	box17.isPortalAble = true;
	box17.trans.lookDirection(D3DXVECTOR3(0, -1, 0));
	box17.trans.setWorldPosition(-0.7f, 3.45f, -4.7f);
	box17.dir = BOUNDBOXDIR_UP;
	box17.box.setBound(&D3DXVECTOR3(-0.7, 3.3f, -4.7f), &D3DXVECTOR3(3.2f, 0.15f, 1.7f));
	_collision.push_back(box17);

	//���� �ڽ� �Ʒ���
	mapBoundBox box18;
	box18.isPortalAble = true;
	box18.trans.lookDirection(D3DXVECTOR3(0, 1, 0));
	box18.trans.setWorldPosition(-0.7f, 0.15f, -4.7f);
	box18.dir = BOUNDBOXDIR_DOWN;
	box18.box.setBound(&D3DXVECTOR3(-0.7, 0.3f, -4.7f), &D3DXVECTOR3(3.2f, 0.15f, 1.7f));
	_collision.push_back(box18);

	//������ �ڽ� ����
	mapBoundBox box19;
	box19.isPortalAble = true;
	box19.trans.lookDirection(D3DXVECTOR3(-1, 0, 0));
	box19.trans.setWorldPosition(0.77f, 1.75f, 7.8f);
	box19.dir = BOUNDBOXDIR_BACK;
	box19.box.setBound(&D3DXVECTOR3(2.38f, 1.75f, 7.8f), &D3DXVECTOR3(0.15f, 1.6f, 1.8f));
	_basicBox.push_back(box19);

	//������ �ڽ� ����
	mapBoundBox box20;
	box20.isPortalAble = true;
	box20.trans.lookDirection(D3DXVECTOR3(1, 0, 0));
	box20.trans.setWorldPosition(-4, 1.75f, 7.8f);
	box20.dir = BOUNDBOXDIR_FORWARD;
	box20.box.setBound(&D3DXVECTOR3(-3.85f, 1.75f, 7.8f), &D3DXVECTOR3(0.15f, 1.6f, 1.8f));
	_basicBox.push_back(box20);

	//������ �ڽ� ����
	mapBoundBox box21;
	box21.isPortalAble = true;
	box21.trans.lookDirection(D3DXVECTOR3(0, 0, 1));
	box21.trans.setWorldPosition(-0.7f, 1.65f, 5.92f);
	box21.dir = BOUNDBOXDIR_LEFT;
	box21.box.setBound(&D3DXVECTOR3(-0.7f, 1.65f, 6.07f), &D3DXVECTOR3(3.2f, 1.6f, 0.15f));
	_collision.push_back(box21);

	//������ �ڽ� ����
	mapBoundBox box22;
	box22.isPortalAble = true;
	box22.trans.lookDirection(D3DXVECTOR3(0, -1, 0));
	box22.trans.setWorldPosition(-0.7, 3.15f, 7.7f);
	box22.dir = BOUNDBOXDIR_UP;
	box22.box.setBound(&D3DXVECTOR3(-0.7, 3.3f, 7.7f), &D3DXVECTOR3(3.2f, 0.15f, 1.7f));
	_collision.push_back(box22);

	//������ �ڽ� �Ʒ���
	mapBoundBox box23;
	box23.isPortalAble = true;
	box23.trans.lookDirection(D3DXVECTOR3(0, 1, 0));
	box23.trans.setWorldPosition(-0.7, 0.15f, 7.7f);
	box23.dir = BOUNDBOXDIR_DOWN;
	box23.box.setBound(&D3DXVECTOR3(-0.7, 0.3f, 7.7f), &D3DXVECTOR3(3.2f, 0.15f, 1.7f));
	_collision.push_back(box23);

	//���� �Ʒ� �ڽ� ����
	mapBoundBox box24;
	box24.isPortalAble = true;
	box24.trans.lookDirection(D3DXVECTOR3(1, 0, 0));
	box24.trans.setWorldPosition(5.48f, -1.45f, 1.45f);
	box24.dir = BOUNDBOXDIR_FORWARD;
	box24.box.setBound(&D3DXVECTOR3(5.63f, -1.65f, 1.45f), &D3DXVECTOR3(0.15f, 1.6f, 1.6f));
	_basicBox.push_back(box24);

	//���� �Ʒ� �ڽ� ������
	mapBoundBox box25;
	box25.isPortalAble = true;
	box25.trans.lookDirection(D3DXVECTOR3(0, 0, -1));
	box25.trans.setWorldPosition(7.43f, -1.45f, 3.07f);
	box25.dir = BOUNDBOXDIR_RIGHT;
	box25.box.setBound(&D3DXVECTOR3(7.43f, -1.65f, 2.92f), &D3DXVECTOR3(1.8f, 1.6f, 0.15f));
	_basicBox.push_back(box25);

	//���� �Ʒ� �ڽ� ����
	mapBoundBox box26;
	box26.isPortalAble = true;
	box26.trans.lookDirection(D3DXVECTOR3(0, 0, 1));
	box26.trans.setWorldPosition(7.43f, -1.45f, -0.2f);
	box26.dir = BOUNDBOXDIR_LEFT;
	box26.box.setBound(&D3DXVECTOR3(7.43f, -1.65f, -0.05f), &D3DXVECTOR3(1.8f, 1.6f, 0.15f));
	_basicBox.push_back(box26);

	//���� �Ʒ� �ڽ� ����
	mapBoundBox box27;
	box27.isPortalAble = true;
	box27.trans.lookDirection(D3DXVECTOR3(0, -1, 0));
	box27.trans.setWorldPosition(7.1f, 0.17f, 1.4f);
	box27.dir = BOUNDBOXDIR_UP;
	box27.box.setBound(&D3DXVECTOR3(7.1f, -0.05f, 1.4f), &D3DXVECTOR3(1.5f, 0.15f, 1.5f));
	_basicBox.push_back(box27);
	
	//���� �� �ڽ� ����
	mapBoundBox box28;
	box28.isPortalAble = true;
	box28.trans.lookDirection(D3DXVECTOR3(1, 0, 0));
	box28.trans.setWorldPosition(5.48f, 4.5f, 1.45f);
	box28.dir = BOUNDBOXDIR_FORWARD;
	box28.box.setBound(&D3DXVECTOR3(5.63f, 4.5f, 1.45f), &D3DXVECTOR3(0.15f, 1.7f, 1.6f));
	_basicBox.push_back(box28);

	//���� �� �ڽ� ������
	mapBoundBox box29;
	box29.isPortalAble = true;
	box29.trans.lookDirection(D3DXVECTOR3(0, 0, -1));
	box29.trans.setWorldPosition(7.43f, 4.5f, 3.07f);
	box29.dir = BOUNDBOXDIR_RIGHT;
	box29.box.setBound(&D3DXVECTOR3(7.43f, 4.5f, 2.92f), &D3DXVECTOR3(1.8f, 1.7f, 0.15f));
	_basicBox.push_back(box29);

	//���� �� �ڽ� ����
	mapBoundBox box30;
	box30.isPortalAble = true;
	box30.trans.lookDirection(D3DXVECTOR3(0, 0, 1));
	box30.trans.setWorldPosition(7.43f, 4.5f, -0.25f);
	box30.dir = BOUNDBOXDIR_LEFT;
	box30.box.setBound(&D3DXVECTOR3(7.43f, 4.5f, -0.1f), &D3DXVECTOR3(1.8f, 1.7f, 0.15f));
	_basicBox.push_back(box30);

	//���� �� �ڽ� ����
	mapBoundBox box31;
	box31.isPortalAble = true;
	box31.trans.lookDirection(D3DXVECTOR3(0, -1, 0));
	box31.trans.setWorldPosition(7.1f, 6.32f, 1.4f);
	box31.dir = BOUNDBOXDIR_UP;
	box31.box.setBound(&D3DXVECTOR3(7.1f, 6.17f, 1.4f), &D3DXVECTOR3(1.5f, 0.15f, 1.5f));
	_basicBox.push_back(box31);

	//���� �� �ڽ� �Ʒ���
	mapBoundBox box32;
	box32.isPortalAble = true;
	box32.trans.lookDirection(D3DXVECTOR3(0, 1, 0));
	box32.trans.setWorldPosition(7.1f, 2.75f, 1.4f);
	box32.dir = BOUNDBOXDIR_DOWN;
	box32.box.setBound(&D3DXVECTOR3(7.1f, 2.9f, 1.4f), &D3DXVECTOR3(1.5f, 0.15f, 1.5f));
	_basicBox.push_back(box32);

	//�ⱸ �ٱ��� ����
	mapBoundBox box33;
	box33.isPortalAble = true;
	box33.trans.lookDirection(D3DXVECTOR3(0, 0, 1));
	box33.trans.setWorldPosition(10.6f, 1.5f, 2.92f);
	box33.dir = BOUNDBOXDIR_LEFT;
	box33.box.setBound(&D3DXVECTOR3(10.6f, 1.5f, 3.1f), &D3DXVECTOR3(1.5f, 1.5f, 0.15f));
	_basicBox.push_back(box33);

	//�ⱸ �ٱ��� ������
	mapBoundBox box34;
	box34.isPortalAble = true;
	box34.trans.lookDirection(D3DXVECTOR3(0, 0, -1));
	box34.trans.setWorldPosition(10.6f, 1.5f, 0);
	box34.dir = BOUNDBOXDIR_RIGHT;
	box34.box.setBound(&D3DXVECTOR3(10.6f, 1.5f, -0.3f), &D3DXVECTOR3(1.5f, 1.5f, 0.15f));
	_basicBox.push_back(box34);

	//�ⱸ �ٱ��� ����
	mapBoundBox box35;
	box35.isPortalAble = true;
	box35.trans.lookDirection(D3DXVECTOR3(0, 1, 0));
	box35.trans.setWorldPosition(10.6f, 3.05f, 1.4f);
	box35.dir = BOUNDBOXDIR_DOWN;
	box35.box.setBound(&D3DXVECTOR3(10.6f, 3.2f, 1.4f), &D3DXVECTOR3(1.5f, 0.15f, 1.5f));
	_basicBox.push_back(box35);

	//�ⱸ �ٱ��� �Ʒ���
	mapBoundBox box36;
	box36.isPortalAble = true;
	box36.trans.lookDirection(D3DXVECTOR3(0, -1, 0));
	box36.trans.setWorldPosition(10.6f, 0.2f, 1.4f);
	box36.dir = BOUNDBOXDIR_UP;
	box36.box.setBound(&D3DXVECTOR3(10.6f, 0.07f, 1.4f), &D3DXVECTOR3(1.5f, 0.15f, 1.5f));
	_basicBox.push_back(box36);

	//�ⱸ ���� ����
	mapBoundBox box37;
	box37.isPortalAble = true;
	box37.trans.lookDirection(D3DXVECTOR3(0, 0, -1));
	box37.trans.setWorldPosition(14.3f, 1.5f, 2.95f);
	box37.dir = BOUNDBOXDIR_LEFT;
	box37.box.setBound(&D3DXVECTOR3(14.3f, 1.5f, 3.1f), &D3DXVECTOR3(1.5f, 1.5f, 0.15f));
	_basicBox.push_back(box37);

	//�ⱸ ���� ������
	mapBoundBox box38;
	box38.isPortalAble = true;
	box38.trans.lookDirection(D3DXVECTOR3(0, 0, 1));
	box38.trans.setWorldPosition(14.3f, 1.5f, -0.15f);
	box38.dir = BOUNDBOXDIR_RIGHT;
	box38.box.setBound(&D3DXVECTOR3(14.3f, 1.5f, -0.3f), &D3DXVECTOR3(1.5f, 1.5f, 0.15f));
	_basicBox.push_back(box38);

	//�ⱸ ���� ����
	mapBoundBox box39;
	box39.isPortalAble = true;
	box39.trans.lookDirection(D3DXVECTOR3(0, 1, 0));
	box39.trans.setWorldPosition(14.3f, 3.05f, 1.4f);
	box38.dir = BOUNDBOXDIR_DOWN;
	box39.box.setBound(&D3DXVECTOR3(14.3f, 3.2f, 1.4f), &D3DXVECTOR3(1.5f, 0.15f, 1.5f));
	_basicBox.push_back(box39);

	//�ⱸ ���� �Ʒ���
	mapBoundBox box40;
	box40.isPortalAble = true;
	box40.trans.lookDirection(D3DXVECTOR3(0, -1, 0));
	box40.trans.setWorldPosition(14.3f, 0.2f, 1.4f);
	box40.dir = BOUNDBOXDIR_UP;
	box40.box.setBound(&D3DXVECTOR3(14.3f, 0.05f, 1.4f), &D3DXVECTOR3(1.5f, 0.15f, 1.5f));
	_basicBox.push_back(box40);

	//�Ա� ���� ����
	mapBoundBox box41;
	box41.isPortalAble = true;
	box41.trans.lookDirection(D3DXVECTOR3(0, 1, 0));
	box41.trans.setWorldPosition(-12, 3.05f, 1.4f);
	box41.dir = BOUNDBOXDIR_DOWN;
	box41.box.setBound(&D3DXVECTOR3(-12, 3.2f, 1.4f), &D3DXVECTOR3(1.5f, 0.15f, 1.5f));
	_basicBox.push_back(box41);

	//�Ա� ���� ������
	mapBoundBox box42;
	box42.isPortalAble = true;
	box42.trans.lookDirection(D3DXVECTOR3(0, 0, 1));
	box42.trans.setWorldPosition(-11.6f, 1.5f, 3.03f);
	box42.dir = BOUNDBOXDIR_LEFT;
	box42.box.setBound(&D3DXVECTOR3(-11.6f, 1.5f, 3.18f), &D3DXVECTOR3(1.5f, 1.5f, 0.15f));
	_basicBox.push_back(box42);

	//�Ա� ���� ����
	mapBoundBox box43;
	box43.isPortalAble = true;
	box43.trans.lookDirection(D3DXVECTOR3(0, 0, -1));
	box43.trans.setWorldPosition(-11.6f, 1.5f, -0.03f);
	box43.dir = BOUNDBOXDIR_RIGHT;
	box43.box.setBound(&D3DXVECTOR3(-11.6f, 1.5f, -0.23f), &D3DXVECTOR3(1.5f, 1.5f, 0.15f));
	_basicBox.push_back(box43);

	//�Ա� ���� �Ʒ���
	mapBoundBox box44;
	box44.isPortalAble = false;
	box44.box.setBound(&D3DXVECTOR3(-14.4f, 0, 1.4f), &D3DXVECTOR3(1.1f, 0.15f, 1.5f));
	_basicBox.push_back(box44);

	//�Ա� ���� ������
	mapBoundBox box45;
	box45.isPortalAble = false;
	box45.box.setBound(&D3DXVECTOR3(-16.4f, 1.5f, 3.18f), &D3DXVECTOR3(3.1f, 1.5f, 0.15f));
	_basicBox.push_back(box45);

	//�Ա� ���� ����
	mapBoundBox box46;
	box46.isPortalAble = false;
	box46.box.setBound(&D3DXVECTOR3(-16.4f, 1.5f, -0.23f), &D3DXVECTOR3(3.1f, 1.5f, 0.15f));
	_basicBox.push_back(box46);

	//�ⱸ ���� �Ʒ���
	mapBoundBox box47;
	box47.isPortalAble = false;
	box47.box.setBound(&D3DXVECTOR3(17, 0, 1.4f), &D3DXVECTOR3(1.1f, 0.15f, 1.5f));
	_basicBox.push_back(box47);

	//�ⱸ ���� ������
	mapBoundBox box48;
	box48.isPortalAble = false;
	box48.box.setBound(&D3DXVECTOR3(17, 1.5f, 3.18f), &D3DXVECTOR3(1.1f, 1.5f, 0.15f));
	_basicBox.push_back(box48);

	//�ⱸ ���� ����
	mapBoundBox box49;
	box49.isPortalAble = false;
	box49.box.setBound(&D3DXVECTOR3(17, 1.5f, -0.23f), &D3DXVECTOR3(1.1f, 1.5f, 0.15f));
	_basicBox.push_back(box49);
	
	//�Ա� ���� ����
	mapBoundBox box50;
	box50.isPortalAble = false;
	box50.box.setBound(&D3DXVECTOR3(-19, 1.5f, 1.4f), &D3DXVECTOR3(0.15f, 1.5f, 1.5f));
	_basicBox.push_back(box50);

	//�ⱸ �� �Ʒ�
	mapBoundBox box51;
	box51.isPortalAble = false;
	box51.box.setBound(&D3DXVECTOR3(12.4f, 0.07f, 1.4f), &D3DXVECTOR3(0.22f, 0.15f, 1.5f));
	_basicBox.push_back(box51);
}
