#include "stdafx.h"
#include "physicsManager.h"
#include "boundBox.h"
#include "baseObject.h"

HRESULT physicsManager::init(void)
{
	return S_OK;
}

void physicsManager::release(void)
{
}

bool physicsManager::isRayHitBound(LPRay ray, boundSphere * bound, transform * boundTrans, D3DXVECTOR3 * hitPos, D3DXVECTOR3 * hitNormal)
{
	//��ġ �� 
	D3DXMATRIXA16 matWorld = boundTrans->getFinalMatrix();

	//������ �� 
	D3DXVECTOR3 scale = boundTrans->getScale();

	//�ٿ�� Sphere �� ������ ��´�.
	D3DXVECTOR3 center;
	float radius;
	D3DXVECTOR3 halfSize;

	//��ȯ�� ��ġ
	D3DXVec3TransformCoord(&center, &bound->_localCenter, &matWorld);

	//������
	halfSize.x = bound->_halfSize.x * scale.x;
	halfSize.y = bound->_halfSize.y * scale.y;
	halfSize.z = bound->_halfSize.z * scale.z;
	radius = D3DXVec3Length(&halfSize);

	//������ ���������� ���� ���� ���ͱ����� ���⺤��
	D3DXVECTOR3 dirToCenter = center - ray->origin;

	//������ ����
	float lengthSq = D3DXVec3LengthSq(&dirToCenter);

	//�������� ����
	float r2 = radius * radius;

	//���� ������ ���ȿ� �ִٸ�..
	if (r2 > lengthSq)
	{
		//������ �ȿ��� ������ ���� üũ �ȵȴ�.
		return false;
	}

	//����������� �������� ���ۿ� �ִٴ¿���
	//�����ͱ����� ���⺤�Ϳ� ������ ���⺤�Ͱ� ������ ������ 
	//�а��̶�� �׾��ٱ���� �浹���Ͼ���
	float dot = D3DXVec3Dot(&dirToCenter, &ray->direction);
	if (dot <= 0.0f)
	{
		return false;
	}

	// ��Ÿ����� ������ �ϱ����� ���� �ﰢ�� ��������
	// d2 = x2 + y2;
	// d = sqrt( x2 + y2 );

	float x2 = dot * dot;
	float d2 = lengthSq;
	//d2 - x2 = y2;
	float y2 = d2 - x2;

	//������ ������ �����.
	if (y2 > r2)
	{
		return false;
	}
	///////   /|		
	//////   / |
	/////  a/  |
	////   /   |c
	///   /    |
	//   /__b__|

	// a*a = b*b + c*c
	//��������´ٸ� �ϴ��� ��Ʈ
	//���� ��ư��߾� HitPoint�� �ִٸ�
	if (hitPos != NULL)
	{
		//d�� raius ����
		d2 = r2;
		//d2 = y2 + x2
		//float x2 = d2 - y2;
		float x = sqrt(d2 - y2);

		//
		*hitPos = ray->origin + (ray->direction * (dot - x));

		//Hit�� ��ġ�� �븻�� ���
		if (hitNormal)
		{
			*hitNormal = *hitPos - center;
			D3DXVec3Normalize(hitNormal, hitNormal);
		}
	}

	return true;
}

