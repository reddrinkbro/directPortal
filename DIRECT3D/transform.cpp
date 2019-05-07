#include "stdafx.h"
#include "transform.h"

// �����ڷ� �ʱ�ȭ
transform::transform()
{
	// �θ��ڽ� �ʱ�ȭ
	_parent = NULL;
	_firstChild = NULL;
	_nextSibling = NULL;

	// ���⺤�� �ʱ�ȭ
	this->right = D3DXVECTOR3(1, 0, 0);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);
	// ��ġ �ʱ�ȭ
	_position = D3DXVECTOR3(0, 0, 0);
	// ũ�� �ʱ�ȭ
	_scale = D3DXVECTOR3(1, 1, 1);

	// �ڵ� ������Ʈ �ʱ�ȭ
	_isAutoUpdate = TRUE;

	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();

	_nowAngleH = 135.0f;
	_nowAngleV = 0.0f;
	_maxAngleV = 85.0f;
	_minAngleV = -80.0f;
	_sensitivityH = 0.03f;
	_sensitivityV = 0.03;
	_nowVelocity = D3DXVECTOR3(0, 0, 0);
	_accelate = 30.0f;
	_nowSpeed = 5.0f;
	_maxSpeed = 10.0f;
	_isSetMouse = false;
}

// Ư�� Child �� ���ڽ����� ���δ�
void transform::addChild(transform * pNewChild)
{
	// �̹� �������� ���ʿ� ����
	if (pNewChild->_parent == this) return;

	// ���̻��� �������� ���÷��� �θ�� ���� �����...
	pNewChild->releaseParent();

	// �θ��� ������� ��ǥ������ �����ϱ����� 
	// �θ��� final ������� ���Ѵ�.
	D3DXMATRIXA16 matInvFinal;
	D3DXMatrixInverse(&matInvFinal, NULL, &this->_matFinal);

	// �ڽ��� Position �� Axis �� Scale ����
	D3DXVec3TransformCoord(&pNewChild->_position, &pNewChild->_position, &matInvFinal);

	// ��3�� ��ȯ�ϰ� 
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(pNewChild->axis + i, pNewChild->axis + i, &matInvFinal);
	}

	// 3�࿡ ���� ���̰��� ��´�.
	pNewChild->_scale.x = D3DXVec3Length(&pNewChild->right);
	pNewChild->_scale.y = D3DXVec3Length(&pNewChild->up);
	pNewChild->_scale.z = D3DXVec3Length(&pNewChild->forward);

	// ����ȭ
	D3DXVec3Normalize(&pNewChild->right, &pNewChild->right);
	D3DXVec3Normalize(&pNewChild->up, &pNewChild->up);
	D3DXVec3Normalize(&pNewChild->forward, &pNewChild->forward);

	// ���ο� ���� �θ�� ���� �ȴ�.
	pNewChild->_parent = this;

	// ���� �ڽĳ� ������
	transform* pChild = this->_firstChild;

	// �ڽ��� ���� ������ ���ų����̶��...
	if (pChild == NULL)
	{
		// �Ƚ��ϰ� �߰�
		this->_firstChild = pNewChild;
		pNewChild->_parent = this;
	}
	else // �밡���� ����.
	{
		while (pChild != NULL)
		{
			// ���� �� �ڸ��� ã�Ҵٸ�...
			if (pChild->_nextSibling == NULL)
			{
				pChild->_nextSibling = pNewChild;
				pNewChild->_parent = this;
				break;
			}
			pChild = pChild->_nextSibling;
		}
	}

	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	this->updateTransform();
}

// Ư�� Transform �� �ٴ´�
void transform::attachTo(transform * pParent)
{
	pParent->addChild(this);
	// ���� �������� �پ����� ������������ 000���� ����� ����
	this->setLocalPosition(0, 0, 0);
}

// �θ�� �ȳ�
void transform::releaseParent()
{
	// �θ� ����? ������ �θ�� �ȳ��� �ϵ� ����
	if (_parent == NULL) return;

	// �θ�� ���� ���� ���� �θ���� �ڽĿ��� �����...
	transform* pChild = _parent->_firstChild;

	// ���� �θ��� ù°�ڽ��̴�?
	if (pChild == this)
	{
		// ������ �ڽ��� ù��° �ڽ��� �ȴ�.
		_parent->_firstChild = this->_nextSibling;

		// ��������� ���� ���´�.
		this->_nextSibling = NULL;
	}
	else
	{
		while (pChild != NULL)
		{
			// ���� �ڽ��� ������ ����?
			if (pChild->_nextSibling == this)
			{
				pChild->_nextSibling = this->_nextSibling;
				// �������� ���� ���´�.
				this->_nextSibling = NULL;
				break;
			}
			// ���� �ڽĺ���.
			pChild = pChild->_nextSibling;
		}
	}

	// �θ�� ���� �����...
	_parent = NULL;

	// �ڽ��� ���� ���� ��ġ�� ���� ������ �ʿ��ϴ�.
	// ��¥���� ��ġ�� matFinal �� �� ������ �ִ�.

	// ���� ��ġ ����
	this->_position.x = this->_matFinal._41;
	this->_position.y = this->_matFinal._42;
	this->_position.z = this->_matFinal._43;

	// 3�� ���´�.
	D3DXVECTOR3 forwardScaled(this->_matFinal._31, this->_matFinal._32, this->_matFinal._33);
	D3DXVECTOR3 upScaled(this->_matFinal._21, this->_matFinal._22, this->_matFinal._23);
	D3DXVECTOR3 rightScaled(this->_matFinal._11, this->_matFinal._12, this->_matFinal._13);

	// 3�࿡�� ������ ����
	float scaleX = D3DXVec3Length(&rightScaled);
	float scaleY = D3DXVec3Length(&upScaled);
	float scaleZ = D3DXVec3Length(&forwardScaled);

	// ����ȭ
	D3DXVECTOR3 forwardUnit;
	D3DXVECTOR3 upUnit;
	D3DXVECTOR3 rightUnit;
	D3DXVec3Normalize(&rightUnit, &rightScaled);
	D3DXVec3Normalize(&upUnit, &upScaled);
	D3DXVec3Normalize(&forwardUnit, &forwardScaled);

	// ����ȭ�� 3�� ����
	this->forward = forwardUnit;
	this->right = rightUnit;
	this->up = upUnit;

	// ������ ����
	this->_scale.x = scaleX;
	this->_scale.y = scaleY;
	this->_scale.z = scaleZ;

	// ������ ������ �Ǿ���
	// �θ�� ����� ������� Update �ѹ� ��
	this->updateTransform();
}

