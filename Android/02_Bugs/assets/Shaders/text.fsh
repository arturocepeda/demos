
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Fragment shader (Text)
//
//  --- text.fsh ---
//
//////////////////////////////////////////////////////////////////


precision lowp float;

varying vec2 varTextCoord0;

uniform vec4 uObjectColor;
uniform sampler2D uTexture0;

void main()
{
   gl_FragColor = texture2D(uTexture0, varTextCoord0) * uObjectColor;
}