bool physicsManager::isRayHitBound(LPRay ray, boundBox * bound, transform * boundTrans, D3DXVECTOR3 * hitPos, D3DXVECTOR3 * hitNormal)
{
	//���ô��� ���� �浹���ϸ� �簢���� �׾��ٱ���� �ȵȴ�
	if (this->isRayHitBound(
		ray,
		(boundSphere*)bound,
		boundTrans,
		NULL, NULL) == false)
	{
		return false;
	}

	//������� �´ٸ� �簢���� �浹�˻縦 �ؾ��Ѵ�
	//���̸� ���÷� ������
	//bound �� �����
	D3DXMATRIXA16 matWorld = boundTrans->getFinalMatrix();
	D3DXMATRIXA16 matInvMatrix;
	D3DXMatrixInverse(&matInvMatrix, NULL, &matWorld);

	//NewRayInfo
	D3DXVECTOR3 origin;
	D3DXVec3TransformCoord(&origin, &ray->origin, &matInvMatrix);
	D3DXVECTOR3 direction;
	D3DXVec3TransformNormal(&direction, &ray->direction, &matInvMatrix);

	Ray newRay;
	newRay.origin = origin;
	newRay.direction = direction;

	//���� 8 �� ����
	//   5-------6
	//  /|      /|
	// 1-------2 |
	// | 4-----|-7
	// |/      |/
	// 0-------3 

	//���� 8 ���� ������ ���Ѵ�
	D3DXVECTOR3 Vertices[8];
	Vertices[0] = D3DXVECTOR3(bound->_localMinPos.x, bound->_localMinPos.y, bound->_localMinPos.z);
	Vertices[1] = D3DXVECTOR3(bound->_localMinPos.x, bound->_localMaxPos.y, bound->_localMinPos.z);
	Vertices[2] = D3DXVECTOR3(bound->_localMaxPos.x, bound->_localMaxPos.y, bound->_localMinPos.z);
	Vertices[3] = D3DXVECTOR3(bound->_localMaxPos.x, bound->_localMinPos.y, bound->_localMinPos.z);
	Vertices[4] = D3DXVECTOR3(bound->_localMinPos.x, bound->_localMinPos.y, bound->_localMaxPos.z);
	Vertices[5] = D3DXVECTOR3(bound->_localMinPos.x, bound->_localMaxPos.y, bound->_localMaxPos.z);
	Vertices[6] = D3DXVECTOR3(bound->_localMaxPos.x, bound->_localMaxPos.y, bound->_localMaxPos.z);
	Vertices[7] = D3DXVECTOR3(bound->_localMaxPos.x, bound->_localMinPos.y, bound->_localMaxPos.z);

	D3DXVECTOR3 hit;
	D3DXVECTOR3 min = bound->_localMinPos;
	D3DXVECTOR3 max = bound->_localMaxPos;

	//�޸�
	D3DXPLANE planeBack;
	this->createPlane(&planeBack, Vertices + 0, Vertices + 1, Vertices + 2);
	//�޸��� ���̶� �浹�ߴ�?
	if (this->intersectRayToPlane(&hit, &newRay, &planeBack, false))
	{
		//��Ʈ������ �� �ȿ� �ִ�?
		if (min.x <= hit.x && hit.x <= max.x &&
			min.y <= hit.y && hit.y <= max.y)
		{
			//hit���� ����� ���ܼ� ����
			if (hitPos != NULL)
			{
				D3DXVec3TransformCoord(hitPos, &hit, &matWorld);
			}

			if (hitNormal != NULL)
			{
				*hitNormal = -boundTrans->getForward();
			}

			return true;
		}
	}

	//�ո�
	D3DXPLANE planeFront;
	this->createPlane(&planeFront, Vertices + 6, Vertices + 5, Vertices + 4);
	//�ո��� ���̶� �浹�ߴ�?
	if (this->intersectRayToPlane(&hit, &newRay, &planeFront, false))
	{
		//��Ʈ������ �� �ȿ� �ִ�?
		if (min.x <= hit.x && hit.x <= max.x &&
			min.y <= hit.y && hit.y <= max.y)
		{

			//hit���� ����� ���ܼ� ����
			if (hitPos != NULL)
			{
				D3DXVec3TransformCoord(hitPos, &hit, &matWorld);
			}

			if (hitNormal != NULL)
			{
				*hitNormal = boundTrans->getForward();
			}

			return true;
		}
	}

	//�ϸ�
	D3DXPLANE planeBottom;
	this->createPlane(&planeBottom, Vertices + 3, Vertices + 4, Vertices + 0);
	//�ϸ��� ���̶� �浹�ߴ�?
	if (this->intersectRayToPlane(&hit, &newRay, &planeBottom, false))
	{
		//��Ʈ������ �� �ȿ� �ִ�?
		if (min.x <= hit.x && hit.x <= max.x &&
			min.z <= hit.z && hit.z <= max.z)
		{
			//hit���� ����� ���ܼ� ����
			if (hitPos != NULL)
			{
				D3DXVec3TransformCoord(hitPos, &hit, &matWorld);
			}

			if (hitNormal != NULL)
			{
				*hitNormal = -boundTrans->getUp();
			}

			return true;
		}
	}

	//���
	D3DXPLANE planeTop;
	this->createPlane(&planeTop, Vertices + 1, Vertices + 5, Vertices + 2);
	//����� ���̶� �浹�ߴ�?
	if (this->intersectRayToPlane(&hit, &newRay, &planeTop, false))
	{
		//��Ʈ������ �� �ȿ� �ִ�?
		if (min.x <= hit.x && hit.x <= max.x &&
			min.z <= hit.z && hit.z <= max.z)
		{
			//hit���� ����� ���ܼ� ����
			if (hitPos != NULL)
			{
				D3DXVec3TransformCoord(hitPos, &hit, &matWorld);
			}

			if (hitNormal != NULL)
			{
				*hitNormal = boundTrans->getUp();
			}

			return true;
		}
	}

	//�¸�
	D3DXPLANE planeLeft;
	this->createPlane(&planeLeft, Vertices + 0, Vertices + 5, Vertices + 1);
	//�¸��� ���̶� �浹�ߴ�?
	if (this->intersectRayToPlane(&hit, &newRay, &planeLeft, false))
	{
		//��Ʈ������ �� �ȿ� �ִ�?
		if (min.y <= hit.y && hit.y <= max.y &&
			min.z <= hit.z && hit.z <= max.z)
		{
			//hit���� ����� ���ܼ� ����
			if (hitPos != NULL)
			{
				D3DXVec3TransformCoord(hitPos, &hit, &matWorld);
			}

			if (hitNormal != NULL)
			{
				*hitNormal = -boundTrans->getRight();
			}

			return true;
		}
	}

	//���
	D3DXPLANE planeRight;
	this->createPlane(&planeRight, Vertices + 2, Vertices + 6, Vertices + 3);
	//����� ���̶� �浹�ߴ�?
	if (this->intersectRayToPlane(&hit, &newRay, &planeRight, false))
	{
		//��Ʈ������ �� �ȿ� �ִ�?
		if (min.y <= hit.y && hit.y <= max.y &&
			min.z <= hit.z && hit.z <= max.z)
		{
			//hit���� ����� ���ܼ� ����
			if (hitPos != NULL)
			{
				D3DXVec3TransformCoord(hitPos, &hit, &matWorld);
			}

			if (hitNormal != NULL)
			{
				*hitNormal = boundTrans->getRight();
			}

			return true;
		}
	}

	//��������´ٸ� �浹����
	return false;
}

