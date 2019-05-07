// ====================================================================
// 			## ������� ���ش� ##
// ====================================================================
#include "stdafx.h"
#include "mainGame.h"
#include "Resource.h"
// ====================================================================
// 			## �������� ���ش� ##
// ====================================================================
HWND _hWnd;				// ������ �ڵ�(�ڽ��� ���α׷����� �����ִ� ������ ��ȣ)
HINSTANCE _hInstance;	// ���α׷� �ν��Ͻ� �ڵ� (OS �� �ο��� ���α׷� ��ȣ)
float	_deltaTime;	
bool _isEnding;
// ���ΰ��� Ŭ����
mainGame _mg;

// ====================================================================
// 			## �Լ� ���漱�� ##
// ====================================================================
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void setWindowSize(int x, int y, int width, int height);

// ====================================================================
// 			## ������ �����Լ� ##
// ====================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	// �ν��Ͻ��� ���������� ��Ƶд�
	_hInstance = hInstance;
	_isEnding = false;
	// WNDCLASS : ������â�� ������ �����ϱ� ���� ����ü
	// ������Ŭ���� ����ü ������ �ʱ�ȭ
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;		// �����쿡�� ����ϴ� ������ �޸𸮼���( �׳� 0 �̴�  �Ű澲������ )
	wndClass.cbWndExtra = 0;		// �����쿡�� ����ϴ� ������ �޸𸮼���( �׳� 0 �̴�  �Ű澲������ )
	wndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);  // ������ ������
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					// �������� Ŀ����� ����
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// ����������ܸ�� ����
	wndClass.hInstance = hInstance;									// ���α׷��ν��Ͻ��ڵ� 
	wndClass.lpfnWndProc = (WNDPROC)WndProc;						// ������ ���ν��� �Լ� ������
	wndClass.lpszClassName = WINNAME;								// �޴��̸� ������ NULL
	wndClass.lpszMenuName = NULL;									// ���� �ۼ��ϰ� �ִ� ������ Ŭ������ �̸�
	wndClass.style = CS_HREDRAW | CS_VREDRAW;						// ������ �׸��� ��� ���� (����� ����ɶ� ȭ�鰻�� CS_HREDRAW | CS_VREDRAW)

																	// ������ �ۼ��� ������ Ŭ���� ���
	RegisterClass(&wndClass);

	// ������ â ����
	// ������ ������ �ڵ��� �������� _hWnd�� �޴´�.
	_hWnd = CreateWindow(
		WINNAME,				// ������ Ŭ������ �̸�
		WINNAME,				// ������ Ÿ��Ʋ�� �̸�
		WS_OVERLAPPEDWINDOW,	// ������ ��Ÿ��
		WINSTARTX,				// ������ ȭ�� x��ǥ
		WINSTARTY,				// ������ ȭ�� y��ǥ
		WINSIZEX,				// ������ ȭ�� ����ũ��
		WINSIZEY,				// ������ ȭ�� ����ũ��
		GetDesktopWindow(),		// �θ� ������ �ڵ� (���α׷����� �ֻ��� ������� NULL �Ǵ� GetDesktopWindow())
		(HMENU)NULL,			// �޴��ڵ�
		hInstance,				// �ν��Ͻ�
		NULL					// �츰 �� NULL����ϸ� �� (�Ű����)
	);

	// ������ Ŭ���̾�Ʈ���� ��Ȯ�� �����ϱ� 
	setWindowSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);

	// ȭ�鿡 ������ �����ֱ�
	ShowWindow(_hWnd, nCmdShow);

	// ���ΰ��� Ŭ���� �ʱ�ȭ �����ߴٸ� �� �����Ű��
	if (FAILED(_mg.init()))
	{
		return E_FAIL;
	}

	// MSG : �ü������ �߻��ϴ� �޼��� ������ �����ϱ� ���� ����ü
	MSG message;
	ZeroMemory(&message, sizeof(MSG));

	
	// �޽�ť���� ���� �޽����� ����޽����� �ƴϸ� ��� ����
	while (message.message != WM_QUIT)
	{
		// �޽���ť�� �޽��������� 
		// ������ �������� PM_REMOVE �ɼ��������� ���޽�����
		// �����.
		// ���� Message �� ���ٸ� false �� �����Ѵ�.

		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);  // ���� Ű������ ���ڸ� �����Ͽ� WM_CHAR �޽����� �߻���Ų��.
			DispatchMessage(&message);	 // �޾ƿ� �޽��� ������ ������ ���ν��� �Լ��� �����Ų��.
		}

		// Ÿ�ӸŴ��� ������Ʈ(FPS 60)
		TIMEMANAGER->update(60.0f);
		_deltaTime = TIMEMANAGER->getElapedTime();
		_mg.update();
		_mg.render();
	}

	// ���ΰ��� Ŭ���� ����
	_mg.release();

	// ������ Ŭ���� ������� (���൵ �׸� �����൵ �׸�..)
	UnregisterClass(WINNAME, hInstance);

	// ������ ���α׷� ����	
	return (int)message.wParam;
}

// ====================================================================
// 			## ������ ���ν��� ##
// ====================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return _mg.MainProc(hWnd, iMessage, wParam, lParam);
}

// ====================================================================
// 			## ������ ũ�� ���� ## (Ŭ���̾�Ʈ ������ ����� ��Ȯ�� ����ش�)
// ====================================================================
void setWindowSize(int x, int y, int width, int height)
{
	RECT rc = { 0, 0, width, height };
	// ���� ������ ũ�� ����
	AdjustWindowRect(&rc, WINSTYLE, false);
	// �� ��Ʈ ������ ������ Ŭ���̾�Ʈ ������ ����
	SetWindowPos(_hWnd, NULL, x, y, (rc.right - rc.left), (rc.bottom - rc.top),
		SWP_NOZORDER | SWP_NOMOVE);
}