// vertex transformations world
float4x4 mWorld;
// vertex transformation view/projection
float4x4 mViewProjection;

struct VS_OUTPUT
{
	float4 Pos  : POSITION;
	float2 tc: TEXCOORD0;
};

VS_OUTPUT main(float3 Pos  : POSITION0, float4 Color: COLOR0)
{
	VS_OUTPUT Out;
	// tranform vertex
	float4 pos = mul(float4(Pos, 1), mWorld);
	Out.Pos = mul(pos, mViewProjection);
	Out.tc = Color.rg;
	return Out;
}