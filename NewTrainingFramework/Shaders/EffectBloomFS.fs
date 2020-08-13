precision mediump float;

uniform sampler2D u_texturePost[3];  //[0] colorBuf  [1]BlurredColorBuf

varying vec2 v_uv;

void main()
{
	vec4 color = texture2D(u_texturePost[0],v_uv);
	vec4 blur  = texture2D(u_texturePost[1],v_uv);

	gl_FragColor = color + 1.2*blur;
}

