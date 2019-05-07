#include "stdafx.h"
#include "camera.h"

camera::camera() : _renderTexture(NULL), _renderSurface(NULL),
_deviceDepthAndStencilSurface(NULL)
{
	setFov(D3DXToRadian(60));
	setCamNear(0.1f);
	setCamFar(1000.0f);
}

void camera::update()
{
	// ȭ���� ���� ������� ������Ʈ
	D3DXMatrixPerspectiveFovLH(&_matProjection, _fov,
		static_cast<float>(WINSIZEX) / static_cast<float>(WINSIZEY),
		_camNear, _camFar
	);

	// ����� ī�޶� ���� ��ġ�� ���� ����� D3DXMatrixInverse()
	D3DXMatrixInverse(&_matView, NULL, &_matFinal);

	// �� ���� ��� 
	_matViewProjection = _matView * _matProjection;
	
	// ����̽��� ���� ��, ������� ��ȯ
	device->SetTransform(D3DTS_VIEW, &_matView);
	device->SetTransform(D3DTS_PROJECTION, &_matProjection);
}

void camera::updateFrustum()
{
	_frustum.updateFrustum(&_matViewProjection);
}

void camera::updateMatrix()
{
	//��������
	if (_isOrtho == FALSE)
	{
		//ȭ���� ���� ������� ������Ʈ(FOV���� �ϱ�(��������))
		D3DXMatrixPerspectiveFovLH(&_matProjection, _fov, _aspect, _camNear, _camFar);
	}
	//��������
	else
	{
		//ȭ���� ���� ������� ������Ʈ(��������)
		D3DXMatrixOrthoLH(&_matProjection, _aspect * _orthoSize, _orthoSize, _camNear, _camFar);
	}

	//����� ī�޶� ������ġ�� ���� �����
	D3DXMatrixInverse(&_matView, NULL, &_matFinal);

	//�� * �������
	_matViewProjection = _matView * _matProjection;
}

void camera::computeRay(LPRay pOutRay, D3DXVECTOR2 * screenPos)
{
	//����̽��� ���� ����Ʈ�� ���
	D3DVIEWPORT9 viewPort;
	device->GetViewport(&viewPort);

	//��ũ���� ��ġ ������ ���
	float factorX = screenPos->x / viewPort.Width;
	float factorY = (1.0f - (screenPos->y / viewPort.Height));

	//0 ~ 1 => -1 ~ 1
	factorX = factorX * 2.0f - 1.0f;
	factorY = factorY * 2.0f - 1.0f;

	//��������� ���
	D3DXMATRIXA16 matProj = this->getProjectionMatrix();

	//[1][][][]
	//[][1][][]
	//[][][1][]
	//[0][0][0][]
	//ȭ�� �����Ϸ��� ������
	D3DXVECTOR3 direction(
		factorX / matProj._11,
		factorY / matProj._22,
		1.0f);

	//ī�޶� ���� ���
	D3DXMATRIXA16 matCamWorld = this->getFinalMatrix();

	//������ ������ ī�޶� ����������� ����Ѵ�
	D3DXVec3TransformNormal(&direction, &direction, &matCamWorld);

	//���⺤�� ����ȭ
	D3DXVec3Normalize(&direction, &direction);

	//Ray�� ����
	pOutRay->direction = direction;
	//Ray�� �������� ī�޶��� ��ġ��
	pOutRay->origin = this->getWorldPosition();
}

bool camera::getWorldPosToScreenPos(D3DXVECTOR2 * pScreenPos, const D3DXVECTOR3 * pWorldPos)
{
	//ī�޶��� ViewProjection ����� ��´�.
	D3DXMATRIXA16 matViewProj = this->getViewProjectionMatrix();

	//���� ��ġ�� ������...
	D3DXVECTOR3 pos;
	D3DXVec3TransformCoord(&pos, pWorldPos, &matViewProj);

	//��������
	//				*-------*<-( 1, 1, 1 )
	//			   /|      /|
	//			  *-------* |
	//            | *-----|-*
	//		      |/      |/
	//(-1,-1, 0)->*-------* 

	//0 ~ 1 ���� ���� ������ �̸� ī�޶� �ڿ� �ִٴ¿���
	if (pos.z < 0.0f || pos.z > 1.0f) return false;

	//ȭ�� �߾ӿ� �ִٸ� pos xy ���� ������ ����.
	//pos.x == 0  
	//pos.y == 0

	//ȭ�� ���� ������
	float halfScreenW = WINSIZEX * 0.5f;
	float halfScreenH = WINSIZEY * 0.5f;

	//���� ȭ�� ��ġ
	pScreenPos->x = halfScreenW + (halfScreenW * pos.x);
	pScreenPos->y = halfScreenH - (halfScreenH * pos.y);	//Y ���� ����

	return true;
}

