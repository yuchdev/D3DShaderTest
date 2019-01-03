float4 main(float2 texCoord: TEXCOORD0) : COLOR
{
   float ang = atan2 (texCoord.x, texCoord.y);
   float rad = dot (texCoord, texCoord);

   return 0.5*(1 + sin (ang + rings * rad + time));
}