bool physicsManager::isRayHitStaticMeshObject(LPRay ray, baseObject * object, D3DXVECTOR3 * hitPos, D3DXVECTOR3 * hitNormal)
{
	//���̸� ���÷� ������

	//bound �� �����
	D3DXMATRIXA16 matWorld = object->_transform->getFinalMatrix();
	D3DXMATRIXA16 matInvMatrix;
	D3DXMatrixInverse(&matInvMatrix, NULL, &matWorld);

	//NewRayInfo
	D3DXVECTOR3 origin;
	D3DXVec3TransformCoord(&origin, &ray->origin, &matInvMatrix);
	D3DXVECTOR3 direction;
	D3DXVec3TransformNormal(&direction, &ray->direction, &matInvMatrix);

	Ray newRay;
	newRay.origin = origin;
	newRay.direction = direction;

	//�޽� �浹 �����Լ�
	xMeshStatic* pStaticMesh = dynamic_cast<xMeshStatic*>(object->_pMesh);
	if (pStaticMesh != NULL)
	{
		//����´�� ����
		for (int i = 0; i < pStaticMesh->_materialsNum; i++)
		{
			BOOL bHit = false;
			DWORD faceIndex = 0;
			float dist = 0.0f;
			DWORD hitCount = 0;

			D3DXIntersectSubset(
				pStaticMesh->_mesh,			// xMesh
				i,							// ����³�
				&origin,					// ������ġ
				&direction,					// ���� ����
				&bHit,						// �浹 ���� ���
				&faceIndex,					// �浹�� �ε��� 
				NULL,
				NULL,
				&dist,						// �浹�Ÿ�
				NULL,						// �����浹�� ������Ʈ���� ���� ����, 
				&hitCount);					// �����浹�� ��Ʈ ���� 

			if (bHit)
			{
				// hit���� ����� ���ܼ� ����
				if (hitPos != NULL)
				{
					D3DXVec3TransformCoord(hitPos, &(origin + direction * dist), &matWorld);
				}

				// Hit �븻 ���Ѵ�.
				if (hitNormal != NULL)
				{
					// �浹�� �ε����� ���� ���� 3���� ���Ѵ�.
					DWORD i0 = (faceIndex * 3);
					DWORD i1 = (faceIndex * 3 + 1);
					DWORD i2 = (faceIndex * 3 + 2);

					// �����ε����� ��´�.
					D3DXVECTOR3 v0 = pStaticMesh->_vertices[pStaticMesh->_indices[i0]];
					D3DXVECTOR3 v1 = pStaticMesh->_vertices[pStaticMesh->_indices[i1]];
					D3DXVECTOR3 v2 = pStaticMesh->_vertices[pStaticMesh->_indices[i2]];

					// ���� 2���� �븻�� ���Ѵ�.
					D3DXVECTOR3 edge1 = v1 - v0;
					D3DXVECTOR3 edge2 = v2 - v0;

					D3DXVec3Cross(hitNormal, &edge1, &edge2);
					D3DXVec3Normalize(hitNormal, hitNormal);

					D3DXVec3TransformNormal(hitNormal, hitNormal, &matWorld);
				}
				return true;
			}
		}
	}
	return false;
}

// ���̰� ���� �浹�ߴ��� Ȯ��
bool physicsManager::isRayHitSphere(LPRay ray, D3DXVECTOR3 * pCenter, float radius, D3DXVECTOR3 * hitPos, D3DXVECTOR3 * hitNormal)
{
	// �ٿ�� Sphere �� ������ ��´�.
	D3DXVECTOR3 center = *pCenter;

	// ������ ���������� ���� ���� ���ͱ����� ���⺤��
	D3DXVECTOR3 dirToCenter = center - ray->origin;

	// ������ ����
	float lengthSq = D3DXVec3LengthSq(&dirToCenter);

	// �������� ����
	float r2 = radius * radius;

	float dot = D3DXVec3Dot(&dirToCenter, &ray->direction);

	// ��Ÿ����� ������ �ϱ����� ���� �ﰢ�� ��������
	// d2 = x2 + y2;
	// d = sqrt( x2 + y2 );

	float x2 = dot * dot;
	float d2 = lengthSq;
	//d2 - x2 = y2;
	float y2 = d2 - x2;

	// ������ ������ �����.
	if (y2 > r2)
	{
		return false;
	}

	// ��������´ٸ� �ϴ��� ��Ʈ
	// ���� ��ư��߾� HitPoint ���ִٸ�
	if (hitPos != NULL)
	{
		// d�� raius ����
		d2 = r2;
		// d2 = y2 + x2
		// float x2 = d2 - y2;
		float x = sqrt(d2 - y2);

		*hitPos = ray->origin + (ray->direction * (dot - x));

		// Hit �� ��ġ�� �븻�� ��ڴٸ�..
		if (hitNormal)
		{
			*hitNormal = *hitPos - center;
			D3DXVec3Normalize(hitNormal, hitNormal);
		}
	}

	return true;
}

bool physicsManager::isOverlap(baseObject * objA, baseObject * objB)
{
	return this->isOverlap(
		objA->_transform, &objA->_boundBox,
		objB->_transform, &objB->_boundBox);
}

bool physicsManager::isOverlap(transform * transA, boundSphere * boundA, transform * transB, boundSphere * boundB)
{
	// ���� �������� ��´�.
	D3DXVECTOR3 centerA;
	float radiusA;
	D3DXVECTOR3 centerB;
	float radiusB;

	boundA->getWorldCenterRadius(transA, &centerA, &radiusA);
	boundB->getWorldCenterRadius(transB, &centerB, &radiusB);

	D3DXVECTOR3 dirTo = centerB - centerA;

	// �Ÿ��� ��������´� ( sqrt ���� �����Ѵ� )
	float distPow2 = D3DXVec3LengthSq(&dirTo);

	// ������ ������
	float sumRadiusPow2 = radiusA + radiusB;
	sumRadiusPow2 *= sumRadiusPow2;

	if (distPow2 > sumRadiusPow2)
	{
		return false;
	}

	return true;
}

