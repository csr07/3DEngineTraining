attribute vec3 a_pos;
attribute vec2 a_uv;

varying   vec2 v_uv;
varying   vec2 v_uvTiling;
varying   vec4 v_posL;

uniform mat4 u_WVPMatrix;
uniform float u_tilingFactor;

void main()
{
	vec4 posL = vec4(a_pos, 1.0);	
	v_posL = posL;
	gl_Position = u_WVPMatrix * posL;	
	
	v_uv = a_uv;
	v_uvTiling = a_uv * u_tilingFactor;	
}  