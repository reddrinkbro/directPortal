#pragma once
#include "singletonBase.h"
// ====================================================================
// 			## deviceManager ## (����̽� �Ŵ���)
// ====================================================================

class deviceManager : public singletonBase <deviceManager>
{
private:
	// ��¥ �߿���, �� ����̽��� ���� �� �����Ѵٰ� �����ϸ��!!!
	// �̳��� �Ŀ��� HDC�ʹ� �񱳺Ұ�
	LPDIRECT3DDEVICE9 _device;		// ����̽�
	LPDIRECT3D9	_d3d9;				// ����̽��� ������ IDirect3D9 ��ü	

public:
	HRESULT CreateDevice(void);		// ����̽� ����
	void ReleaseDevice(void);		// ����̽� ����

									// ����̽� ���
	LPDIRECT3DDEVICE9 getDevice(void) { return _device; }

	deviceManager() {}
	~deviceManager() {}
};

