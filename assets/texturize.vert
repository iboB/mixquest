uniform mat4 u_proj;

attribute vec3 a_pos;
attribute vec2 a_texCoord;

varying vec2 v_texCoord;

void main(void)
{
    v_texCoord = a_texCoord;
    gl_Position = u_proj * vec4(a_pos.xyz, 1.0);
}
