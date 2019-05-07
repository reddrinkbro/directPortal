#include "stdafx.h"
#include "fontManager.h"


HRESULT fontManager::init(void)
{
	//  ����Ʈ�� ����� ��Ʈ ��ü �����
	D3DXCreateFont(
		device,							//  ����̽�		
		20,								//  ��Ʈ�� ����(������ ��Ʈũ��)
		0,								//  ��Ʈ�� ��(0�϶� ���̿� ����ؼ� ó���ȴ�)
		FW_BOLD,						//  ��Ʈ�β� ���� (���彺Ÿ��)
		1,								//  �Ӹ��� �׳� 1
		FALSE,							//  ���ظ� ����(������ �ִ� ��Ʈ)
		DEFAULT_CHARSET,				//  ����Ʈ ĳ���ͼ�
		OUT_DEFAULT_PRECIS,				//  ����Ʈ �ܰ�ó�����
		DEFAULT_QUALITY,				//  ����Ʈ ����Ƽ
		DEFAULT_PITCH | FF_DONTCARE,	//  ����Ʈ ��ġ�� �йи���
		"����ü",						//  ���� ��Ʈ�̸� �׳� �ѱ۷� ��Ʈ�̸� ���� ��
		&_font);						//  ������ ��Ʈ��ü

	return S_OK;
}

void fontManager::release(void)
{
	// ����Ʈ�� ��Ʈ ����
	SAFE_RELEASE(_font);
	// �ʿ� ����ִ� ��Ʈ���� ���� ����
	mapFontIter iter;
	for (iter = _mapFont.begin(); iter != _mapFont.end(); ++iter)
	{
		SAFE_RELEASE(iter->second);
	}
	_mapFont.clear();
}

void fontManager::addFont(string strKey, string fontName, int fontSize)
{
	//  �̹� ��Ʈ ������ ������ �߰���Ű�� ���� �Լ��� �׳� �������´�
	mapFontIter find = _mapFont.find(strKey);
	if (find != _mapFont.end()) return;

	//  ���ο���Ʈ ��ü �����
	LPD3DXFONT newFont;
	D3DXCreateFont(
		device,							//  ����̽�		
		fontSize,						//  ��Ʈ�� ����(������ ��Ʈũ��)
		0,								//  ��Ʈ�� ��(0�϶� ���̿� ����ؼ� ó���ȴ�)
		FW_BOLD,						//  ��Ʈ�β� ���� (���彺Ÿ��)
		1,								//  �Ӹ��� �׳� 1
		FALSE,							//  ���ظ� ����(������ �ִ� ��Ʈ)
		DEFAULT_CHARSET,				//  ����Ʈ ĳ���ͼ�
		OUT_DEFAULT_PRECIS,				//  ����Ʈ �ܰ�ó�����
		DEFAULT_QUALITY,				//  ����Ʈ ����Ƽ
		DEFAULT_PITCH | FF_DONTCARE,	//  ����Ʈ ��ġ�� �йи���
		fontName.c_str(),				//  ���� ��Ʈ�̸� �׳� �ѱ۷� ��Ʈ�̸� ���� ��
		&newFont);						//  ������ ��Ʈ��ü

										//  ���� ���� ��Ʈ�� �ʿ� ��´�
	_mapFont.insert(make_pair(strKey, newFont));
}

void fontManager::fontOut(string str, int x, int y, DWORD color)
{
	//  ��¿���(������ ��ǥ��)
	RECT rcTestArea = { x, y, 0, 0 };
	_font->DrawText(NULL, str.c_str(), -1, &rcTestArea, DT_LEFT | DT_NOCLIP, color);
}

void fontManager::fontOut(string strKey, string str, int x, int y, DWORD color)
{
	LPD3DXFONT font = NULL;
	//  ��Ʈ������ Ű���� ��ã�Ҵٸ� ����Ʈ�� ��Ʈ�� ����� �ϰ� ã�Ҵٸ� ã�� ��Ʈ�� ��½�Ű��
	mapFontIter find = _mapFont.find(strKey);
	if (find == _mapFont.end()) font = _font;
	else font = find->second;

	//  ��¿���(������ ��ǥ��)
	RECT rcTestArea = { x, y, 0, 0 };
	font->DrawText(NULL, str.c_str(), -1, &rcTestArea, DT_LEFT | DT_NOCLIP, color);
}

void fontManager::fontOutShadow(string str, int x, int y, DWORD color, DWORD shadowColor)
{
	//  ������ ���� �׸���
	fontOut(str, x + 1, y + 1, shadowColor);
	//  ���� �ؽ�Ʈ�� �׸���
	fontOut(str, x, y, color);
}

void fontManager::fontOutShadow(string strKey, string str, int x, int y, DWORD color, DWORD shadowColor)
{
	//  ������ ���� �׸���
	fontOut(strKey, str, x + 1, y + 1, shadowColor);
	//  ���� �ؽ�Ʈ�� �׸���
	fontOut(strKey, str, x, y, color);
}

void fontManager::fontOutLine(string str, int x, int y, DWORD color, DWORD outLineColor)
{
	//  ������ ���� �׸���
	fontOut(str, x + 1, y, outLineColor);
	fontOut(str, x, y + 1, outLineColor);
	fontOut(str, x - 1, y, outLineColor);
	fontOut(str, x, y - 1, outLineColor);
	//  ���� �ؽ����� �׸���
	fontOut(str, x, y, color);
}

void fontManager::fontOutLine(string strKey, string str, int x, int y, DWORD color, DWORD outLineColor)
{
	// ������ ���� �׸���
	fontOut(strKey, str, x + 1, y, outLineColor);
	fontOut(strKey, str, x, y + 1, outLineColor);
	fontOut(strKey, str, x - 1, y, outLineColor);
	fontOut(strKey, str, x, y - 1, outLineColor);
	// ���� �ؽ����� �׸���
	fontOut(strKey, str, x, y, color);
}