bool physicsManager::isOverlap(transform * transA, boundBox * boundA, transform * transB, boundBox * boundB)
{
	// ���� �������� ��´�.
	D3DXVECTOR3 centerA;
	float radiusA;
	D3DXVECTOR3 centerB;
	float radiusB;

	boundA->getWorldCenterRadius(transA, &centerA, &radiusA);
	boundB->getWorldCenterRadius(transB, &centerB, &radiusB);

	D3DXVECTOR3 dirTo = centerB - centerA;

	// �Ÿ��� ��������´� ( sqrt ���� �����Ѵ� )
	float distPow2 = D3DXVec3LengthSq(&dirTo);

	// ������ ������
	float sumRadiusPow2 = radiusA + radiusB;
	sumRadiusPow2 *= sumRadiusPow2;

	// ���� ������ �浹 �������� �׾��ٱ���� ���ڳ����� �浹���� �ʴ´�
	if (distPow2 > sumRadiusPow2)
	{
		return false;
	}
	// ������� �´ٸ� ������������ �浹�ߴٴ� ����

	// �䳪���� OBB �ڽ� �浹üũ

	// �迭����
	//X = 0, Y = 1, Z = 2;
	// OBB �浹�� �ʿ��� �浹 ����ü
	struct OBB
	{
		D3DXVECTOR3 center;			// �߽���
		D3DXVECTOR3 axis[3];		// �����	
		float halfLength[3];		// ���࿡ ���� ���� ũ��
	};

	//
	// A �ٿ���� ���� �浹 ����ü
	//
	OBB obbA;

	// �� �� ����
	obbA.axis[0] = transA->getRight();
	obbA.axis[1] = transA->getUp();
	obbA.axis[2] = transA->getForward();

	// ����
	obbA.center = centerA;

	// ����������
	D3DXVECTOR3 scaleA = transA->getScale();
	obbA.halfLength[0] = boundA->_halfSize.x * scaleA.x;
	obbA.halfLength[1] = boundA->_halfSize.y * scaleA.y;
	obbA.halfLength[2] = boundA->_halfSize.z * scaleA.z;

	//
	// B �ٿ���� ���� �浹 ����ü
	//
	OBB obbB;

	//�� �� ����
	obbB.axis[0] = transB->getRight();
	obbB.axis[1] = transB->getUp();
	obbB.axis[2] = transB->getForward();

	// ����
	obbB.center = centerB;

	// ����������
	D3DXVECTOR3 scaleB = transB->getScale();
	obbB.halfLength[0] = boundB->_halfSize.x * scaleB.x;
	obbB.halfLength[1] = boundB->_halfSize.y * scaleB.y;
	obbB.halfLength[2] = boundB->_halfSize.z * scaleB.z;

	//
	// OBB �浹
	//
	float cos[3][3];	// ���� �������� ���� �ڻ��� ��  [A��][B��]  ([0][1] => ���ε����� A�� X ��� B�� Y ���� ������ ���� cos ���̴�)
	float absCos[3][3];	// ���� �������� ���� �ڻ��� ���밪  [A��][B��]  ([0][1] => ���ε����� A�� X ��� B�� Y ���� ������ ���� cos ���� ���̴�)
	float dist[3];		// A �ٿ�� �������� A �߽������� B �� �߽������͸� ������ ��������			

	const float cutOff = 0.99999f;		// ���� �Ǵ��� �ϱ����� �ƿ��� �� (��� �� ���� cos ��� ���� �̺��� ũ�ٸ� �� �浹ü�� ������ �����ϴٴ� ����)
	bool existParallelPair = false;		// �� ���̶� �����ϳ�?

	//A ���� B �� ���⺤��
	D3DXVECTOR3 D = obbB.center - obbA.center;

	float r, r0, r1;	 // r0 �� r1 �� ���� r ���� ������ �浹 ���� 

	for (int a = 0; a < 3; a++)
	{
		for (int b = 0; b < 3; b++)
		{
			cos[a][b] = D3DXVec3Dot(&obbA.axis[a], &obbB.axis[b]);
			absCos[a][b] = abs(cos[a][b]);

			// ������ ���� ���� �Ǵ� ��Ȯ��
			if (absCos[a][b] > cutOff) existParallelPair = true;
		}

		// ���ͳ����� ���⺤�͸� A �ٿ�� Axis �� ������ �Ÿ�
		dist[a] = D3DXVec3Dot(&obbA.axis[a], &D);
	}

	//
	// A �ٿ�� �ڽ��� X ���� �������� �� ����
	//

	// r �� dist[0] �� ���� ���� �ȴ�.
	r = abs(dist[0]);

	// r0 
	r0 = obbA.halfLength[0];

	// r1
	/*
	r1 = abs( D3DXVec3Dot( &obbA.axis[0], &( obbB.axis[0] * obbB.halfLength[0] ) ) ) +
	abs( D3DXVec3Dot( &obbA.axis[0], &( obbB.axis[1] * obbB.halfLength[1] ) ) ) +
	abs( D3DXVec3Dot( &obbA.axis[0], &( obbB.axis[2] * obbB.halfLength[2] ) ) );
	*/
	/*
	r1 = abs( D3DXVec3Dot( &obbA.axis[0], &obbB.axis[0] ) ) * obbB.halfLength[0] +
	abs( D3DXVec3Dot( &obbA.axis[0], &obbB.axis[1] ) ) * obbB.halfLength[1] +
	abs( D3DXVec3Dot( &obbA.axis[0], &obbB.axis[2] ) ) * obbB.halfLength[2];
	*/
	r1 = absCos[0][0] * obbB.halfLength[0] +
		absCos[0][1] * obbB.halfLength[1] +
		absCos[0][2] * obbB.halfLength[2];

	if (r > r0 + r1) return false;

	//
	// A �ٿ�� �ڽ��� Y ���� �������� �� ����
	//

	// r �� dist[1] �� ���� ���� �ȴ�.
	r = abs(dist[1]);

	// r0 
	r0 = obbA.halfLength[1];

	// r1 
	r1 = absCos[1][0] * obbB.halfLength[0] +
		absCos[1][1] * obbB.halfLength[1] +
		absCos[1][2] * obbB.halfLength[2];

	if (r > r0 + r1) return false;

	//
	// A �ٿ�� �ڽ��� Z ���� �������� �� ����
	//

	// r �� dist[2] �� ���� ���� �ȴ�.
	r = abs(dist[2]);

	// r0 
	r0 = obbA.halfLength[2];

	// r1 
	r1 = absCos[2][0] * obbB.halfLength[0] +
		absCos[2][1] * obbB.halfLength[1] +
		absCos[2][2] * obbB.halfLength[2];

	if (r > r0 + r1) return false;

	//
	// B �ٿ�� �ڽ��� X ���� �������� �� ����
	//
	r = abs(D3DXVec3Dot(&obbB.axis[0], &D));

	// r0 
	r0 = absCos[0][0] * obbA.halfLength[0] +
		absCos[1][0] * obbA.halfLength[1] +
		absCos[2][0] * obbA.halfLength[2];

	// r1 
	r1 = obbB.halfLength[0];

	if (r > r0 + r1) return false;

	//
	// B �ٿ�� �ڽ��� Y ���� �������� �� ����
	//
	r = abs(D3DXVec3Dot(&obbB.axis[1], &D));

	// r0 
	r0 = absCos[0][1] * obbA.halfLength[0] +
		absCos[1][1] * obbA.halfLength[1] +
		absCos[2][1] * obbA.halfLength[2];

	// r1 
	r1 = obbB.halfLength[1];
	if (r > r0 + r1) return false;

	//
	// B �ٿ�� �ڽ��� Z ���� �������� �� ����
	//
	r = abs(D3DXVec3Dot(&obbB.axis[2], &D));

	// r0 
	r0 = absCos[0][2] * obbA.halfLength[0] +
		absCos[1][2] * obbA.halfLength[1] +
		absCos[2][2] * obbA.halfLength[2];

	// r1 
	r1 = obbB.halfLength[2];
	if (r > r0 + r1) return false;


	// �����Դµ� ���е��� �ʾҴ�.. �׷��� existParallelPair true �̸�
	// ������ �����ϴٴ� �����ε� �̷��� �и��� 6 ���� �˻��ϸ�ȴ�....
	if (existParallelPair) return true;

	//////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////

	// A�� X �� B �� X �࿡ ���� ������ �浹 üũ
	r = abs(dist[2] * cos[1][0] - dist[1] * cos[2][0]);
	r0 = obbA.halfLength[1] * absCos[2][0] + obbA.halfLength[2] * absCos[1][0];
	r1 = obbB.halfLength[1] * absCos[0][2] + obbB.halfLength[2] * absCos[0][1];
	if (r > r0 + r1) return false;

	// A�� X �� B �� Y �࿡ ���� ������ �浹 üũ
	r = abs(dist[2] * cos[1][1] - dist[1] * cos[2][1]);
	r0 = obbA.halfLength[1] * absCos[2][1] + obbA.halfLength[2] * absCos[1][1];
	r1 = obbB.halfLength[0] * absCos[0][2] + obbB.halfLength[2] * absCos[0][0];
	if (r > r0 + r1) return false;

	// A�� X �� B �� Z �࿡ ���� ������ �浹 üũ
	r = abs(dist[2] * cos[1][2] - dist[1] * cos[2][2]);
	r0 = obbA.halfLength[1] * absCos[2][2] + obbA.halfLength[2] * absCos[1][2];
	r1 = obbB.halfLength[0] * absCos[0][1] + obbB.halfLength[1] * absCos[0][0];
	if (r > r0 + r1) return false;

	/////////////////////////////////////////////////////////////////

	// A�� Y �� B �� X �࿡ ���� ������ �浹 üũ
	r = abs(dist[0] * cos[2][0] - dist[2] * cos[0][0]);
	r0 = obbA.halfLength[0] * absCos[2][0] + obbA.halfLength[2] * absCos[0][0];
	r1 = obbB.halfLength[1] * absCos[1][2] + obbB.halfLength[2] * absCos[1][1];
	if (r > r0 + r1) return false;

	// A�� Y �� B �� Y �࿡ ���� ������ �浹 üũ
	r = abs(dist[0] * cos[2][1] - dist[2] * cos[0][1]);
	r0 = obbA.halfLength[0] * absCos[2][1] + obbA.halfLength[2] * absCos[0][1];
	r1 = obbB.halfLength[0] * absCos[1][2] + obbB.halfLength[2] * absCos[1][0];
	if (r > r0 + r1) return false;

	// A�� Y �� B �� Z �࿡ ���� ������ �浹 üũ
	r = abs(dist[0] * cos[2][2] - dist[2] * cos[0][2]);
	r0 = obbA.halfLength[0] * absCos[2][2] + obbA.halfLength[2] * absCos[0][2];
	r1 = obbB.halfLength[0] * absCos[1][1] + obbB.halfLength[1] * absCos[1][0];
	if (r > r0 + r1) return false;

	/////////////////////////////////////////////////////////////////	 

	// A�� Z �� B �� X �࿡ ���� ������ �浹 üũ
	r = abs(dist[1] * cos[0][0] - dist[0] * cos[1][0]);
	r0 = obbA.halfLength[0] * absCos[1][0] + obbA.halfLength[1] * absCos[0][0];
	r1 = obbB.halfLength[1] * absCos[2][2] + obbB.halfLength[2] * absCos[2][1];
	if (r > r0 + r1) return false;

	// A�� Z �� B �� Y �࿡ ���� ������ �浹 üũ
	r = abs(dist[1] * cos[0][1] - dist[0] * cos[1][1]);
	r0 = obbA.halfLength[0] * absCos[1][1] + obbA.halfLength[1] * absCos[0][1];
	r1 = obbB.halfLength[0] * absCos[2][2] + obbB.halfLength[2] * absCos[2][0];
	if (r > r0 + r1) return false;

	// A�� Z �� B �� Z �࿡ ���� ������ �浹 üũ
	r = abs(dist[1] * cos[0][2] - dist[0] * cos[1][2]);
	r0 = obbA.halfLength[0] * absCos[1][2] + obbA.halfLength[1] * absCos[0][2];
	r1 = obbB.halfLength[0] * absCos[2][1] + obbB.halfLength[1] * absCos[2][0];
	if (r > r0 + r1) return false;

	// ������� Ŭ���� �ߴٸ� �浹����
	return true;
}

