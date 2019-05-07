#pragma once

class tutorialMap2
{
private:
	baseObject *		 _object;
	lightDirection*		 _light;
	vector<mapBoundBox> _collision;							//�� �ٿ�� �ڽ�
	vector<mapBoundBox> _basicBox;							//�ʿ� �ִ� ���� �������ڽ� �浹, �浹�� ��Ż�� ��� ��ġ�ϱ� ����
public:
	HRESULT init(void);
	void release(void);
	void render(void);
	void setBoundBox(void);
public:
	vector<mapBoundBox> getBoundBox(void) { return _collision; }
	vector<mapBoundBox> getbasicBox(void) { return _basicBox; }
	baseObject * getObject(void) { return _object; }
	tutorialMap2() {};
	~tutorialMap2() {};
};

