#version 100

precision mediump float;

uniform sampler2D u_tex;

varying vec2 v_texCoord;
varying vec4 v_color;

void main(void)
{
    gl_FragColor = texture2D(u_tex, v_texCoord) * v_color;
}
