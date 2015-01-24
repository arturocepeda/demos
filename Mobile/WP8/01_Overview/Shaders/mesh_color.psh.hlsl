
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Pixel shader (3D mesh, solid color, lighting)
//
//  --- mesh_color.psh.hlsl ---
//
//////////////////////////////////////////////////////////////////


cbuffer ConstantMaterial : register(b0)
{
   float4 vDiffuseColor;
   float4 vSpecularColor;
};

cbuffer ConstantLighting : register(b1)
{
   float4 vAmbientLightColor;
   float4 vLightColor;
   float3 vLightPosition;
   float fSpotAngle;
   float3 vLightDirection;
   float fAttenuation;
   int iLightType;
   float3 vConstantLightingPadding;
};

struct PixelShaderInput
{
   float4 Position : SV_POSITION;
   float4 WorldSpacePosition : TEXCOORD1;
   float3 WorldSpaceNormal : TEXCOORD2;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
   return vDiffuseColor;
}
