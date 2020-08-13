attribute vec3 a_pos;
attribute vec2 a_uv;
attribute vec3 a_normal;
attribute vec3 a_binormal;
attribute vec3 a_tgt;

varying vec3 v_posW;
varying vec2 v_uv;
varying vec3 v_normalW;
varying vec3 v_binormalW;
varying vec3 v_tgtW;

uniform float u_tilingFactor;
uniform   mat4 u_WVPMatrix;
uniform   mat4 u_WMatrix;

void main()
{
	v_posW		= (u_WMatrix * vec4(a_pos,1.0)).xyz;	
	v_uv		= a_uv;		
	v_normalW	= (u_WMatrix * vec4(a_normal, 0.0)).xyz;
	v_binormalW = (u_WMatrix * vec4(a_binormal, 0.0)).xyz;
	v_tgtW		= (u_WMatrix * vec4(a_tgt, 1.0)).xyz;	

	gl_Position = u_WVPMatrix * vec4(a_pos,1.0);	
}