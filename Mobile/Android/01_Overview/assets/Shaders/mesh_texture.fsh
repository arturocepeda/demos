
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Fragment shader (3D mesh with texture)
//
//  --- mesh_texture.fsh ---
//
//////////////////////////////////////////////////////////////////


precision lowp float;

varying vec3 varEyeSpaceVertexNormal;
varying vec4 varEyeSpaceVertexPosition;
varying vec4 varEyeSpacePointLight1Position;
varying vec2 varTextCoord0;

uniform vec4 uObjectColor;
uniform sampler2D uTexture0;
uniform vec4 uAmbientLightColor;
uniform vec4 uPointLight1Color;

const float fSpecularExponent = 10.0;

void main()
{
   vec3 vColor;
   float fAlpha = uObjectColor.w;

   // material properties
   vec3 vMaterialDiffuseColor = texture2D(uTexture0, varTextCoord0).xyz;
   vec3 vMaterialSpecularColor = vec3(0.1, 0.1, 0.1);
   
   // get camera direction vector
   vec3 vCameraDirection = normalize(-varEyeSpaceVertexPosition).xyz;


   //   
   //  Add ambient light
   //
   vColor = vMaterialDiffuseColor * uAmbientLightColor.xyz * uAmbientLightColor.w;


   //
   //  Add point light
   //   
   vec3 vLightColor = uPointLight1Color.xyz * uPointLight1Color.w;
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