// ��ġ�� ���� ��ǥ��� ����
void transform::setWorldPosition(float x, float y, float z)
{
	D3DXVECTOR3 pos(x, y, z);

	// �θ� �ִٸ� �θ��� �������� ��ġ�� �ٲ��...
	if (_parent != NULL)
	{
		// �θ��� ���� ����� �����
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &_parent->_matFinal);

		// �� ���࿭�� Pos ����
		D3DXVec3TransformCoord(&pos, &pos, &matInvParentFinal);
	}

	_position = pos;

	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();
}

void transform::setWorldPosition(D3DXVECTOR3 pos)
{
	// �θ� �ִٸ� �θ��� �������� ��ġ�� �ٲ��...
	if (_parent != NULL)
	{
		// �θ��� ���� ����� �����
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &_parent->_matFinal);

		// �� ���࿭�� Pos ����
		D3DXVec3TransformCoord(&pos, &pos, &matInvParentFinal);
	}

	_position = pos;

	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();
}

// ��ġ�� ���� ��ǥ��� ����(�θ� �ִ� ��� �θ��� �������� ��ġ)
void transform::setLocalPosition(float x, float y, float z)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;
	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();
}

void transform::setLocalPosition(D3DXVECTOR3 pos)
{
	_position = pos;
	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();
}

// �ڱ��ڽ� �������� �̵��ϱ�(�����ǥ)
void transform::movePositionSelf(float dx, float dy, float dz)
{
	// �̵� ����
	D3DXVECTOR3 move(0, 0, 0);

	// �ڽ��� �̵� �� ��´�.
	D3DXVECTOR3 moveAxis[3];
	this->getUnitAxies(moveAxis);
	move += moveAxis[0] * dx;
	move += moveAxis[1] * dy;
	move += moveAxis[2] * dz;

	// ���� �̵�
	D3DXVECTOR3 nowWorldPos = this->getWorldPosition();

	// �Ʒ��� �Լ����� TransformUpdate �� �Ͼ
	this->setWorldPosition(nowWorldPos + move);
}

void transform::movePositionSelf(D3DXVECTOR3 delta)
{
	// �̵� ����
	D3DXVECTOR3 move(0, 0, 0);

	// �ڽ��� �̵� �� ��´�.
	D3DXVECTOR3 moveAxis[3];
	this->getUnitAxies(moveAxis);
	move += moveAxis[0] * delta.x;
	move += moveAxis[1] * delta.y;
	move += moveAxis[2] * delta.z;

	// ���� �̵�
	D3DXVECTOR3 nowWorldPos = this->getWorldPosition();

	// �Ʒ��� �Լ����� TransformUpdate �� �Ͼ
	this->setWorldPosition(nowWorldPos + move);
}

// ���� �������� �̵��ϱ�(������ǥ)
void transform::movePositionWorld(float dx, float dy, float dz)
{
	// �̵� ����
	D3DXVECTOR3 move(dx, dy, dz);

	// ���� �̵�
	D3DXVECTOR3 nowWorldPos = this->getWorldPosition();

	// �Ʒ��� �Լ����� TransformUpdate �� �Ͼ
	this->setWorldPosition(nowWorldPos + move);
}

void transform::movePositionWorld(D3DXVECTOR3 delta)
{
	// ���� �̵�
	D3DXVECTOR3 nowWorldPos = this->getWorldPosition();

	// �Ʒ��� �Լ����� TransformUpdate �� �Ͼ
	this->setWorldPosition(nowWorldPos + delta);
}

void transform::movePositionLocal(float dx, float dy, float dz)
{
	_position.x += dx;
	_position.y += dy;
	_position.z += dz;
	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();
}

void transform::movePositionLocal(D3DXVECTOR3 delta)
{
	_position += delta;
	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();
}

// ������ ����
void transform::setScale(float x, float y, float z)
{
	_scale.x = x;
	_scale.y = y;
	_scale.z = z;
	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();
}

void transform::setScale(D3DXVECTOR3 scale)
{
	_scale = scale;
	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();
}

// �����ϸ�
void transform::scaling(float dx, float dy, float dz)
{
	_scale.x += dx;
	_scale.y += dy;
	_scale.z += dz;
	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();
}

void transform::scaling(D3DXVECTOR3 delta)
{
	_scale += delta;
	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();
}

