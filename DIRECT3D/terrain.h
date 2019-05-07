#pragma once

class quadTree;

class terrain
{
public:
	//���� ���� ����ü
	typedef struct tagTerrainVertex
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 binormal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR2 baseUV;
		D3DXVECTOR2 tileUV;
	}TERRAINVERTEX, *LPTERRAINVERTEX;
	//���� �ε��� ����ü
	typedef struct tagTerrainIndex
	{
		DWORD _0;
		DWORD _1;
		DWORD _2;
	}TERRAININDEX, *LPTERRAININDEX;

private:
	quadTree*						_quadTree;
	LPDIRECT3DVERTEXBUFFER9			_terrainVb;		//���� ��������
	LPDIRECT3DINDEXBUFFER9			_terrainIb;		//���� �ε�������
	LPDIRECT3DVERTEXDECLARATION9	_terrainDecl;	//���� ��������

	LPTERRAINVERTEX					_terrainVertices;	//���� ��������
	LPD3DXEFFECT					_terrainEffect;		//���� Effect
	LPDIRECT3DTEXTURE9				_heightMap;			//���� ���̸� �ؽ���
	LPDIRECT3DTEXTURE9				_texTile_0;			//���� 0 �� Ÿ�� �ؽ���
	LPDIRECT3DTEXTURE9				_texTile_1;			//���� 1 �� Ÿ�� �ؽ���
	LPDIRECT3DTEXTURE9				_texTile_2;			//���� 2 �� Ÿ�� �ؽ���
	LPDIRECT3DTEXTURE9				_texTile_3;			//���� 3 �� Ÿ�� �ؽ���
	LPDIRECT3DTEXTURE9				_texSlat;			//���÷��� �ؽ���

	float _cellScale;				//�� ����
	float _heightScale;				// ���� ������(�ȼ��÷��� 255�� �� ����)
	int _verNumX;					//���� ������
	int _verNumZ;					//���� ������
	int _totalVerNum;				//��ü ������
	int _cellNumX;					//���� ����
	int _cellNumZ;					//���� ����
	int _totalCellNum;				//��ü ����
	int _totalTriangle;				//��ü �ﰢ����
	float _terrainSizeX;			//���� ���� ������
	float _terrainSizeZ;			//���� ���� ������

									//��Ȯ�� ���� ����� ���� �ʿ��� ������
	float _terrainStartX;			//���� ���� ��ġX
	float _terrainStartZ;			//���� ���� ��ġZ
	float _terrainEndX;				//���� ���� ��ġX
	float _terrainEndZ;				//���� ���� ��ġZ

	D3DXVECTOR3* _vertexPosArray;	//���� ��ġ �迭
	LPTERRAININDEX	_indices;		//�ε��� ����ü
		

public:
	HRESULT init(int vertexNum, float cellSize);
	HRESULT init(
		char * heightMapName,		// ���̸� �̸�
		char * tile_0,				// Ÿ�� �̹��� 0
		char * tile_1,				// Ÿ�� �̹��� 1
		char * tile_2,				// Ÿ�� �̹��� 2
		char * tile_3,				// Ÿ�� �̹��� 3
		char * tileSplat,			// Ÿ�� �̹��� ��Ʈ��(RGB BLACK)
		float cellSize,				// �ϳ��� ��ũ��
		float heightScale,			// ���� ������
		int smoothLevel,			// ������ ó�� Ƚ��
		int tileNum);				// Ÿ�ϸ� ����
	void release(void);
	void update(void);
	void render(void);
	void render(camera* cam, lightDirection* directionLight);
	void render(camera* cam, lightDirection* directionLight, camera* directionLightCamera);
	void renderShadow(camera* directionLightCam);

	// get set terrainEffect
	LPD3DXEFFECT getTerrainEffect() { return _terrainEffect; }
	void setTerrainEffect(LPD3DXEFFECT terrainEffect) { _terrainEffect = terrainEffect; }

	// Ray ��Ʈ ��ġ�� ����.
	bool isIntersectRay(D3DXVECTOR3 * pOut, LPRay ray);

	// X, Z ��ġ�� ������ ���̸� ����.
	float getHeight(float x, float z);

	// �ش� X, Z ��ġ�� ��� ����
	bool getSlant(D3DXVECTOR3* pOut, float gravityPower, float x, float z);

	terrain() {}
	~terrain() {}

private:
	// ����Ŭ���� ���ο����� ��밡���� �Լ�
	// ���� ���� �����
	HRESULT createTerrain(int smooth, int tileNum);
	// ���� ������
	void smoothTerrain(int passed);
};

