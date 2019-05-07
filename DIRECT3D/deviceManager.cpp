#include "stdafx.h"
#include "deviceManager.h"

// ����̽� ����
HRESULT deviceManager::CreateDevice()
{
	/*
	���̷�Ʈ3D �ʱ�ȭ
	1. Device�� ������ IDirect3D9 ��ü�� ��´�
	2. �ϵ���� ������ ������ �ͼ� �ڽ��� ���� ���μ��� Ÿ���� ���Ѵ�
	3. D3DPRESENT_PARAMETERS ����ü ������ ����
	4. Device ����
	*/

	// 1. Device�� ������ IDirect3D9 ��ü�� ��´�
	_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	// LPDIRECT3D9 ��ü��⸦ �����ϸ� NULL�� �����ȴ�
	if (_d3d9 == NULL)
	{
		MessageBox(_hWnd, "Direct3DCreate9() - FAILED", "FAILED", MB_OK);
		return E_FAIL;
	}

	// 2. �ϵ���� ������ ������ �ͼ� �ڽ��� ���� ���μ��� Ÿ���� ���Ѵ�
	D3DCAPS9 caps;
	// Direct3D9 ��ü�� ���� ���� ī���� �ϵ���� ������ ������ �´�
	_d3d9->GetDeviceCaps(
		D3DADAPTER_DEFAULT,	// �� ���÷��� �׷���ī�� �׳� D3DADAPTER_DEFAULT ����ϸ� ��
		D3DDEVTYPE_HAL,		// ����̽�Ÿ�� ���� �׳� D3DDEVTYPE_HAL
		&caps				// ����̽� ������ �޾ƿ� Direct3D9 ������
	);

	// ������� ó������� ������ �÷��� ��
	DWORD vertexProcessing = 0;
	// ���� ó���� ���� ���� ó���� �ϵ����� �����ϳ�?
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	// 3. D3DPRESENT_PARAMETERS ����ü ������ ����
	// ���� �̷��� ����̽��� ����ڴٶ�� ����
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				// �������ۿ� ���ٽǹ����� ũ�� ����24��Ʈ, ���ٽ�8��Ʈ
	d3dpp.BackBufferCount = 1;									// ����۴� �Ѱ�
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;					// ����� �������� ����8��Ʈ,����8��Ʈ,�׸�8��Ʈ,���8��Ʈ
	d3dpp.BackBufferWidth = WINSIZEX;							// ����� �ȼ� ����ũ��
	d3dpp.BackBufferHeight = WINSIZEY;							// ����� �ȼ� ����ũ��
	d3dpp.EnableAutoDepthStencil = true;						// �ڵ� ���̹��� ��뿩�� (�׳�true)
	d3dpp.Flags = 0;											// �ϴ� 0
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // ȭ�� �ֻ��� (�׳� D3DPRESENT_RATE_DEFAULT ����� �ֻ����� ����)
	d3dpp.hDeviceWindow = _hWnd;								// ����̽��� ��µ� �������ڵ�
	d3dpp.MultiSampleQuality = 0;								// ��Ƽ ���ø� ����Ƽ
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;				// ��Ƽ ���ø� Ÿ�� 
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;		// ȭ�� ���� ����
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ȭ�� ���� ü�� ���
	d3dpp.Windowed = true;

	// 4. Device ����
	// Direct3d9 ��ü�� ���ؼ� ����̽��� �����Ѵ�

	HRESULT result = _d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,		// �� ���÷��� �׷���ī�� �׳� ����Ʈ
		D3DDEVTYPE_HAL,			// ����̽� Ÿ�� ���� �׳� "��"
		_hWnd,					// ����̽� ����� ������ �ڵ�
		vertexProcessing,		// ���� ó����̿� ���� �÷���
		&d3dpp,					// �տ��� ������ d3dpp ����ü ������
		&_device);				// ���� ����̽� ������

	if (FAILED(result))
	{
		// ����̽� �������н� d3d9 ��ü ������ �Ѵ�
		SAFE_RELEASE(_d3d9);

		MessageBox(_hWnd, "CreateDevice() - FAILED", "FAILED", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

// ����̽� ����
void deviceManager::ReleaseDevice()
{
	//SAFE_RELEASE(_d3d9);
	//SAFE_RELEASE(_device);
}
