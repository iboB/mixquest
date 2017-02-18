uniform mat4 u_proj;

attribute vec3 a_pos;
attribute vec2 a_texCoord;
attribute vec4 a_color;

varying vec2 v_texCoord;
varying vec4 v_color;

void main(void)
{
    v_texCoord = a_texCoord;
    v_color = a_color;
    gl_Position = u_proj * vec4(a_pos.xyz, 1.0);
}
