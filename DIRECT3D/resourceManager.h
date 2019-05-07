#pragma once
#include "singletonBase.h"
// =============================================================
// 	## resourceManager ## (���ҽ� �Ŵ���)
// =============================================================
template<typename T, typename A>
class resourceManager : public singletonBase <A>
{
protected:
	typedef map<string, T> mapResource;
	mapResource _resourceList;	// �ʾȿ� ��� ���ҽ� ��ϵ�

public:
	// ���ҽ� �߰�
	T addResource(string fileName, void* param = NULL);
	// ���ҽ� ����
	void removeResource(string fileName);
	// ��� ���ҽ� ����
	void clearResource(void);

	// =============================================================
	// 	## ���ҽ� �ε� �������� ���������Լ� ##
	// =============================================================
	virtual T loadResource(string fileName, void* param = NULL) = 0;
	virtual void releaseResource(T data) = 0;

	resourceManager() {}
	~resourceManager() {}
};

// ���ҽ� �߰�
template<typename T, typename A>
inline T resourceManager<T, A>::addResource(string fileName, void * param)
{
	mapResource::iterator find;
	// �̹� ���ҽ��� �߰��� �Ǿ����� Ȯ��
	find = _resourceList.find(fileName);

	// �ش� ���ҽ��� �ʿ� �߰��Ǿ� ���� �ʴٸ� ���� ���� �߰���Ű��
	if (find == _resourceList.end())
	{
		// �ε帮�ҽ� �Լ��� ���ο� ���ҽ� �߰�
		T newResource = this->loadResource(fileName, param);
		// ���ҽ� �߰��� �����ߴٸ� 0����
		if (newResource == NULL) return NULL;
		// ���ҽ���Ͽ� �߰��ϱ�
		_resourceList.insert(make_pair(fileName, newResource));

		return newResource;
	}

	return find->second;
}

// ���ҽ� ����
template<typename T, typename A>
inline void resourceManager<T, A>::removeResource(string fileName)
{
	mapResource::iterator find;
	// �̹� ���ҽ��� �߰��� �Ǿ����� Ȯ��
	find = _resourceList.find(fileName);

	// ���ҽ��� �ִٸ� �����
	if (find != _resourceList.end())
	{
		// ã�� ���ҽ� ����
		this->releaseResource(find->second);
		// �ش��ϴ� ���� ����
		_resourceList.remove(find);
	}
}

// ��� ���ҽ� ����
template<typename T, typename A>
inline void resourceManager<T, A>::clearResource(void)
{
	mapResource::iterator iter;
	for (iter = _resourceList.begin(); iter != _resourceList.end(); ++iter)
	{
		this->releaseResource(iter->second);
	}
	// ��ü����
	_resourceList.clear();
}