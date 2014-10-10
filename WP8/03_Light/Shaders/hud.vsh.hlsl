
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Vertex shader (HUD)
//
//  --- hud.vsh.hlsl ---
//
//////////////////////////////////////////////////////////////////


cbuffer ConstantBuffer : register(b0)
{
   matrix mWorldViewProjectionMatrix;
   float4 vObjectColor;
};

struct VertexShaderInput
{
   float3 Position : POSITION;
   float2 TextCoord0 : TEXCOORD0;
};

struct VertexShaderOutput
{
   float4 Position : SV_POSITION;
   float4 Color : COLOR0;
   float2 TextCoord0 : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input)
{
   VertexShaderOutput output;

   output.Position = mul(float4(input.Position, 1.0f), mWorldViewProjectionMatrix);
   output.Color = vObjectColor;
   output.TextCoord0 = input.TextCoord0;

   return output;
}
