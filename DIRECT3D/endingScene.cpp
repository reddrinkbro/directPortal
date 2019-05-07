#include "stdafx.h"
#include "endingScene.h"

HRESULT endingScene::init(void)
{
	CoInitialize(NULL);
	_graph.SetMediaWindow(_hWnd);
	_isEnding = true;
	_Layer = 0;
	_Left = 0.0f;
	_Top = 0.0f;
	_Right = WINSIZEX;
	_Bottom = WINSIZEY;
	_alpha = 50.0f;
	_playTime = TIMEMANAGER->getWorldTime();
	RECT videoRect;
	RECT newRect = { _Left, _Top, _Right, _Bottom };
	videoRect = newRect;

	_graph.SetLayerRect(_Layer, newRect);
	_graph.SetAlphaLayer(_Layer, _alpha);

	if (_graph.GetVideoRect(&videoRect)) {
		_Left = videoRect.left;
		_Right = videoRect.right;
		_Top = videoRect.top;
		_Bottom = videoRect.bottom;
	}

	_graph.SetMediaFile("Resources/ending/ending.avi", 0);
	_graph.SetLayerZOrder(0, 0);
	return S_OK;
}

void endingScene::release(void)
{
	
}

void endingScene::render(void)
{
	if (_graph.PlayGraph())
	{
		if (TIMEMANAGER->getWorldTime() - _playTime >= 181)
		{
			_graph.StopGraph();
			_isEnding = false;
			SCENEMANAGER->changeScene("메인화면");
		}
	}
	if (KEYMANAGER->isOnceKeyDown(VK_ESCAPE))
	{
		_graph.StopGraph();
		_isEnding = false;
		SCENEMANAGER->changeScene("메인화면");
	}
}