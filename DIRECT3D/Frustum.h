#pragma once

class baseObject;

class Frustum
{
private :
	D3DXVECTOR3		_RHWPos[8];		// viewProj ��ȯ�� ��ġ
	D3DXVECTOR3		_pos[8];		// ��ȯ �� ��ġ
	D3DXPLANE		_plane[6];		// ����ü ���

public :
	// ������Ʈ�� �������� �ȿ� �ִ��� üũ
	bool isInFrustum(baseObject* pObject);
	// �������� ���� ������Ʈ
	void updateFrustum(D3DXMATRIXA16* matViewProj);
	// �������� �����
	void renderGizmo();
	
	Frustum();
	~Frustum();
};