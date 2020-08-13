precision mediump float;

varying vec3 v_reflect;

uniform samplerCube u_s_cubeTexture;

void main()
{
	gl_FragColor = textureCube(u_s_cubeTexture,v_reflect);
}