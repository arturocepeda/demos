
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Pixel shader (3D mesh, diffuse texture, lighting)
//
//  --- mesh_texture.psh.hlsl ---
//
//////////////////////////////////////////////////////////////////


Texture2D DiffuseTexture : register(t0);
SamplerState TextureSamplerState : register(s0);

cbuffer ConstantMaterial : register(b0)
{
   float4 vDiffuseColor;
   float4 vSpecularColor;
};

cbuffer ConstantLighting : register(b1)
{
   float4 vAmbientLightColor;
   float4 vLightColor;
   float3 vEyePosition;
   int iLightType;
   float3 vLightPosition;
   float fAttenuation;
   float3 vLightDirection;
   float fSpotAngle;
};

struct PixelShaderInput
{
   float4 Position : SV_POSITION;
   float4 WorldSpacePosition : TEXCOORD1;
   float3 WorldSpaceNormal : TEXCOORD2;
   float2 TexCoord0 : TEXCOORD0;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
   float4 vPosition = input.WorldSpacePosition;
   float3 vNormal = normalize(input.WorldSpaceNormal);
   float3 vToCamera = normalize(vEyePosition - vPosition.xyz);
   float4 vTextureColor = DiffuseTexture.Sample(TextureSamplerState, input.TexCoord0);

   float4 vResultAmbient = vAmbientLightColor;
   float4 vResultDiffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
   float4 vResultSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);

   float fDistance = 0.0f;

   // directional light
   if(iLightType == 0)
   {
      float3 vToLight = -normalize(vLightDirection.xyz);
      vResultDiffuse = vLightColor * max(0.0f, dot(vNormal, vToLight));

      float fSpecularBase = max(0.0f, dot(normalize(reflect(-vToLight, vNormal)), vToCamera));
      float fSpecularPower = 10.0f;
      vResultSpecular = vLightColor * pow(fSpecularBase, fSpecularPower);
   }

   // point light
   else if(iLightType == 1)
   {
      float3 vToLight = vLightPosition - vPosition.xyz;
      fDistance = length(vToLight);
      vToLight = vToLight / fDistance;

      float fAttenuationFactor = 1.0f / (fAttenuation * fDistance);

      vResultDiffuse = vLightColor * max(0.0f, dot(vNormal, vToLight));
      vResultDiffuse *= fAttenuationFactor;

      float fSpecularBase = max(0.0f, dot(vNormal, normalize(vToLight + vToCamera)));
      float fSpecularPower = 10.0f;
      vResultSpecular = vLightColor * pow(fSpecularBase, fSpecularPower);
      vResultSpecular *= fAttenuationFactor;
   }

   float4 vFinalColor = (vResultAmbient + vResultDiffuse + vResultSpecular) * vTextureColor;
   vFinalColor.a = 1.0f;

   return vFinalColor;
}