bool physicsManager::isOverlap(transform * transA, boundSphere * boundA, transform * transB, boundBox * boundB)
{
	return true;
}

bool physicsManager::isOverlap(transform * transA, boundBox * boundA, transform * transB, boundSphere * boundB)
{
	return true;
}

bool physicsManager::isBlocking(baseObject * objA, baseObject * objB, float moveFactor)
{
	return this->isBlocking(
		objA->_transform, &objA->_boundBox,
		objB->_transform, &objB->_boundBox, moveFactor);
}

// 2���� �ٿ�� ���Ǿ���� �⵿�ϰ� ��ġ�� �ʰ� �Ѵ�.
// moveFactor �� 0 �ϼ��� ��Ĩ�� ���� B �� �����δ�.
// moveFactor �� 0.5 �̸� ��Ĩ�� ���� A �� B �� �Ȱ��������� �����δ�.
// moveFactor �� 1 �ϼ��� ��Ĩ�� ���� A �� �����δ�.
bool physicsManager::isBlocking(transform * transA, boundSphere * boundA, transform * transB, boundSphere * boundB, float moveFactor)
{
	return true;
}

bool physicsManager::isBlocking(transform * transA, boundBox * boundA, transform * transB, boundBox * boundB, float moveFactor)
{
	// ���� �浹���� �ʾ����� ���ʿ����
	if (isOverlap(transA, boundA, transB, boundB) == false) return false;

	moveFactor = clamp01(moveFactor);

	// ���� �ε��ƽ��� �о���Ѵ�

	// A�� Min Max
	D3DXVECTOR3 minA = boundA->_localMinPos;
	D3DXVECTOR3 maxA = boundA->_localMaxPos;

	// B�� Min Max
	D3DXVECTOR3 minB = boundB->_localMinPos;
	D3DXVECTOR3 maxB = boundB->_localMaxPos;

	// B �� ���� �簢 8 ��

	//       5-------6 Max
	//      /|      /|
	//     1-------2 |
	//     | 4-----|-7
	//     |/      |/
	// Min 0-------3

	D3DXVECTOR3 pos[8];
	pos[0] = D3DXVECTOR3(minB.x, minB.y, minB.z);
	pos[1] = D3DXVECTOR3(minB.x, maxB.y, minB.z);
	pos[2] = D3DXVECTOR3(maxB.x, maxB.y, minB.z);
	pos[3] = D3DXVECTOR3(maxB.x, minB.y, minB.z);
	pos[4] = D3DXVECTOR3(minB.x, minB.y, maxB.z);
	pos[5] = D3DXVECTOR3(minB.x, maxB.y, maxB.z);
	pos[6] = D3DXVECTOR3(maxB.x, maxB.y, maxB.z);
	pos[7] = D3DXVECTOR3(maxB.x, minB.y, maxB.z);


	// A �� ���� �����
	D3DXMATRIXA16 matWorldAInv;
	D3DXMatrixInverse(&matWorldAInv, NULL, &transA->getFinalMatrix());

	// B �� ���� ���
	D3DXMATRIXA16 matWorldB = transB->getFinalMatrix();

	// B ���� ��ŭ ���� A �� ������ �ٽ� ������ ���
	D3DXMATRIXA16 mat = matWorldB * matWorldAInv;

	// B pos �� ����
	for (int i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&pos[i], &pos[i], &mat);

	// �� ���� Pos ���� A ���� B �� ������� ��ġ�����̵ȴ�.

	// ���������� ����� B pos �� ������ min max �� ���� ����
	minB = pos[0];
	maxB = pos[0];
	for (int i = 1; i < 8; i++) {
		if (pos[i].x < minB.x) minB.x = pos[i].x;
		if (pos[i].y < minB.y) minB.y = pos[i].y;
		if (pos[i].z < minB.z) minB.z = pos[i].z;
		if (pos[i].x > maxB.x) maxB.x = pos[i].x;
		if (pos[i].y > maxB.y) maxB.y = pos[i].y;
		if (pos[i].z > maxB.z) maxB.z = pos[i].z;
	}

	// �簢 �� ����ü
	struct fRect
	{
		float left;
		float right;
		float bottom;
		float top;
		float back;
		float front;
	};

	fRect rcA = { minA.x, maxA.x, minA.y, maxA.y, minA.z, maxA.z };
	fRect rcB = { minB.x, maxB.x, minB.y, maxB.y, minB.z, maxB.z };

	//��Ĩ���� �簢��
	fRect rcInter;
	rcInter.left = max(rcA.left, rcB.left);
	rcInter.right = min(rcA.right, rcB.right);
	rcInter.bottom = max(rcA.bottom, rcB.bottom);
	rcInter.top = min(rcA.top, rcB.top);
	rcInter.back = max(rcA.back, rcB.back);
	rcInter.front = min(rcA.front, rcB.front);


	// ������ ��Ĩ���� ���ϰ� ���� ���� ���� ������ B �̵���Ų��.
	float interX = rcInter.right - rcInter.left;
	float interY = rcInter.top - rcInter.bottom;
	float interZ = rcInter.front - rcInter.back;
	float minInter = (interX < interY) ? ((interX < interZ) ? interX : interZ) : ((interY < interZ) ? interY : interZ);

	// �̴� ����
	D3DXVECTOR3 moveDirA(0, 0, 0);

	// �з�
	float moveLengthA = minInter;

	// X ���� ��Ĩ���� ���� �۴ٸ�..
	if (minInter == interX)
	{
		// A �� �������� �о�� �Ѵٸ�....
		if (FLOATEQUAL(rcInter.left, rcA.left))
			moveDirA = -transA->getRight();

		// A �� ���������� �о�� �Ѵٸ�....
		else if (FLOATEQUAL(rcInter.right, rcA.right))
			moveDirA = transA->getRight();
	}
	// Y ���� ��Ĩ���� ���� �۴ٸ�..
	else if (minInter == interY)
	{
		// A �� ������ �о�� �Ѵٸ�....
		if (FLOATEQUAL(rcInter.top, rcA.top))
			moveDirA = transA->getUp();

		// A �� �Ʒ����� �о�� �Ѵٸ�....
		else if (FLOATEQUAL(rcInter.bottom, rcA.bottom))
			moveDirA = -transA->getUp();
	}

	// Z ���� ��ħ���� ���� �۴ٸ�..
	else if (minInter == interZ)
	{
		// A �� �������� �о�� �Ѵٸ�....
		if (FLOATEQUAL(rcInter.front, rcA.front))
			moveDirA = transA->getForward();

		// A �� ������ �о�� �Ѵٸ�....
		else if (FLOATEQUAL(rcInter.back, rcA.back))
			moveDirA = -transA->getForward();
	}

	// ��������´ٸ� �з��� �̴� ������ ���ȴ�.
	// moveDirA
	// moveLengthA
	// ���� �ΰ��� A �� ������ �ΰ� B �� A ������ ����� ���̵ȴ�.

	//
	// �Ʒ��� ������ B �� ������ �ΰ� A �� B ������ ����� ���̵ȴ�.
	//
	D3DXVECTOR3 moveDirB(0, 0, 0);
	float moveLengthB = 0.0f;

	// A�� Min Max
	minA = boundA->_localMinPos;
	maxA = boundA->_localMaxPos;

	// B�� Min Max
	minB = boundB->_localMinPos;
	maxB = boundB->_localMaxPos;

	// B �� ���� �簢 8 ��
	//       5-------6 Max
	//      /|      /|
	//     1-------2 |
	//     | 4-----|-7
	//     |/      |/
	// Min 0-------3

	pos[0] = D3DXVECTOR3(minA.x, minA.y, minA.z);
	pos[1] = D3DXVECTOR3(minA.x, maxA.y, minA.z);
	pos[2] = D3DXVECTOR3(maxA.x, maxA.y, minA.z);
	pos[3] = D3DXVECTOR3(maxA.x, minA.y, minA.z);
	pos[4] = D3DXVECTOR3(minA.x, minA.y, maxA.z);
	pos[5] = D3DXVECTOR3(minA.x, maxA.y, maxA.z);
	pos[6] = D3DXVECTOR3(maxA.x, maxA.y, maxA.z);
	pos[7] = D3DXVECTOR3(maxA.x, minA.y, maxA.z);

	// B �� ���� �����
	D3DXMATRIXA16 matWorldBInv;
	D3DXMatrixInverse(&matWorldBInv, NULL, &transB->getFinalMatrix());

	// A �� ���� ���
	D3DXMATRIXA16 matWorldA = transA->getFinalMatrix();

	// A ���� ��ŭ ���� B �� ������ �ٽ� ������ ���
	mat = matWorldA * matWorldBInv;

	// A pos �� ����
	for (int i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&pos[i], &pos[i], &mat);

	// ���������� ����� A pos �� ������ min max �� ���� ����
	minA = pos[0];
	maxA = pos[0];
	for (int i = 1; i < 8; i++) {
		if (pos[i].x < minA.x) minA.x = pos[i].x;
		if (pos[i].y < minA.y) minA.y = pos[i].y;
		if (pos[i].z < minA.z) minA.z = pos[i].z;
		if (pos[i].x > maxA.x) maxA.x = pos[i].x;
		if (pos[i].y > maxA.y) maxA.y = pos[i].y;
		if (pos[i].z > maxA.z) maxA.z = pos[i].z;
	}
	// rcA = { minA.x, maxA.x, minA.y, maxA.y, minA.z, maxA.z };
	// rcB = { minB.x, maxB.x, minB.y, maxB.y, minB.z, maxB.z };
	rcA.left = minA.x;		rcA.right = maxA.x;			rcA.bottom = minA.y;			rcA.top = maxA.y;			rcA.back = minA.z;			rcA.front = maxA.z;
	rcB.left = minB.x;		rcB.right = maxB.x;			rcB.bottom = minB.y;			rcB.top = maxB.y;			rcB.back = minB.z;			rcB.front = maxB.z;

	// ��Ĩ���� �簢��
	rcInter.left = max(rcA.left, rcB.left);
	rcInter.right = min(rcA.right, rcB.right);
	rcInter.bottom = max(rcA.bottom, rcB.bottom);
	rcInter.top = min(rcA.top, rcB.top);
	rcInter.back = max(rcA.back, rcB.back);
	rcInter.front = min(rcA.front, rcB.front);

	// ������ ��Ĩ���� ���ϰ� ���� ���� ���� ������ B �̵���Ų��.
	interX = rcInter.right - rcInter.left;
	interY = rcInter.top - rcInter.bottom;
	interZ = rcInter.front - rcInter.back;
	minInter = (interX < interY) ? ((interX < interZ) ? interX : interZ) : ((interY < interZ) ? interY : interZ);

	moveLengthB = minInter;

	// X ���� ��Ĩ���� ���� �۴ٸ�..
	if (minInter == interX)
	{
		// B �� �������� �о�� �Ѵٸ�....
		if (FLOATEQUAL(rcInter.left, rcB.left))
			moveDirB = -transB->getRight();

		// B �� ���������� �о�� �Ѵٸ�....
		else if (FLOATEQUAL(rcInter.right, rcB.right))
			moveDirB = transB->getRight();
	}
	// Y ���� ��Ĩ���� ���� �۴ٸ�..
	else if (minInter == interY)
	{
		// B �� ������ �о�� �Ѵٸ�....
		if (FLOATEQUAL(rcInter.top, rcB.top))
			moveDirB = transB->getUp();

		// B �� �Ʒ����� �о�� �Ѵٸ�....
		else if (FLOATEQUAL(rcInter.bottom, rcB.bottom))
			moveDirB = -transB->getUp();

	}

	// Z ���� ��ħ���� ���� �۴ٸ�..
	else if (minInter == interZ)
	{
		// B �� �������� �о�� �Ѵٸ�....
		if (FLOATEQUAL(rcInter.front, rcB.front))
			moveDirB = transB->getForward();

		// B �� ������ �о�� �Ѵٸ�....
		else if (FLOATEQUAL(rcInter.back, rcB.back))
			moveDirB = -transB->getForward();
	}

	// ��������´ٸ� �Ʒ��� 4 ���� ������ ���� ���̴�....
	// D3DXVECTOR3 moveDirA( 0, 0, 0 );
	// float moveLengthA = minInter;
	// D3DXVECTOR3 moveDirB( 0, 0, 0 );
	// float moveLengthB = 0.0f;

	// �з��� ����������...
	if (moveLengthB > moveLengthA)
	{
		// A �� B �� ������ ����
		// ���� A �� ������ �ΰ� B �� �ǵ��ΰŴ�
		// ���� A �� �������� 2 ���� B �� ������ �� 1 �̿��ٰ� �����Ͽ�����
		// A �� ����ķ� ���Ǿ� A �� �������� 1 B �� �������� 0.5 �� ���� �����̴�
		// �׷��Ƿ� �� ���� ���̴�
		// Ascale ���� ���Ͽ� ����ϴ� ���� �ùٸ���.
		D3DXVECTOR3 scaleA = transA->getScale();
		D3DXVECTOR3 scaleB = transB->getScale();

		transB->movePositionWorld(
			moveDirA.x * moveLengthA * (1.0 - moveFactor) * scaleA.x,
			moveDirA.y * moveLengthA * (1.0 - moveFactor) * scaleA.y,
			moveDirA.z * moveLengthA * (1.0 - moveFactor) * scaleA.z);

		transA->movePositionWorld(
			-moveDirA.x * moveLengthA * moveFactor * scaleA.x,
			-moveDirA.y * moveLengthA * moveFactor * scaleA.y,
			-moveDirA.z * moveLengthA * moveFactor * scaleA.z);

	}
	else
	{
		// A �� B �� ������ ����
		// ���� B �� ������ �ΰ� A �� �ǵ��ΰŴ� /
		// ���� A �� �������� 2 ���� B �� ������ �� 1 �̿��ٰ� �����Ͽ�����
		// B �� ����ķ� ���Ǿ� A �� �������� 2 B �� �������� 1 �� ���� �����̴�
		// �׷��Ƿ� �� ���� ���̴� BScale ���� ���Ͽ� ����ϴ� ���� �ùٸ���.;

		D3DXVECTOR3 scaleA = transA->getScale();
		D3DXVECTOR3 scaleB = transB->getScale();

		transA->movePositionWorld(
			moveDirB.x * moveLengthB * moveFactor  * scaleB.x,
			moveDirB.y * moveLengthB * moveFactor  * scaleB.y,
			moveDirB.z * moveLengthB * moveFactor  * scaleB.z);

		transB->movePositionWorld(
			-moveDirB.x * moveLengthB * (1.0 - moveFactor) *scaleB.x,
			-moveDirB.y * moveLengthB * (1.0 - moveFactor) *scaleB.y,
			-moveDirB.z * moveLengthB * (1.0 - moveFactor) *scaleB.z);
	}

	return true;
}

