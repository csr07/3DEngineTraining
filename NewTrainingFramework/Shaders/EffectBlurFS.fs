precision mediump float;

varying vec2 v_uv;

uniform sampler2D u_texturePost[3];
uniform vec4 u_blurStep;

void main()
{
	//vec4 blurColor = texture2D(u_s_texture[0],v_uv);	

	vec3 color1 = texture2D(u_texturePost[0], v_uv).xyz;
	vec3 color2 = texture2D(u_texturePost[0], vec2(v_uv.x + u_blurStep.x,v_uv.y)).xyz;
	vec3 color3 = texture2D(u_texturePost[0], vec2(v_uv.x - u_blurStep.x,v_uv.y)).xyz;
	vec3 color4 = texture2D(u_texturePost[0], vec2(v_uv.x, v_uv.y + u_blurStep.y)).xyz;
	vec3 color5 = texture2D(u_texturePost[0], vec2(v_uv.x, v_uv.y - u_blurStep.y)).xyz;
	vec3 color6 = texture2D(u_texturePost[0], vec2(v_uv.x + u_blurStep.z, v_uv.y + u_blurStep.w)).xyz;
	vec3 color7 = texture2D(u_texturePost[0], vec2(v_uv.x - u_blurStep.z, v_uv.y + u_blurStep.w)).xyz;
	vec3 color8 = texture2D(u_texturePost[0], vec2(v_uv.x - u_blurStep.z, v_uv.y - u_blurStep.w)).xyz;
	vec3 color9 = texture2D(u_texturePost[0], vec2(v_uv.x + u_blurStep.z, v_uv.y - u_blurStep.w)).xyz;

	gl_FragColor = vec4(((color1*2.0) + color2 + color3 + color4 + color5 + color6 + color7 + color8 + color9)*0.1, 1.0);	
	//gl_FragColor = u_blurStep;
}