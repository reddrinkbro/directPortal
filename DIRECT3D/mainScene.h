#pragma once
#include "gameNode.h"

enum MAINSELECT
{
	MAINSELECT_NONE,
	MAINSELECT_START,
	MAINSELECT_OPTION,
	MAINSELECT_EXIT
};
class mainScene : public gameNode
{
private:
	LPDIRECT3DTEXTURE9 _background;			//배경화면
	LPDIRECT3DTEXTURE9 _startBefore;		//시작버튼 누르기전
	LPDIRECT3DTEXTURE9 _startAfter;			//시작버튼 누르기후
	LPDIRECT3DTEXTURE9 _continueBefore;		//옵션버튼 누르기전
	LPDIRECT3DTEXTURE9 _continueAfter;		//옵션버튼 누르기후
	LPDIRECT3DTEXTURE9 _exitBefore;			//나가기버튼 누르기전
	LPDIRECT3DTEXTURE9 _exitAfter;			//나가기버튼 누르기후
	LPDIRECT3DTEXTURE9 _changeScene;		//화면전환

	RECT			   _pStart;				//시작버튼 위치
	RECT			   _pContinue;			//계속하기버튼 위치
	RECT			   _pExit;				//나가기버튼 위치

	MAINSELECT		   _select;				//선택한 메뉴
	bool			   _isChange;			//화면이 바뀌었는지
	int				   _changeCount;		//일정 시간뒤에 화면 전환을 위한 변수
	char			   _nextSceneName[10];	//다음 화면 선택
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);
	mainScene() {};
	~mainScene() {};
};

