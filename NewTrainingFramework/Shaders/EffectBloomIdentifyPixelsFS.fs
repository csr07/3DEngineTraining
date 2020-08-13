precision mediump float;

uniform sampler2D u_texturePost[3];
uniform float u_bloomLimit;

varying vec2 v_uv;

void main()
{
	vec3  color			= texture2D(u_texturePost[0], v_uv).xyz;
	float brightness	= 0.3 * color.r + 0.59 * color.g + 0.11 * color.b;
	float val           = step(u_bloomLimit, brightness);
	gl_FragColor		= vec4(color * val, 1.0);

}