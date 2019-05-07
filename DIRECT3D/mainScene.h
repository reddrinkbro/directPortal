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
	LPDIRECT3DTEXTURE9 _background;			//���ȭ��
	LPDIRECT3DTEXTURE9 _startBefore;		//���۹�ư ��������
	LPDIRECT3DTEXTURE9 _startAfter;			//���۹�ư ��������
	LPDIRECT3DTEXTURE9 _continueBefore;		//�ɼǹ�ư ��������
	LPDIRECT3DTEXTURE9 _continueAfter;		//�ɼǹ�ư ��������
	LPDIRECT3DTEXTURE9 _exitBefore;			//�������ư ��������
	LPDIRECT3DTEXTURE9 _exitAfter;			//�������ư ��������
	LPDIRECT3DTEXTURE9 _changeScene;		//ȭ����ȯ

	RECT			   _pStart;				//���۹�ư ��ġ
	RECT			   _pContinue;			//����ϱ��ư ��ġ
	RECT			   _pExit;				//�������ư ��ġ

	MAINSELECT		   _select;				//������ �޴�
	bool			   _isChange;			//ȭ���� �ٲ������
	int				   _changeCount;		//���� �ð��ڿ� ȭ�� ��ȯ�� ���� ����
	char			   _nextSceneName[10];	//���� ȭ�� ����
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);
	mainScene() {};
	~mainScene() {};
};

