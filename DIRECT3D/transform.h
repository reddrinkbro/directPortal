#pragma once

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2

class transform
{
protected:
	// ����ü(�޸𸮸� �������� ����� �� �ֵ��� ���ش�)
	// �迭�ε� ���� �����ϰ�, ���⺤�ͷε� ���ٰ����ϰ� �����
	// ���⺤��
	union
	{
		struct
		{
			D3DXVECTOR3 axis[3];
		};
		struct
		{
			// �ٶ󺸴� ���� ���� �׻� ����ȭ �Ǿ� �־�� �Ѵ�
			// �׻� ����ȭ �Ǿ� �ִ�
			D3DXVECTOR3 right;
			D3DXVECTOR3 up;
			D3DXVECTOR3 forward;
		};
	};

	D3DXVECTOR3			_position;	// ��ġ����
	D3DXVECTOR3			_scale; 	// ũ�⺤��(������)
	D3DXMATRIXA16		_matFinal; 	// �������(�������)


									// �������
									// �θ� ���������� �������� ���(���� �θ� ���ٸ� matFinal �̶� ����)
	D3DXMATRIXA16		_matLocal;
	// �θ� ����(null �̸� �θ� ���¾�)
	transform*			_parent;
	// �ڽ� ����(null �̸� �ڽ��� ���¾�)
	transform*			_firstChild;
	// ���� ����(null �̸� �ܾƵ�)
	transform*			_nextSibling;

	// Ʈ������ ��ȯ�� �ڵ����� ������Ʈ �Ҳ���?
	// ���� �̰� false�̸� ����ڰ� ���� updateTransform() ȣ���� �ؾ��Ѵ�
	bool				_isAutoUpdate;
	// ����Ʈ��Ʈ���� ���� ī�޶� �ޱ۰�
	float				_nowAngleH;			// ����
	float				_nowAngleV;			// ������
	float				_maxAngleV;			// ���� �ִ밢
	float				_minAngleV;			// ���� ������
	float				_sensitivityH;		// ���� �ΰ���
	float				_sensitivityV;		// ���� �ΰ���
	D3DXVECTOR3			_nowVelocity;		// ���� ����� �ӵ��� ���� ����
	float				_accelate;			// �ʴ� �̵� ������
	float				_nowSpeed;			// ���� �ӵ�
	float				_maxSpeed;			// �ְ� �ӵ�
	bool				_isSetMouse;		// ���콺�� ��� ��������
public:
	transform();
	~transform() {}

	// Ư�� Child �� ���ڽ����� ���δ�
	void addChild(transform* pNewChild);
	// Ư�� Transform �� �ٴ´�
	void attachTo(transform* pNewParent);
	// �θ�� �ȳ�
	void releaseParent();

	// ��ġ�� ���� ��ǥ��� ����
	void setWorldPosition(float x, float y, float z);
	void setWorldPosition(D3DXVECTOR3 pos);

	// ��ġ�� ���� ��ǥ��� ����(�θ� �ִ� ��� �θ��� �������� ��ġ)
	void setLocalPosition(float x, float y, float z);
	void setLocalPosition(D3DXVECTOR3 pos);

	// �ڱ��ڽ� �������� �̵��ϱ�(�����ǥ)
	void movePositionSelf(float dx, float dy, float dz);
	void movePositionSelf(D3DXVECTOR3 delta);

	// ���� �������� �̵��ϱ�(������ǥ)
	void movePositionWorld(float dx, float dy, float dz);
	void movePositionWorld(D3DXVECTOR3 delta);

	// �θ� �ִ� ��� ���� �������� �̵� ��Ų��.
	void movePositionLocal(float dx, float dy, float dz);
	void movePositionLocal(D3DXVECTOR3 delta);

	// ������ ����
	void setScale(float x, float y, float z);
	void setScale(D3DXVECTOR3 scale);

	// �����ϸ�
	void scaling(float dx, float dy, float dz);
	void scaling(D3DXVECTOR3 delta);

	// ���� �������� ȸ��
	void rotateWorld(float angleX, float angleY, float angleZ);
	void rotateWorld(D3DXVECTOR3 angle);

	// �ڽ��� �� �������� ȸ��
	void rotateSelf(float angleX, float angleY, float angleZ);
	void rotateSelf(D3DXVECTOR3 angle);