// ���� �������� ȸ��
void transform::rotateWorld(float angleX, float angleY, float angleZ)
{
	// �θ� �ִ� ���
	if (_parent)
	{
		// ��¥�ο��� ��
		D3DXVECTOR3 worldAxis[3];
		this->getUnitAxies(worldAxis);

		// �� �࿡ ���� ȸ�� ���
		D3DXMATRIXA16 matRotateX, matRotateY, matRotateZ;
		D3DXMatrixRotationX(&matRotateX, angleX);
		D3DXMatrixRotationY(&matRotateY, angleY);
		D3DXMatrixRotationZ(&matRotateZ, angleZ);

		/*
		// ������ ���� �ϸ� ȸ�� ����� 2�� �����ϱ�.....

		// ���� ȸ�� ���
		D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;

		// ���� ȸ�� ��� ��� ȸ�� ��Ų��.
		for( int i = 0 ; i < 3 ; i++ )
		D3DXVec3TransformNormal( worldAxis + i, worldAxis + i, &matRotate );

		// �θ��� ����ķ� �ٽ� ȸ��
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse( &matInvParentFinal, NULL, &_parent->_matFinal );


		// ���� ȸ�� ��� ��� ȸ�� ��Ų��.
		for( int i = 0 ; i < 3 ; i++ )
		D3DXVec3TransformNormal( worldAxis + i, worldAxis + i, &matInvParentFinal );
		*/

		// ���� ȸ�� ��ĸ��鶧 �̸� ����ļ����� ���س����� 1 ���� ȸ����Ű�� �ȴ�.

		// ���� ȸ�� ���
		D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;

		// �θ��� ����ķ� �ٽ� ȸ��
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &_parent->_matFinal);

		matRotate = matRotate * matInvParentFinal;

		// ���� ȸ�� ��� ��� ȸ�� ��Ų��.
		for (int i = 0; i < 3; i++)
		{
			D3DXVec3TransformNormal(this->axis + i, worldAxis + i, &matRotate);
		}
		// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
		if (_isAutoUpdate) this->updateTransform();
	}
	// �θ� ���� ���
	else
	{
		rotateLocal(angleX, angleY, angleZ);
	}
}

void transform::rotateWorld(D3DXVECTOR3 angle)
{
	// �θ� �ִ� ���
	if (_parent)
	{
		// ��¥�ο��� ��
		D3DXVECTOR3 worldAxis[3];
		this->getUnitAxies(worldAxis);

		// �� �࿡ ���� ȸ�� ���
		D3DXMATRIXA16 matRotateX, matRotateY, matRotateZ;
		D3DXMatrixRotationX(&matRotateX, angle.x);
		D3DXMatrixRotationY(&matRotateY, angle.y);
		D3DXMatrixRotationZ(&matRotateZ, angle.z);

		// ���� ȸ�� ��ĸ��鶧 �̸� ����ļ����� ���س����� 1 ���� ȸ����Ű�� �ȴ�.

		// ���� ȸ�� ���
		D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;

		// �θ��� ����ķ� �ٽ� ȸ��
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &_parent->_matFinal);

		matRotate = matRotate * matInvParentFinal;

		// ���� ȸ�� ��� ��� ȸ�� ��Ų��.
		for (int i = 0; i < 3; i++)
		{
			D3DXVec3TransformNormal(this->axis + i, worldAxis + i, &matRotate);
		}

		// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
		if (_isAutoUpdate) this->updateTransform();
	}
	// �θ� ���� ���
	else
	{
		rotateLocal(angle.x, angle.y, angle.z);
	}
}

// �ڽ��� �� �������� ȸ��
void transform::rotateSelf(float angleX, float angleY, float angleZ)
{
	// �� �࿡ ���� ȸ�����
	D3DXMATRIXA16 matRotateX, matRotateY, matRotateZ;
	D3DXMatrixRotationAxis(&matRotateX, &this->right, angleX);
	D3DXMatrixRotationAxis(&matRotateY, &this->up, angleY);
	D3DXMatrixRotationAxis(&matRotateZ, &this->forward, angleZ);

	// ���� ȸ�����(�� ��Ʈ�������� ���ϸ� ��)
	// ���ǻ��� x, y, z ���� 3���� ���� ������ ������� �Ѵ� (Yaw->Pitch->Roll) Y->X->Z����
	D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;

	// ���� ȸ�� ��Ĵ�� ȸ����Ű��
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);
	}
	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();
}

void transform::rotateSelf(D3DXVECTOR3 angle)
{
	// �� �࿡ ���� ȸ�����
	D3DXMATRIXA16 matRotateX, matRotateY, matRotateZ;
	D3DXMatrixRotationAxis(&matRotateX, &this->right, angle.x);
	D3DXMatrixRotationAxis(&matRotateY, &this->up, angle.y);
	D3DXMatrixRotationAxis(&matRotateZ, &this->forward, angle.z);

	// ���� ȸ�����(�� ��Ʈ�������� ���ϸ� ��)
	// ���ǻ��� x, y, z ���� 3���� ���� ������ ������� �Ѵ� (Yaw->Pitch->Roll) Y->X->Z����
	D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;

	// ���� ȸ�� ��Ĵ�� ȸ����Ű��
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);
	}
	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();
}

