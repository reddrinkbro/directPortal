#pragma once
class stage2
{
private:
	baseObject *		_object;
	lightDirection*		_light;
	vector<mapBoundBox> _collision;							//�� �ٿ�� �ڽ�
public:
	HRESULT init(void);
	void release(void);
	void render(void);
	void setBoundBox(void);
public:
	vector<mapBoundBox> getBoundBox(void) { return _collision; }
	baseObject * getObject(void) { return _object; }
	stage2() {};
	~stage2() {};
};

