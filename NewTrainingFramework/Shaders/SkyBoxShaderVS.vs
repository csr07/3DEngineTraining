attribute vec3 a_pos;
varying	  vec3 v_normal;

uniform mat4 u_WVPMatrix;


void main()
{
	vec4 posL = vec4(a_pos,1.0);	
	gl_Position = u_WVPMatrix * posL;

	v_normal = a_pos;
}