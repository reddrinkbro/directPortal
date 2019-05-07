#pragma once

class light
{
public :
	transform	_transform;			//  ����Ʈ Ʈ������
	D3DXCOLOR	_color;				//  ����Ʈ ����
	float		_intensity;			//  ����Ʈ ����

public :
	//  ����Ʈ ���
	//  Pos.x,      Pos.y,      Pos.z,     lightType (0 Direction, 1 Point, 2 Spot)
	//  Dir.x,      Dir.y,      Dir.z,     DistancePow;
	//  Color.r,    Color.g,    Color.b,   strength
	//  MaxRange,   MinRange,   InAngle    OutAngle 

	//  ����Ʈ ��� ���
	virtual D3DXMATRIXA16 getLightMatrix() = 0;
	//  ����Ʈ ����� �׸���
	virtual void renderGizmo() = 0;

	light() : _intensity(1.0f) {};
	~light() {};

};