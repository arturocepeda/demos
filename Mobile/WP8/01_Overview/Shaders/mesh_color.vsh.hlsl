
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Vertex shader (3D mesh, solid color, lighting)
//
//  --- mesh_color.vsh.hlsl ---
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
};

struct VertexShaderOutput
{
   float4 Position : SV_POSITION;
   float4 WorldSpacePosition : TEXCOORD1;
   float3 WorldSpaceNormal : TEXCOORD2;
};

VertexShaderOutput main(VertexShaderInput input)
{
   VertexShaderOutput output;

   output.Position = mul(float4(input.Position, 1.0f), mWorldViewProjectionMatrix);
   output.WorldSpacePosition = mul(float4(input.Position, 1.0f), mWorldMatrix);
   output.WorldSpaceNormal = mul(input.Normal, (float3x3)mInverseTransposeWorldMatrix);

   return output;
}