void physicsManager::createPlane(LPD3DXPLANE pOutPlane, D3DXVECTOR3 * p0, D3DXVECTOR3 * p1, D3DXVECTOR3 * p2)
{
	// ����� �븻 
	D3DXVECTOR3 normal;
	D3DXVECTOR3 edge1 = *p1 - *p0;
	D3DXVECTOR3 edge2 = *p2 - *p0;
	D3DXVec3Cross(&normal, &edge1, &edge2);
	D3DXVec3Normalize(&normal, &normal);

	// ����� �븻 �������� ���������� �ִ� �Ÿ�
	float dist = -D3DXVec3Dot(&normal, p0);

	//��鰪 ����
	pOutPlane->a = normal.x;
	pOutPlane->b = normal.y;
	pOutPlane->c = normal.z;
	pOutPlane->d = dist;
}

float physicsManager::planeDot(LPD3DXPLANE plane, D3DXVECTOR3 * point)
{
	// ����� �븻
	D3DXVECTOR3 normal(plane->a, plane->b, plane->c);

	/*
	// ��� ���� ������ ����???
	D3DXVECTOR3 p = normal * -pPlane->d;

	// ������ ������ �������� ���� �Ű������� ���� �������� ���⺤��
	D3DXVECTOR3 dir = *point - p;

	// ����
	float distance = D3DXVec3Dot( &normal, &dir );
	*/

	float distance = D3DXVec3Dot(&normal, point) + plane->d;

	return distance;
}