	// �θ� �ִ� ��� �θ��� ���� �� �������� ȸ��
	void rotateLocal(float angleX, float angleY, float angleZ);
	void rotateLocal(D3DXVECTOR3 angle);

	// Ư�� ������ �ٶ󺸰� ȸ���ض�
	void lookDirection(D3DXVECTOR3 dir, D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0));
	// Ư�� ������ �ٶ󺸴µ� angle ��ŭ ȸ���ض�
	void lookDirection(D3DXVECTOR3 dir, float angle);
	// Ư�� ��ġ�� �ٶ󺸰� ȸ���ض�
	void lookPosition(D3DXVECTOR3 pos, D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0));
	// Ư�� ��ġ�� �ٶ󺸴µ� angle ��ŭ ȸ���ض�
	void lookPosition(D3DXVECTOR3 pos, float angle);

	// ������� �̿��� Ư�� ȸ�������� ȸ���� ����
	// void setRotate(float angleX, float angleY, float angleZ);
	void setRotateWorld(float eAngleX, float eAngleY, float aAngleZ);
	void setRotateLocal(float eAngleX, float eAngleY, float aAngleZ);

	//  ȸ�� ����� �־��ָ� �� ȸ�� ��Ĵ�� ȸ���Ѵ�.
	void setRotateWorld(const D3DXMATRIXA16& matWorldRotate);
	void setRotateLocal(const D3DXMATRIXA16& matWorldRotate);

	//  ȸ�� ������� �־��ָ� �� ȸ���� ��� ȸ���Ѵ�.
	void setRotateWorld(const D3DXQUATERNION& matWorldRotate);
	void setRotateLocal(const D3DXQUATERNION& matWorldRotate);

	// �ڽ��� ȸ�� ���� from �� to ������ ȸ������ŭ ȸ������(���麸��) �Ͽ� ����
	void rotateSlerp(transform& from, transform& to, float t);

	// �ڽ��� ��ġ ���� from �� to ������ ��ġ��ŭ �������� �Ͽ� ����
	void positionLerp(transform& from, transform& to, float t);

	// �ڽ��� ��� ������ from �� to ������ ������ŭ ���� �Ͽ� ����
	void interpolate(transform& from, transform& to, float t);

	// Ʈ������ ������Ʈ (Ʈ�������� �������� ����Ǿ����� ����Ѵ�)
	void updateTransform();

	// ����̽��� �ڽ��� Ʈ�������� ����� ����
	void setDeviceWorld();
	// ����̽��� �ڽ��� Ʈ�������� ��� ����
	void setDeviceView();

	// �������(�������) ��������
	D3DXMATRIXA16 getFinalMatrix() { return _matFinal; }
	void setWorldMatrix(const D3DXMATRIXA16 & matWorld);
	D3DXMATRIXA16 getWorldRotateMatrix();
	D3DXQUATERNION getWorldRotateQuaternion();
	// ��ġ ���� ��´�
	D3DXVECTOR3 getWorldPosition();
	D3DXVECTOR3 getLocalPosition();
	// ���� ��´� (�������)
	void getScaledAxies(D3DXVECTOR3* pVecArr);
	void getUnitAxies(D3DXVECTOR3* pVecArr);
	D3DXVECTOR3 getScaledAxis(int axisNum);
	D3DXVECTOR3 getUnitAxis(int axisNum);
	D3DXVECTOR3 getForward(bool isNormalize = true);
	D3DXVECTOR3 getUp(bool isNormalize = true);
	D3DXVECTOR3 getRight(bool isNormalize = true);

	//  ������ �� ���
	D3DXVECTOR3 getScale() { return _scale; }

	// ī�޶� ����Ʈ ��Ʈ��
	void defaultControl(bool isOn);
	// ������Ʈ ��Ʈ��
	void defaultMyControl();

	// Ʈ�������� ���� ����� �׸���
	void renderGizmo(float scale = 1.0f);

	//���� ������ ��������
	float getNowAngleH(void) { return _nowAngleH; }
	float getNowAngleV(void) { return _nowAngleV; }
	void setNowAngeH(float nowAngleH) { _nowAngleH = nowAngleH; }
	void setIsMouse(bool isSetMouse) { _isSetMouse = isSetMouse; }
};

