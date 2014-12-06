
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Vertex shader (3D mesh without texture)
//
//  --- mesh_color.vsh ---
//
//////////////////////////////////////////////////////////////////


attribute vec4 aPosition;
attribute vec3 aNormal;
attribute vec2 aTextCoord0;

uniform mat4 uModelViewProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform mat3 uNormalMatrix;
uniform vec3 uPointLight1Position;

varying vec3 varEyeSpaceVertexNormal;
varying vec4 varEyeSpaceVertexPosition;
varying vec4 varEyeSpacePointLight1Position;

void main()
{
   // varyings for the fragment shader
   varEyeSpaceVertexNormal = normalize(uNormalMatrix * aNormal);
   varEyeSpaceVertexPosition = uModelViewMatrix * aPosition;
   varEyeSpacePointLight1Position = uModelViewMatrix * vec4(uPointLight1Position, 0.0);
   
   // set vertex position
   gl_Position = uModelViewProjectionMatrix * aPosition;
}
