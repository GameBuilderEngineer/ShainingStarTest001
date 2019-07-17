//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//グローバル
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float4x4 matrixProjection;
float4x4 matrixView;
texture textureDecal;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//定義
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
sampler textureSampler = sampler_state
{
	texture = <textureDecal>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = POINT;
};

struct VS_OUT
{
	float4 position : POSITION;
	float2 uv : TEXCOORD0;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//バーテックス・シェーダー
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VS_OUT VS(
	float4 position : POSITION, 
	float2 localUV : TEXCOORD0)
{
	VS_OUT Out;

	Out.position = float4(
		position.x,
		position.y,
		0.0f,
		1.0f);

	float4x4 matrixWorld = float4x4(
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f, 0.0f, 0.0f,1.0f);

	matrixWorld = mul(matrixWorld,matrixView);
	matrixWorld = mul(matrixWorld,matrixProjection);

	Out.position = mul(Out.position, matrixWorld);

	Out.uv = localUV;

	return Out;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ピクセル・シェーダー
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float4 PS(VS_OUT In) : COLOR0
{
	return tex2D(textureSampler, In.uv);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// テクニック
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
technique main{
	pass p0 {
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
}
