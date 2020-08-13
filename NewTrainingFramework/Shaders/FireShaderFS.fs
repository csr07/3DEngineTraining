precision mediump float;

varying vec2 v_uv;

uniform sampler2D u_s_texture[4];
uniform float u_time;

void main()
{	
	vec2 disp = texture2D(u_s_texture[2], vec2(v_uv.x, v_uv.y +  u_time)).xy;

	vec2 offset = ((2.0 * disp) - 1.0) * 0.1; // dMax

	vec2 new_uv = v_uv + offset;

	vec4 fire_color = texture2D(u_s_texture[0],new_uv);

	vec4 AlphaValue = texture2D(u_s_texture[1], v_uv);

	if(fire_color.a < 0.1)
		discard;

	gl_FragColor = fire_color * vec4(1.0, 1.0, 1.0, AlphaValue.r);
}