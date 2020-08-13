attribute vec3 a_pos;
attribute vec2 a_uv;
attribute vec3 a_normal;

varying vec3 v_posW;
varying vec2 v_uv;
varying vec3 v_normalW;

uniform mat4 u_WVPMatrix;
uniform mat4 u_WMatrix;

void main()
{
	v_posW	  = (u_WMatrix * vec4(a_pos,1.0)).xyz;
	v_normalW = (u_WMatrix * vec4(a_normal,1.0)).xyz;
	v_uv	  = a_uv;		
	
	gl_Position = u_WVPMatrix * vec4(a_pos,1.0);
}