// �θ� �ִ� ��� �θ� ������ ��������� ȸ��
void transform::rotateLocal(float angleX, float angleY, float angleZ)
{
	// �� �࿡ ���� ȸ�����
	D3DXMATRIXA16 matRotateX, matRotateY, matRotateZ;
	D3DXMatrixRotationX(&matRotateX, angleX);
	D3DXMatrixRotationY(&matRotateY, angleY);
	D3DXMatrixRotationZ(&matRotateZ, angleZ);

	// ���� ȸ�� ���
	D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;


	// ���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);
	}
	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();
}

void transform::rotateLocal(D3DXVECTOR3 angle)
{
	// �� �࿡ ���� ȸ�� ���
	D3DXMATRIXA16 matRotateX, matRotateY, matRotateZ;
	D3DXMatrixRotationX(&matRotateX, angle.x);
	D3DXMatrixRotationY(&matRotateY, angle.y);
	D3DXMatrixRotationZ(&matRotateZ, angle.z);

	// ���� ȸ�� ���
	D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;

	// ���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);
	}
	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	this->updateTransform();
}

/*����� ������*/
// Ư�� ������ �ٶ󺸰� ȸ���ض�
void transform::lookDirection(D3DXVECTOR3 dir, D3DXVECTOR3 up)
{
	// ���� ����
	D3DXVECTOR3 newForward = dir;

	// �����ʺ��� ( �Ű������� ���� Up �� ������ ���� )
	D3DXVECTOR3 newRight;
	D3DXVec3Cross(&newRight, &up, &newForward);
	D3DXVec3Normalize(&newRight, &newRight);

	// �� 
	D3DXVECTOR3 newUp;
	D3DXVec3Cross(&newUp, &newForward, &newRight);
	D3DXVec3Normalize(&newUp, &newUp);

	// ���� �θ� �ִٸ�...
	if (_parent)
	{
		// ���ο� �� ���п� �θ� ����� ����....
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &_parent->_matFinal);

		D3DXVec3TransformNormal(&this->forward, &newForward, &matInvParentFinal);
		D3DXVec3TransformNormal(&this->right, &newRight, &matInvParentFinal);
		D3DXVec3TransformNormal(&this->up, &newUp, &matInvParentFinal);
	}
	else
	{
		this->forward = newForward;
		this->right = newRight;
		this->up = newUp;
	}
	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();
}

// Ư�� ������ �ٶ󺸴µ� angle ��ŭ ȸ���ض�
void transform::lookDirection(D3DXVECTOR3 dir, float angle)
{
	// ��¥�ο��� ��
	D3DXVECTOR3 worldAxis[3];
	this->getUnitAxies(worldAxis);

	// ���� ���Ϳ� �ٶ� ������ �������� ����...
	float distRadian = acos(clampMinusPlusOne(D3DXVec3Dot(worldAxis + AXIS_Z, &dir)));

	// �������� ���� ���ٸ� ������...
	if (FLOATZERO(distRadian)) return;

	// ���� ( ������� Ÿ�ٱ����� ������ ���� )
	D3DXVECTOR3 cross;
	D3DXVec3Cross(&cross, worldAxis + AXIS_Z, &dir);
	D3DXVec3Normalize(&cross, &cross);

	// ���������� ������ŭ ȸ�� ��Ű�� ���
	D3DXMATRIXA16 matRotate;
	D3DXMatrixRotationAxis(&matRotate, &cross, min(angle, distRadian));

	// ���� �θ� �ִٸ�...
	if (_parent)
	{
		// ȸ�� ���п� �θ� ����� ����....
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &_parent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}
	// ����
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(this->axis + i, worldAxis + i, &matRotate);
	}
	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();
}

// Ư�� ��ġ�� �ٶ󺸰� ȸ���ض�
void transform::lookPosition(D3DXVECTOR3 pos, D3DXVECTOR3 up)
{
	// ��ġ�� ���� ���⺤�͸� ��´�.
	D3DXVECTOR3 worldPos = this->getWorldPosition();
	D3DXVECTOR3 dir = pos - worldPos;

	D3DXVec3Normalize(&dir, &dir);
	this->lookDirection(dir, up);
}

// Ư�� ��ġ�� �ٶ󺸴µ� angle ��ŭ ȸ���ض�
void transform::lookPosition(D3DXVECTOR3 pos, float angle)
{
	// ��ġ�� ���� ���⺤�͸� ��´�.
	D3DXVECTOR3 worldPos = this->getWorldPosition();
	D3DXVECTOR3 dir = pos - worldPos;

	D3DXVec3Normalize(&dir, &dir);
	this->lookDirection(dir, angle);
}

// ������� �̿��� Ư�� ȸ�������� ȸ���� ����
void transform::setRotateWorld(float eAngleX, float eAngleY, float aAngleZ)
{
	// ����� �غ�
	D3DXQUATERNION quatRot;
	D3DXQuaternionRotationYawPitchRoll(&quatRot, eAngleY, eAngleX, aAngleZ);

	// ������� ���� ����غ�
	D3DXMATRIXA16 matRotate;
	// ������� ���� ȸ�������� ȸ������� ���������.
	D3DXMatrixRotationQuaternion(&matRotate, &quatRot);

	// ���� �θ� �ִٸ�...
	if (_parent)
	{
		// ȸ�� ���п� �θ� ����� ����....
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &_parent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	// �ฮ��
	this->right = D3DXVECTOR3(1, 0, 0);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);

	// ���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);
	}

	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();
}

