
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Vertex shader (Text)
//
//  --- text.vsh.hlsl ---
//
//////////////////////////////////////////////////////////////////


cbuffer ConstantTransform : register(b0)
{
   matrix mWorldViewProjectionMatrix;
   matrix mWorldMatrix;
   matrix mInverseTransposeWorldMatrix;
};

struct VertexShaderInput
{
   float3 Position : POSITION;
   float2 TextCoord0 : TEXCOORD0;
};

struct VertexShaderOutput
{
   float4 Position : SV_POSITION;
   float2 TextCoord0 : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input)
{
   VertexShaderOutput output;

   output.Position = mul(float4(input.Position, 1.0f), mWorldViewProjectionMatrix);
   output.TextCoord0 = input.TextCoord0;

   return output;
}
