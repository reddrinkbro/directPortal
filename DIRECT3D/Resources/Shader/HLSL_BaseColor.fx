//
// 전역 변수
//
float4x4		matWorld;		//월드 행렬
float4x4		matView;		//뷰 행렬
float4x4		matProjection;	//투영 행렬
float4		vColor;			//출력 컬러

//
// 정점셰이더에서 픽셀셰이더로 넘겨주는 정보의 구조체 
//
struct VS_OUTPUT
{
	float4 Position : POSITION;		//예는 동차영역 변환정보로 무조건 넘겨야 한다. ( Pixel 함수까지 안간다 )
	float4 Color : COLOR0;
};


//
// 정점 셰이더 함수
//

//정점에서 받을 정보가 매개변수로 들어온다
VS_OUTPUT vs_main( float3 Position : POSITION )
{
	VS_OUTPUT Output;

	//정점 월드 변환
	Output.Position = mul( float4( Position, 1.0 ), matWorld );
	
	//정점 뷰 변환
	Output.Position = mul( Output.Position, matView );

	//정점 투영 변환
	Output.Position = mul( Output.Position, matProjection );

	//컬러 대입
	Output.Color = vColor;

	//리턴 
	return Output;
}

//
// 픽셀 셰이더 함수
//
//정점셰이더에서 리턴된 정보가 매개변수로 들어온다
//이때 픽셀단위로 보간되어 들어오고 
//POSITION 값은 빠져서오게 된다.
float4 ps_main( VS_OUTPUT In ) : COLOR0	
{
	//그냥 아무짓하지말고 In 에 들어온 컬러 리턴
	return In.Color;
}


//
// 테크닉
//
technique MyShader
{
	pass P0
	{
		VertexShader = compile vs_3_0 vs_main();
		PixelShader  = compile ps_3_0 ps_main();
	}
}