void transform::setRotateLocal(float eAngleX, float eAngleY, float aAngleZ)
{
	// ����� �غ�
	D3DXQUATERNION quatRot;
	D3DXQuaternionRotationYawPitchRoll(&quatRot, eAngleY, eAngleX, aAngleZ);

	// ������� ���� ����غ�
	D3DXMATRIXA16 matRotate;
	// ������� ���� ȸ�������� ȸ������� ���������.
	D3DXMatrixRotationQuaternion(&matRotate, &quatRot);

	// �ฮ��
	this->right = D3DXVECTOR3(1, 0, 0);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);

	// ���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);
	}
	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();
}
void transform::setRotateWorld(const D3DXMATRIXA16 & matWorldRotate)
{
	D3DXMATRIXA16 matRotate = matWorldRotate;

	// ���� �θ� �ִٸ�...
	if (_parent)
	{
		// ȸ�� ���п� �θ� ����� ����....
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &_parent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	// �ฮ��
	this->right = D3DXVECTOR3(1, 0, 0);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);

	// ���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);
	}
	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();
}

void transform::setRotateLocal(const D3DXMATRIXA16 & matWorldRotate)
{
	D3DXMATRIXA16 matRotate = matWorldRotate;

	// �ฮ��
	this->right = D3DXVECTOR3(1, 0, 0);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);

	// ���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);
	}
	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();
}

// ȸ�� ������� �־��ָ� �� ȸ���� ��� ȸ���Ѵ�.
void transform::setRotateWorld(const D3DXQUATERNION & matWorldRotate)
{
	// ����� �غ�
	D3DXQUATERNION quatRot = matWorldRotate;

	// ������� ���� ����غ�
	D3DXMATRIXA16 matRotate;
	// ������� ���� ȸ�������� ȸ������� ���������.
	D3DXMatrixRotationQuaternion(&matRotate, &quatRot);

	// ���� �θ� �ִٸ�...
	if (_parent)
	{
		// ȸ�� ���п� �θ� ����� ����....
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &_parent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	// �ฮ��
	this->right = D3DXVECTOR3(1, 0, 0);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);

	// ���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);
	}
	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();
}
void transform::setRotateLocal(const D3DXQUATERNION & matWorldRotate)
{
	// ����� �غ�
	D3DXQUATERNION quatRot = matWorldRotate;

	// ������� ���� ����غ�
	D3DXMATRIXA16 matRotate;
	// ������� ���� ȸ�������� ȸ������� ���������.
	D3DXMatrixRotationQuaternion(&matRotate, &quatRot);

	// �ฮ��
	this->right = D3DXVECTOR3(1, 0, 0);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);

	// ���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);
	}
	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();
}

// �ڽ��� ȸ�� ���� from �� to ������ ȸ������ŭ ȸ������(���麸��) �Ͽ� ����
void transform::rotateSlerp(transform & from, transform & to, float t)
{
	t = clamp01(t);

	D3DXQUATERNION fromQuat = from.getWorldRotateQuaternion();
	D3DXQUATERNION toQuat = to.getWorldRotateQuaternion();

	// t �� 0 �� ���ٸ�...
	if (FLOATZERO(t))
	{
		this->setRotateWorld(fromQuat);
	}
	// t �� 1 �� ���ٸ�...
	else if (FLOATEQUAL(t, 1.0f))
	{
		this->setRotateWorld(toQuat);
	}
	// 0 �� 1 ������ ���϶��� ����
	else
	{
		D3DXQUATERNION result;
		// from �� to ��������� t ��ŭ�� ȸ�������� �Ͽ�
		// result ������� ����
		D3DXQuaternionSlerp(&result, &fromQuat, &toQuat, t);

		// ������ ����� ������ ȸ�� ����
		this->setRotateWorld(result);
	}
}

//  �ڽ��� ��ġ ���� from �� to ������ ��ġ��ŭ �������� �Ͽ� ����
void transform::positionLerp(transform & from, transform & to, float t)
{
	t = clamp01(t);

	D3DXVECTOR3 fromWorldPos = from.getWorldPosition();
	D3DXVECTOR3 toWorldPos = to.getWorldPosition();

	// t �� 0 �� ���ٸ�...
	if (FLOATZERO(t))
	{
		this->setWorldPosition(fromWorldPos);
	}
	// t �� 1 �� ���ٸ�...
	else if (FLOATEQUAL(t, 1.0f))
	{
		this->setWorldPosition(toWorldPos);
	}
	// 0 �� 1 ������ ���϶��� ����
	else
	{
		D3DXVECTOR3 result;
		D3DXVec3Lerp(&result, &fromWorldPos, &toWorldPos, t);
		this->setWorldPosition(result);
	}
}

//  �ڽ��� ��� ������ from �� to ������ ������ŭ ���� �Ͽ� ����
void transform::interpolate(transform & from, transform & to, float t)
{
	t = clamp01(t);

	D3DXVECTOR3 resultScale;
	D3DXVECTOR3 resultPosition;
	D3DXQUATERNION resultRotate;

	// t �� 0 �� ���ٸ�...
	if (FLOATZERO(t))
	{
		resultScale = from._scale;
		resultPosition = from.getWorldPosition();
		resultRotate = from.getWorldRotateQuaternion();
	}
	// t �� 1 �� ���ٸ�...
	else if (FLOATEQUAL(t, 1.0f))
	{
		resultScale = to._scale;
		resultPosition = to.getWorldPosition();
		resultRotate = to.getWorldRotateQuaternion();
	}
	// 0 �� 1 ������ ���϶��� ����
	else
	{
		D3DXVECTOR3 fromScale = from._scale;
		D3DXVECTOR3 toScale = to._scale;

		D3DXVECTOR3 fromPosition = from.getWorldPosition();
		D3DXVECTOR3 toPosition = to.getWorldPosition();

		D3DXQUATERNION fromQuat = from.getWorldRotateQuaternion();
		D3DXQUATERNION toQuat = to.getWorldRotateQuaternion();

		// ������ ����
		D3DXVec3Lerp(&resultScale, &fromScale, &toScale, t);

		// ��ġ ����
		D3DXVec3Lerp(&resultPosition, &fromPosition, &toPosition, t);

		// ȸ�� ����
		D3DXQuaternionSlerp(&resultRotate, &fromQuat, &toQuat, t);
	}

	// �ϴ� ���������Ʈ ��� ���´�
	_isAutoUpdate = false;

	this->setScale(resultScale);
	this->setWorldPosition(resultPosition);
	this->setRotateWorld(resultRotate);

	// ���󺹱�
	_isAutoUpdate = true;

	// ���� ����Ǿ����� ��������� ������Ʈ ����� �Ѵ�
	if (_isAutoUpdate) this->updateTransform();
}

