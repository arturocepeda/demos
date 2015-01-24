
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Vertex shader (3D mesh, diffuse texture, lighting)
//
//  --- mesh_texture.vsh.hlsl ---
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
   float3 Normal : NORMAL0;
   float2 TexCoord0 : TEXCOORD0;
};

struct VertexShaderOutput
{
   float4 Position : SV_POSITION;
   float4 WorldSpacePosition : TEXCOORD1;
   float3 WorldSpaceNormal : TEXCOORD2;
   float2 TexCoord0 : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input)
{
   VertexShaderOutput output;

   output.Position = mul(float4(input.Position, 1.0f), mWorldViewProjectionMatrix);
   output.WorldSpacePosition = mul(mWorldMatrix, float4(input.Position, 1.0f));
   output.WorldSpaceNormal = mul((float3x3)mInverseTransposeWorldMatrix, input.Normal);
   output.TexCoord0 = input.TexCoord0;

   return output;
}