void camera::readyRenderToTexture(int width, int height)
{
	SAFE_RELEASE(_renderTexture);
	SAFE_RELEASE(_renderSurface);

	// renderTarget �� Texture ����.
	device->CreateTexture(
		width,
		height,
		1,
		D3DUSAGE_RENDERTARGET,		// render target �� texture
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&_renderTexture,
		NULL
	);

	// render �� surface
	device->CreateDepthStencilSurface(
		width,
		height,
		D3DFMT_D24S8,		// Depth�� 24 bit, Stencil 8 bit
		D3DMULTISAMPLE_NONE,	// ���ø� ��Ƽ�󸮾ƽ��� ����
		0,						// ��Ƽ ���ø� ����Ƽ�� 0
		true,					// ���� ��ü�� ���� ���۳����� ���� �ϰڴ°�?
		&_renderSurface,
		NULL
	);
}

void camera::readyShadowTexture(int size)
{
	SAFE_RELEASE(_renderTexture);
	SAFE_RELEASE(_renderSurface);

	//RenderTarget �� Texture �����
	device->CreateTexture(
		size,						//Texture ���� �ػ� 
		size,						//Texture ���� �ػ�
		1,							//�Ӹ�ü�� ����
		D3DUSAGE_RENDERTARGET,		//RenderTarget �� Texture�̴�
		D3DFMT_R32F,				//��� �÷� ���۸� Red �� �ٻ�� ( 4byte float �Ǽ��� ��� )
		D3DPOOL_DEFAULT,			//RenderTarget �� Texture �� Pool �� Default
		&_renderTexture,			//������ Texture �޾ƿ� ������
		NULL
	);

	//Render �� Surface 
	device->CreateDepthStencilSurface(
		size,						//Texture ���� �ػ� 
		size,						//Texture ���� �ػ�
		D3DFMT_D24S8,				//Deapth �� 24 ��Ʈ Stencil �� 8 ��Ʈ	
		D3DMULTISAMPLE_NONE,		//��Ƽ ���ø� ��Ƽ�˸��ƽ��� �������� �ʴ´�, 
		0,							//��Ƽ ���ø� ����Ƽ�� 0
		TRUE,						//���� ��ü�� ���� �۹������� �������� �ʴ�? ( TRUE �� ���� ���۱�ü �ɶ� ������ ���� ���۳����� ������� �ʴ´� )
		&_renderSurface,			//���� ������...
		NULL);
}


void camera::renderTextureBegin(DWORD backColor)
{
	//�� ����̽��� Target ������ ǥ��� DepthStencil ������ ǥ�������� ���
	device->GetRenderTarget(0, &_deviceTargetSurface);
	device->GetDepthStencilSurface(&_deviceDepthAndStencilSurface);

	//RenderTexture �� Surface �� ��´�
	LPDIRECT3DSURFACE9 texSurface = NULL;
	if (SUCCEEDED(_renderTexture->GetSurfaceLevel(0, &texSurface)))
	{
		//Texture ǥ���� Device �� Target ���۷� �����Ѵ�
		device->SetRenderTarget(0, texSurface);

		//���õ� Surface ������ �ٷ� �����ִ� ���Ǹ� ������
		SAFE_RELEASE(texSurface);
	}

	//Depth ���ۿ� Stencil ������ Surface �� _renderSurface ����
	device->SetDepthStencilSurface(_renderSurface);

	//����̽� ���� Ŭ���� (��� ������ ���õ� Textuer �� Surface �� Ŭ���� �ȴ�)
	device->Clear(0, NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		backColor,
		1.0f,
		0);
}

void camera::renderTextureEnd()
{
	//Render Texture �� �׸� �۾��� �������� �ٽ� ���󺹱��ϴ� ����
	device->SetRenderTarget(0, _deviceTargetSurface);
	device->SetDepthStencilSurface(_deviceDepthAndStencilSurface);
	//���õ� Surface ������ �ٷ� �����ִ� ���Ǹ� ������
	SAFE_RELEASE(_deviceTargetSurface);
	SAFE_RELEASE(_deviceDepthAndStencilSurface);
}
