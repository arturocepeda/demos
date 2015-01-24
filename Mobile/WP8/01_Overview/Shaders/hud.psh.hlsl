
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Pixel shader (HUD)
//
//  --- hud.fsh.hlsl ---
//
//////////////////////////////////////////////////////////////////


Texture2D DiffuseTexture : register(t0);
SamplerState TextureSamplerState : register(s0);

cbuffer ConstantMaterial : register(b0)
{
   float4 vDiffuseColor;
   float4 vSpecularColor;
};

struct PixelShaderInput
{
   float4 Position : SV_POSITION;
   float2 TextCoord0 : TEXCOORD0;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
   return DiffuseTexture.Sample(TextureSamplerState, input.TextCoord0) * vDiffuseColor;
}