// Ʈ������ ������Ʈ (Ʈ�������� �������� ����Ǿ����� ����Ѵ�)
void transform::updateTransform()
{
	// �ڽ��� ������ matLocal ����� �����Ѵ�.
	D3DXMatrixIdentity(&_matLocal);

	// �����Ϻ�ȭ�� ���� ������ ����
	D3DXVECTOR3 scaleRight = this->right * _scale.x;
	D3DXVECTOR3 scaleUp = this->up * _scale.y;
	D3DXVECTOR3 scaleForward = this->forward * _scale.z;

	memcpy(&_matLocal._11, &scaleRight, sizeof(D3DXVECTOR3));
	memcpy(&_matLocal._21, &scaleUp, sizeof(D3DXVECTOR3));
	memcpy(&_matLocal._31, &scaleForward, sizeof(D3DXVECTOR3));
	memcpy(&_matLocal._41, &_position, sizeof(D3DXVECTOR3));

	// ���� ���� ���
	if (_parent == NULL)
	{
		this->_matFinal = _matLocal;
	}
	// ���� �θ� �ִٸ�...
	else
	{
		this->_matFinal = _matLocal * _parent->_matFinal;
	}

	// �ڽ� ������Ʈ
	transform* pChild = this->_firstChild;

	// �ڽĵ�� ����鵵 ������Ʈ
	while (pChild != NULL)
	{
		pChild->updateTransform();
		pChild = pChild->_nextSibling;
	}
}

// ����̽��� �ڽ��� Ʈ�������� ����� ����
void transform::setDeviceWorld()
{
	device->SetTransform(D3DTS_WORLD, &_matFinal);
}
// ����̽��� �ڽ��� Ʈ�������� ��� ����
void transform::setDeviceView()
{
	/*
	�̵��̳� ȸ�� �� ��ȯ�� �ϳ��� ��ķ� ǥ���ϴµ�,
	�� ����� ������� ���ؼ� ��ȯ�� ���� ���ϸ� ����ġ�� �˴ϴ�.

	���� ���� 3d�׷��� ���̺귯���� ����÷��� �� ��ȯ�� ���� �� ��߰�����,
	�׳� � ���̳� ������ �׸���, DX�� ������ �������� �� ��ȯ ����� ���� �������ϴ�.
	�� ��ȯ�� �� �� �ϴ� ���̶� ����� �� �����ٰ�, �� ���� ����ϴ� ����� ���°���.

	����� ȭ����� ��ǥ�� 3D���� ���� ��ü�� ������ ��, ������������ �ϴµ�
	ȭ�� ũ�� ���, �������, �� ��ȯ ����� ���� ���ؼ� ���ϸ� �������� ��ǥ�� ���ɴϴ�.
	*/

	D3DXMATRIXA16 matView;
	D3DXMatrixInverse(&matView, NULL, &_matFinal);
	device->SetTransform(D3DTS_VIEW, &matView);
}

void transform::setWorldMatrix(const D3DXMATRIXA16 & matWorld)
{
	// ���� ��ķ� ����
	this->_matFinal = matWorld;
	this->_matLocal = matWorld;

	// ���� ��ġ ������Ʈ 
	_position = D3DXVECTOR3(
		_matFinal._41,
		_matFinal._42,
		_matFinal._43);

	// ���� ������Ʈ 
	D3DXVECTOR3 front = D3DXVECTOR3(
		_matFinal._31,
		_matFinal._32,
		_matFinal._33);
	D3DXVECTOR3 up = D3DXVECTOR3(
		_matFinal._21,
		_matFinal._22,
		_matFinal._23);
	D3DXVECTOR3 right = D3DXVECTOR3(
		_matFinal._11,
		_matFinal._12,
		_matFinal._13);
	D3DXVec3Normalize(&this->forward, &front);
	D3DXVec3Normalize(&this->up, &up);
	D3DXVec3Normalize(&this->right, &right);

	// ���̷� ������ �ش�.
	_scale = D3DXVECTOR3(
		D3DXVec3Length(&right),
		D3DXVec3Length(&up),
		D3DXVec3Length(&front));

	// ���� �θ� �־��ٸ�..
	if (_parent)
	{
		// ������ ���Ե� ������ �θ������ ���÷� 

		// �θ� �����
		D3DXMATRIXA16 matParentINVWorld = _parent->getFinalMatrix();
		D3DXMatrixInverse(&matParentINVWorld, NULL, &matParentINVWorld);

		// ���� ���
		this->_matLocal = _matFinal * matParentINVWorld;

		// ���� ���� ��ġ
		memcpy(&this->_position, &_matLocal._41, sizeof(D3DXVECTOR3));

		// ���� Front, Up, Right
		D3DXVECTOR3 front;
		memcpy(&front, &_matLocal._31, sizeof(D3DXVECTOR3));
		D3DXVECTOR3 up;
		memcpy(&up, &_matLocal._21, sizeof(D3DXVECTOR3));
		D3DXVECTOR3 right;
		memcpy(&right, &_matLocal._11, sizeof(D3DXVECTOR3));

		// �� ������
		float scaleX = D3DXVec3Length(&right);
		float scaleY = D3DXVec3Length(&up);
		float scaleZ = D3DXVec3Length(&front);
		this->_scale = D3DXVECTOR3(scaleX, scaleY, scaleZ);

		// ������ ���̸� ������ ����ȭ
		// front /= scaleX;
		front *= (1.0f) / scaleZ;
		up *= (1.0f) / scaleY;
		right *= (1.0f) / scaleX;

		this->forward = front;
		this->up = up;
		this->right = right;
	}

	// ���� �ڽ��� �ִٸ�....
	transform* pChild = this->_firstChild;
	while (pChild != NULL)
	{
		pChild->updateTransform();
		pChild = pChild->_nextSibling;
	}
}

