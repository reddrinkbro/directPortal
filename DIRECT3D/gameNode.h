#pragma once

//����Ʈ�� Ŭ����
class quadTree;

class gameNode
{
private:
	typedef struct SCENE_VERTEX
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 uv;
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
	}SCENE_VERTEX, *LPSCENE_VETEX;

public :
	camera* _mainCamera;
	lightDirection*			_sceneBaseDirectionLight;	//���� ��ġ�� �⺻������
protected:
	LPDIRECT3DCUBETEXTURE9	_environmentTexture;		//ȯ�� �ؽ���
	LPD3DXMESH				_environmemtSphereMesh;		//ȯ�� ��
	LPD3DXEFFECT			_environmentEffect;			//ȯ�� ����Ʈ

	SCENE_VERTEX			_scenePlaneVertex[4];		//����� ���ؽ�
	WORD					_scenePlaneIndex[6];		//����� �ε���
	LPD3DXEFFECT			_postEffect;				//����Ʈ ����Ʈ

	camera*					_directionLightCamera;		//���⼺ ������ ���� ī�޶�
	float					_shadowDistance;			//�׸��� �Ÿ�
	
public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);
	virtual void renderSprite(void) {};
	void renderEnvironment(void);
	void setEnvironment(string cubeFilePath);
	void readyShadowMap(vector<baseObject*>* renderObjects, terrain* pTerrain = NULL);
	void addSound(void);
	// ���� ���ν���
	LRESULT CALLBACK MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	gameNode() { 
		_mainCamera = new camera;
		_sceneBaseDirectionLight = new lightDirection;
	};
	virtual ~gameNode() {}
};