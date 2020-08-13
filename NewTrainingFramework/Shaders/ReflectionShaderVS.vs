attribute vec3 a_pos;
attribute vec2 a_uv;
attribute vec3 a_normal;

varying   vec3 v_reflect;

uniform	  vec3 u_cameraPos;
uniform   mat4 u_WVPMatrix;
uniform   mat4 u_WMatrix;

void main()
{
	vec4 posL   = vec4(a_pos,1.0);
	vec4 posW   = u_WVPMatrix * posL;
	gl_Position = posW;
	
	 

	vec4 normL = vec4(a_normal,0.0);
	vec4 normW = u_WMatrix * normL;

	vec3 toEye = u_cameraPos - (u_WMatrix*vec4(a_pos,1.0)).xyz;
	v_reflect = reflect(normalize(-toEye), normalize(normW.xyz));
	
}