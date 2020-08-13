attribute vec3 a_pos;
attribute vec2 a_uv;

varying   vec2 v_uv;

void main()
{
	vec4 posL = vec4(a_pos, 1.0);	
	gl_Position = posL;	
	
	v_uv = a_uv;
}