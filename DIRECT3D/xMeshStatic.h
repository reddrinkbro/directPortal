// xMeshStatic.h
#pragma once
#include "xMesh.h"

class xMeshStatic : public xMesh
{
public:
	friend class physicManager;						// �����Ŵ����� ��ģ��
	static LPD3DXEFFECT			_xMeshEffect;		// ���� �޽��� �׸��� ����Ʈ
	static LPDIRECT3DTEXTURE9	_defDiffuseTex;		// ��ǻ�� �ؽ���
	static LPDIRECT3DTEXTURE9	_defNormalTex;		// �븻 �ؽ���
	static LPDIRECT3DTEXTURE9	_defSpecularTex;	// ����ŧ�� �ؽ���
	static LPDIRECT3DTEXTURE9	_defEmssionTex;		// �̹̼� �ؽ���

public:
	LPD3DXMESH					_mesh;				// xMesh
	DWORD						_materialsNum;		// �ε��� �޽��� ���� ����
	vector<LPDIRECT3DTEXTURE9>	_vDiffuseTex;		// �ε��� �޽����� ���Ǵ� ��ǻ�� �ؽ���
	vector<LPDIRECT3DTEXTURE9>	_vNormalTex;
	vector<LPDIRECT3DTEXTURE9>	_vSpecularTex;
	vector<LPDIRECT3DTEXTURE9>	_vEmissionTex;
	vector<D3DMATERIAL9>		_vMaterials;		// �ε��� �޽����� ���Ǵ� ���׸���

	vector<D3DXVECTOR3>			_vertices;
	vector<D3DXVECTOR3>			_normals;
	vector<DWORD>				_indices;
	DWORD						_triNum;

public:
	D3DXVECTOR3					_boundMin;			// �ٿ���� �ּ� ��ġ
	D3DXVECTOR3					_boundMax;			// �ٿ���� �ִ� ��ġ
	D3DXVECTOR3					_boundCenter;		// �ٿ���� ����
	D3DXVECTOR3					_boundSize;			// �ٿ���� ũ��
	D3DXVECTOR3					_boundHalfSize;		// �ٿ���� ���� ũ��
	float						_boundRadius;		// �ٿ���� ������ ������

public:
	virtual HRESULT init(string filePath, D3DXMATRIXA16* matCorrection = NULL) override;
	virtual void release(void) override;
	virtual void render(transform* trans) override;

	// ������� ��� �޽� ����
	void meshCorrection(D3DXMATRIXA16* matCorrection);
	// ī�޶� ����
	static void setCamera(camera* cam);
	//���̽� ������ ���� ( ���̽� �������� ���⼺ �����̴� )
	static void setBaseLight(lightDirection* pDirLight);
	// ����Ʈ ����
	static void setLighting(vector<light*>* pLights);
	static void setLighting(light** pLights, int lightNum);
	//��ũ�� ����
	static void setTechniqueName(string name);

	xMeshStatic() {};
	~xMeshStatic() {};

};
