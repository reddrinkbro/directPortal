#pragma once
#include "resourceManager.h"
#include "xMeshStatic.h"

class rmStaticMesh : public resourceManager <xMeshStatic*, rmStaticMesh>
{
public :
	HRESULT init(void);
	void release(void);

	//  �μҽ� �ε� ( �ε� ��ĸ� ������)
	xMeshStatic* loadResource(string filePath, void* pParam = NULL) override;
		
	//  ����
	void releaseResource(xMeshStatic* data) override;

	rmStaticMesh() {};
	~rmStaticMesh() {};

};