
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Vertex shader (3D mesh without texture)
//
//  --- mesh_color.vsh.hlsl ---
//
//////////////////////////////////////////////////////////////////


cbuffer ConstantBuffer : register(b0)
{
   matrix mWorldViewProjectionMatrix;
   matrix mWorldViewMatrix;
   matrix mNormalMatrix;
   float4 vObjectColor;
   float4 vAmbientLightColor;   
   float4 vPointLight1Color;
   float3 vPointLight1Position;
};

struct VertexShaderInput
{
   float3 Position : POSITION;
   float3 Normal : NORMAL0;
};

struct VertexShaderOutput
{
   float4 Position : SV_POSITION;
   float4 ObjectColor : COLOR0;
   float3 EyeSpaceVertexNormal : NORMAL0;
   float3 EyeSpaceVertexPosition : POSITION0;
   float3 EyeSpacePointLight1Position : POSITION1;
   float4 AmbientLightColor : COLOR1;
   float4 PointLight1Color : COLOR2;
};

VertexShaderOutput main(VertexShaderInput input)
{
   VertexShaderOutput output;

   output.Position = mul(float4(input.Position, 1.0f), mWorldViewProjectionMatrix);
   output.ObjectColor = vObjectColor;

   output.EyeSpaceVertexNormal = normalize(mul(mNormalMatrix, input.Normal));
   output.EyeSpaceVertexPosition = mul(mWorldViewMatrix, input.Position);
   output.EyeSpacePointLight1Position = mul(mWorldViewMatrix, float4(vPointLight1Position, 0.0f));

   output.AmbientLightColor = vAmbientLightColor;
   output.PointLight1Color = vPointLight1Color;

   return output;
}
