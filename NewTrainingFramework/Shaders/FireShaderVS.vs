attribute vec3 a_pos;
attribute vec2 a_uv;

varying   vec2 v_uv;

uniform mat4  u_WVPMatrix;


void main()
{
	vec4 posL = vec4(a_pos,1.0);
	gl_Position = u_WVPMatrix * posL;

	v_uv = a_uv;
}