D3DXMATRIXA16 transform::getWorldRotateMatrix()
{
	// �ڽ��� ������ ȸ�� ����� ����� �糤��
	D3DXMATRIXA16 matRotate;
	D3DXMatrixIdentity(&matRotate);

	// 3���� ��´�.
	D3DXVECTOR3 axis[3];
	this->getUnitAxies(axis);

	// ��Ŀ� ����
	memcpy(&matRotate._11, axis + 0, sizeof(D3DXVECTOR3));
	memcpy(&matRotate._21, axis + 1, sizeof(D3DXVECTOR3));
	memcpy(&matRotate._31, axis + 2, sizeof(D3DXVECTOR3));

	return matRotate;
}

D3DXQUATERNION transform::getWorldRotateQuaternion()
{
	D3DXQUATERNION quat;

	D3DXMATRIXA16 matRotate = this->getWorldRotateMatrix();

	// ȸ�� ��ķ� ������� �����.
	D3DXQuaternionRotationMatrix(&quat, &matRotate);

	return quat;
}

D3DXVECTOR3 transform::getWorldPosition()
{
	D3DXVECTOR3 pos = _position;

	if (_parent)
	{
		D3DXVec3TransformCoord(&pos, &pos, &_parent->_matFinal);
	}

	return pos;
}

D3DXVECTOR3 transform::getLocalPosition()
{
	return _position;
}

// ���� ��´� (�������)
void transform::getScaledAxies(D3DXVECTOR3 * pVecArr)
{
	for (int i = 0; i < 3; i++)
	{
		pVecArr[i] = this->axis[i];
	}

	// �θ� �ִٸ�..
	if (_parent)
	{
		D3DXMATRIXA16 matParentFinal = _parent->_matFinal;
		for (int i = 0; i < 3; i++)
		{
			D3DXVec3TransformNormal(&pVecArr[i], &pVecArr[i], &matParentFinal);
		}
	}
}

void transform::getUnitAxies(D3DXVECTOR3 * pVecArr)
{
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3Normalize(pVecArr + i, this->axis + i);
	}

	// �θ� �ִٸ�..
	if (_parent)
	{
		D3DXMATRIXA16 matParentFinal = _parent->_matFinal;
		for (int i = 0; i < 3; i++)
		{
			D3DXVec3TransformNormal(&pVecArr[i], &pVecArr[i], &matParentFinal);
		}
	}
}

D3DXVECTOR3 transform::getScaledAxis(int axisNum)
{
	D3DXVECTOR3 result = this->axis[axisNum];

	// �θ� �ִٸ�..
	if (_parent)
	{
		D3DXMATRIXA16 matParentFinal = _parent->_matFinal;
		D3DXVec3TransformNormal(&result, &result, &matParentFinal);
	}

	return result;
}

D3DXVECTOR3 transform::getUnitAxis(int axisNum)
{
	D3DXVECTOR3 result;
	D3DXVec3Normalize(&result, this->axis + axisNum);

	// �θ� �ִٸ�..
	if (_parent)
	{
		D3DXMATRIXA16 matParentFinal = _parent->_matFinal;
		D3DXVec3TransformNormal(&result, &result, &matParentFinal);
	}

	return result;
}

D3DXVECTOR3 transform::getForward(bool isNormalize)
{
	if (isNormalize)
	{
		return this->getUnitAxis(AXIS_Z);
	}
	return this->getScaledAxis(AXIS_Z);
}

D3DXVECTOR3 transform::getUp(bool isNormalize)
{
	if (isNormalize)
	{
		return this->getUnitAxis(AXIS_Y);
	}
	return this->getScaledAxis(AXIS_Y);
}

D3DXVECTOR3 transform::getRight(bool isNormalize)
{
	if (isNormalize)
	{
		return this->getUnitAxis(AXIS_X);
	}
	return this->getScaledAxis(AXIS_X);
}

