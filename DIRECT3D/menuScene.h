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
	LPDIRECT3DTEXTURE9 _backBefore;		//돌아가기 누르기전
	LPDIRECT3DTEXTURE9 _backAfter;		//돌아가기 누르기후
	LPDIRECT3DTEXTURE9 _menuBefore;		//메뉴가기 누르기전
	LPDIRECT3DTEXTURE9 _menuAfter;		//메뉴가기 누르기후

	RECT			   _pBack;				//돌아가기 위치
	RECT			   _pMenu;			//메뉴가기 위치

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

