// camera.h
// 17.12.22
#pragma once
#include "transform.h"
#include "Frustum.h"

class camera : public transform
{
private:
	// ȭ�� : ī�޶�� �����ϴ� ����� �þ�
	float			_fov;				// ī�޶� ȭ��
	float			_camNear;			// ī�޶� �ٰŸ�
	float			_camFar;			// ī�޶� ���Ÿ�
	float			_aspect;			// ī�޶� ��Ⱦ��
	bool			_isOrtho;			// ������? (��������)
	float			_orthoSize;			// ������ ������
	Frustum			_frustum;			// ��������


	LPDIRECT3DTEXTURE9 _renderTexture;	// ������ �� texture
	LPDIRECT3DSURFACE9 _renderSurface;	// Texture Depth ���ۿ� 
	// Stencil ���۰� �ִ� Surface
	LPDIRECT3DSURFACE9 _deviceTargetSurface;
	LPDIRECT3DSURFACE9 _deviceDepthAndStencilSurface;

protected:
	D3DXMATRIXA16	_matView;			// �� ���
	D3DXMATRIXA16	_matProjection;		// ���� ���
	D3DXMATRIXA16	_matViewProjection;	// �� ���� ���	

public:
	camera();
	~camera() {};

	// ī�޶� update()
	void update();	
	void updateFrustum();
	//������İ� ����� ������Ʈ
	void updateMatrix();

	// ī�޶� ���� ��� ��������
	D3DXMATRIXA16 getViewMatrix() { return _matView; };
	D3DXMATRIXA16 getProjectionMatrix() { return _matProjection; };
	D3DXMATRIXA16 getViewProjectionMatrix() { return _matViewProjection; };

	// frustrum get, set
	Frustum getFrustum() { return _frustum; }
	void setFrustum(Frustum frustum) { _frustum = frustum; }

	// aspect get, set
	float getAspect() { return _aspect; }
	void setAspect(float aspect) { _aspect = aspect; }

	// ȭ�� get, set
	float getFov() { return _fov; }
	void setFov(float fov) { _fov = fov; };

	// cma near get, set
	float getCamNear() { return _camNear; }
	void setCamNear(float camNear) { _camNear = camNear; }

	// cam far get , set
	float getCamFar() { return _camFar; }
	void setCamFar(float camFar) { _camFar = camFar; }

	// ������? (��������)
	bool getOrtho() { return _isOrtho; }
	void setOrtho(bool ortho) { _isOrtho = ortho; }

	// ������ ������ _orthoSize
	float getOrthoSize() { return _orthoSize; }
	void setOrthoSize(float orthosize) { _orthoSize = orthosize; }
		
	//ȭ���� ��ġ�� ������ ī�޶��� �������̸� ��� �Լ�
	void computeRay(LPRay pOutRay, D3DXVECTOR2* screenPos);
	//���� ��ġ��  ȭ���� ��ġ�� ��´�.
	bool getWorldPosToScreenPos(D3DXVECTOR2* pScreenPos, const D3DXVECTOR3* pWorldPos);

	// ���� Texture�� �غ� �Լ�
	void readyRenderToTexture(int width, int height);

	//Shadow Map Texture�� �غ��Ѵ�.
	void readyShadowTexture(int size);

	// ���� texture�� ������ ����
	void renderTextureBegin(DWORD backColor);

	// ���� texture�� ������ ����
	void renderTextureEnd();

	// ���� texture get �Լ�
	LPDIRECT3DTEXTURE9 getRenderTexture() { return _renderTexture; }

};