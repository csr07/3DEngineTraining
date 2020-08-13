precision mediump float;

varying vec2 v_uv;

uniform sampler2D u_texturePost[3];


void main()
{
	vec4 bwColor = texture2D(u_texturePost[0] ,v_uv);

	float val = bwColor.x * 0.3 + bwColor.y * 0.59 + bwColor.z * 0.11;
	
	bwColor = vec4(val,val,val, bwColor.w);
		
	gl_FragColor = bwColor;
}