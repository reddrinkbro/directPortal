#pragma once
#include "gameNode.h"
enum MENUSELECT
{
	MENUSELECT_NONE,
	MENUSELECT_BACK,
	MENUSELECT_MAIN
};

class menuScene
{
private:
	LPDIRECT3DTEXTURE9 _backBefore;		//���ư��� ��������
	LPDIRECT3DTEXTURE9 _backAfter;		//���ư��� ��������
	LPDIRECT3DTEXTURE9 _menuBefore;		//�޴����� ��������
	LPDIRECT3DTEXTURE9 _menuAfter;		//�޴����� ��������

	RECT			   _pBack;				//���ư��� ��ġ
	RECT			   _pMenu;			//�޴����� ��ġ

	MENUSELECT		   _select;
	bool			   _isOn;
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);
public:
	bool getIsOn(void) { return _isOn; }
	void setIsOn(bool isOn) { _isOn = isOn; }
	menuScene() {};
	~menuScene() {};
};

