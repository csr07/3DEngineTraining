precision mediump float;

varying vec3        v_normal;
uniform samplerCube u_s_cubeTexture;

void main()
{	
	vec4 color = textureCube(u_s_cubeTexture, v_normal);
	gl_FragColor = color;
}