// ī�޶� ����Ʈ ��Ʈ��
void transform::defaultControl(bool isOn)
{

	// ��Ŭ���� �Ҷ��� ����Ʈ��Ʈ���� �Ѵ�
	if (isOn)
	{
		// �Է� ���⺤��
		D3DXVECTOR3 inputVector(0, 0, 0);
		if (KEYMANAGER->isStayKeyDown(VK_UP))
		{
			inputVector.z = 4.0f;
		}
		else if (KEYMANAGER->isStayKeyDown(VK_DOWN))
		{
			inputVector.z = -4.0f;
		}
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
		{
			inputVector.x = -4.0f;
		}
		else if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		{
			inputVector.x = 4.0f;
		}
		// ���� ���Ͱ� �ƴҶ�
		if (VECTORZERO(inputVector) == false)
		{
			// ����ȭ
			D3DXVec3Normalize(&inputVector, &inputVector);
		}

		/*
		// ====================================================================
		// 			## ���Ӱ��� �ִ� �̵� ##
		// ====================================================================

		// Ÿ�ٺ���
		D3DXVECTOR3 target = inputVector * maxSpeed;
		D3DXVec3TransformNormal( &target, &target, &this->matFinal );

		// Ÿ�ٺ��Ϳ� ����Velocity ũ����
		float targetLength = D3DXVec3Length( &target ); // D3DXVec3Length ������ ũ�⸦ ��´�.
		float nowVelocityLength = D3DXVec3Length( &nowVelocity );
		float dist = abs( targetLength - nowVelocityLength );

		// Ÿ�ٰ� ���̰� �����Ѵٸ�..
		if(FLOATZERO( dist ) == false)
		{
		// ��ȭ ��뷮
		float delta = accelate * timeDelta;

		// ������
		float t = clamp01(delta / dist);

		// �̹������ӿ� Ÿ�ٱ��� ��ȭ �ϴµ� ����ϴ�
		if(FLOATEQUAL( t, 1.0f ))
		{
		nowVelocity = target;
		}
		else
		{
		nowVelocity = VecLerp( nowVelocity, target, t);
		}
		}

		// �̵� ���ʹ�
		D3DXVECTOR3 moveDeltaVector = nowVelocity * timeDelta;
		// �̵� �ض�
		this->MovePositionWorld( moveDeltaVector );
		*/

		// �̵��ϱ�
		// Ÿ�ٺ���
		D3DXVECTOR3 target = inputVector * _nowSpeed;
		this->movePositionSelf(target * _deltaTime);

		// ȸ��ó��
		// ȭ���� �߽���ġ
		int screenCenterX = WINSIZEX / 2;
		int screenCenterY = WINSIZEY / 2;

		// ���� ���콺 ��ġ
		POINT mousePos = getMousePos();

		// �̵���(�߽ɿ��� �־��� ��)
		float deltaX = mousePos.x - screenCenterX;
		float deltaY = mousePos.y - screenCenterY;
		// ���콺 ����
		if (_isSetMouse) setMousePos(WINSIZEX / 2, WINSIZEY / 2);
		// �ޱ� �߰�
		_nowAngleH += deltaX * _sensitivityH;
		_nowAngleV += deltaY * _sensitivityV;

		// �ޱ۰��� Min Max ���� ������
		_nowAngleV = clamp(_nowAngleV, _minAngleV, _maxAngleV);

		this->setRotateWorld(D3DXToRadian(_nowAngleV), D3DXToRadian(_nowAngleH), 0.0f);

		if (KEYMANAGER->isOnceKeyDown(VK_CONTROL))
		{
			this->movePositionSelf(0, -0.5f, 0);
		}
		if (KEYMANAGER->isOnceKeyUp(VK_CONTROL))
		{
			this->movePositionSelf(0, 0.5f, 0);
		}
	}
}

void transform::defaultMyControl()
{
	// �ʴ� 3��, �ʴ� 90���� ��������
	float deltaMove = 5.0f * _deltaTime;
	float deltaAngle = D3DXToRadian(90) * _deltaTime;

	if (KEYMANAGER->isStayKeyDown(VK_RBUTTON) == FALSE)
	{
		if (KEYMANAGER->isStayKeyDown('A'))
		{
			this->movePositionSelf(-deltaMove, 0.0f, 0.0f);
		}
		else if (KEYMANAGER->isStayKeyDown('D'))
		{
			this->movePositionSelf(deltaMove, 0.0f, 0.0f);
		}
		if (KEYMANAGER->isStayKeyDown('W'))
		{
			this->movePositionSelf(0.0f, 0.0f, deltaMove);
		}
		else if (KEYMANAGER->isStayKeyDown('S'))
		{
			this->movePositionSelf(0.0f, 0.0f, -deltaMove);
		}
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
		{
			this->rotateSelf(0.0f, -deltaAngle, 0.0f);
		}
		else if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		{
			this->rotateSelf(0.0f, deltaAngle, 0.0f);
		}
		if (KEYMANAGER->isStayKeyDown(VK_UP))
		{
			this->rotateSelf(-deltaAngle, 0.0f, 0.0f);
		}
		else if (KEYMANAGER->isStayKeyDown(VK_DOWN))
		{
			this->rotateSelf(deltaAngle, 0.0f, 0.0f);
		}
	}
}

// Ʈ�������� ���� ����� �׸���
void transform::renderGizmo(float scale)
{
	GIZMOMANAGER->lineMake(_position, _position + this->right * scale, 0xffff0000);
	GIZMOMANAGER->lineMake(_position, _position + this->up * scale, 0xff00ff00);
	GIZMOMANAGER->lineMake(_position, _position + this->forward * scale, 0xff0000ff);
	GIZMOMANAGER->sphereMake(_position, 1.0f);
}
