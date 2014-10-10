
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  iOS Game Engine
//
//  Fragment shader (3D mesh without texture)
//
//  --- mesh_color.fsh ---
//
//////////////////////////////////////////////////////////////////


precision lowp float;

varying vec3 varEyeSpaceVertexNormal;
varying vec4 varEyeSpaceVertexPosition;
varying vec4 varEyeSpacePointLight1Position;

uniform vec4 uObjectColor;
uniform vec3 uAmbientLightColor;
uniform float uAmbientLightIntensity;
uniform vec3 uPointLight1Color;
uniform float uPointLight1Intensity;

const float fSpecularExponent = 10.0;

void main()
{
   vec3 vColor;
   float fAlpha = uObjectColor.w;

   // material properties
   vec3 vMaterialDiffuseColor = uObjectColor.xyz;
   vec3 vMaterialSpecularColor = vec3(0.1, 0.1, 0.1);
   
   // get camera direction vector
   vec3 vCameraDirection = normalize(-varEyeSpaceVertexPosition).xyz;


   //   
   //  Add ambient light
   //
   vColor = vMaterialDiffuseColor * uAmbientLightColor * uAmbientLightIntensity;


   //
   //  Add point light
   //   
   vec3 vLightColor = uPointLight1Color * uPointLight1Intensity;
   vec3 vLightDirection = normalize(varEyeSpacePointLight1Position - varEyeSpaceVertexPosition).xyz;
   vec3 vHalfPlane = normalize(vLightDirection + vCameraDirection);
      
   float fDiffuseFactor = max(0.0, dot(varEyeSpaceVertexNormal, vLightDirection));
   float fSpecularFactor = max(0.0, dot(varEyeSpaceVertexNormal, vHalfPlane));
   fSpecularFactor = pow(fSpecularFactor, fSpecularExponent);
      
   vColor += (vMaterialDiffuseColor * vLightColor * fDiffuseFactor) + (vMaterialSpecularColor * fSpecularFactor);

   
   //
   //  Set fragment color
   //
   gl_FragColor = vec4(vColor, fAlpha);
}
