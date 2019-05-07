#pragma once
class stage3 
{
private:
	baseObject * _object;
	lightDirection*		_light;
	skinnedAnimation*	_ani;
	vector<mapBoundBox> _collision;							//�� �ٿ�� �ڽ�
public:
	HRESULT init(void);
	void release(void);
	void render(void);
	void update(void);
	void setBoundBox(void);
public:
	vector<mapBoundBox> getBoundBox(void) { return _collision; }
	baseObject * getObject(void) { return _object; }
	stage3() {};
	~stage3() {};
};

