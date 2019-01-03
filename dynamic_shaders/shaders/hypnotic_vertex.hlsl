float4x4 mvp;

struct VS_OUTPUT 
{
   float4 Pos: POSITION;
   float2 TexCoord: TEXCOORD0; 
};

VS_OUTPUT main(float4 Pos: POSITION)
{
   VS_OUTPUT Out;
   Out.Pos = mul(mvp,Pos);
   Out.TexCoord = normalize(Pos.xy);
   return Out;
}
