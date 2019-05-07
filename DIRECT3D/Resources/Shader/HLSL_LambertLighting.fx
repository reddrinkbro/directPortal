//
// ���� ����
//
float4x4		matWorld;		//���� ���
float4x4		matView;		//�� ���
float4x4		matProjection;	//���� ���

float4		vColor;			//��� �÷�
float4		vLightDir;		//������ ���� ( ����ȭ �Ǿ� ���Դ� )
float4x4		matInvWorld;	//������ �����

//���������Ϳ��� �ȼ� ���̴��� �Ѱ��ִ� ����ü ����
struct VS_OUTPUT
{
	float4	Position:POSITION;
	float4	Color:COLOR0;
};

//���� ���̴� �Լ�
//�������� ���� ������ �Ű������� ���´�.
VS_OUTPUT vs_main(float3 Position:POSITION, float3 Normal : NORMAL)
{
	VS_OUTPUT Output;

	//�������� ��ȯ
	Output.Position = mul(float4(Position,1),matWorld);

	//���� �� ��ȯ
	Output.Position = mul(Output.Position,matView);

	//���� �������� ��ȯ
	Output.Position = mul(Output.Position, matProjection);

	//���� �븻
	float3 localNormal = Normal;

	//���層���� ������ ���÷� �����.
	float3 localLightDir = mul(vLightDir, (float3x3)matInvWorld);

	//���� ���� ������ ����ȭ
	localLightDir = normalize(localLightDir);

	float3 toLocalLight = -localLightDir;

	//����Ʈ ����
	float diffuse = dot(toLocalLight, Normal);

	//0~1���̷� �߶��ִ� �Լ�
	diffuse = saturate(diffuse);

	//�÷�����
	Output.Color = vColor*diffuse;

	return Output;

}


//
// �ȼ� ���̴� �Լ�
//
//�������̴����� ���ϵ� ������ �Ű������� ���´�
//�̶� �ȼ������� �����Ǿ� ������ 
//POSITION ���� ���������� �ȴ�.
float4 ps_main(VS_OUTPUT In) : COLOR0
{
	//�׳� �ƹ����������� In �� ���� �÷� ����
	return In.Color;
}


//
// ��ũ��
//
technique MyShader
{
	pass P0
	{
		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
	}
}