bool physicsManager::intersectRayToPlane(D3DXVECTOR3 * pOut, LPRay ray, LPD3DXPLANE plane, bool checkToSide)
{
	// �븻 ����
	D3DXVECTOR3 normal(plane->a, plane->b, plane->c);

	float dot2 = D3DXVec3Dot(&normal, &ray->direction);	//������ ����� ����� ���� ������ ������ cos ��

	// dot2 �� 0 �̶������ �������� ����� ����� ������ �����Ѵٴ� �����ε�...
	// �̴� �� ���� �������� �����ϴٴ� ���Ⱑ �ȴ�.
	if (FLOATEQUAL(dot2, 0.0f))
	{
		return false;
	}

	// �������� ���������� �������� �ִܰŸ�
	float dist = D3DXVec3Dot(&normal, &ray->origin) + plane->d;

	// ��� üũ�� ���Ѵٸ�...
	if (checkToSide == false)
	{
		//�������� ���� ��ġ�� �ڿ� �־ ����
		if (dist < 0.0f)
			return false;
	}

	// �������������� �浹���������� �Ÿ�
	float t = dist / -dot2;

	// ���� ������ �ݴ�� �Ǿ��ִ� ���
	if (t < 0.0f)
		return false;

	// �浹 ��ġ
	*pOut = ray->origin + (ray->direction * t);

	return true;
}