precision mediump float;

varying vec2 v_uv;
varying vec2 v_uvTiling;
uniform sampler2D u_s_texture[4];

varying vec3 v_fragmentWPos;
varying vec4 v_posL;

uniform mat4 u_WMatrix;
uniform vec3 u_cameraPos;

uniform vec3 u_fogColor;
uniform float u_fogStart;
uniform float u_fogRange;


void main()
{
	//TERRAIN COLOR
	vec4 colorGrass		= texture2D(u_s_texture[0], v_uvTiling);
	vec4 colorDirt		= texture2D(u_s_texture[1], v_uvTiling);
	vec4 colorRock		= texture2D(u_s_texture[2], v_uvTiling);
	vec4 colorBlend_map = texture2D(u_s_texture[3], v_uv);

	vec4 terrainColor	= (colorRock * colorBlend_map.x +
						   colorDirt * colorBlend_map.y +
						   colorGrass* colorBlend_map.z) / 
						   (colorBlend_map.x + colorBlend_map.y + colorBlend_map.z); 

	vec4 wPos = u_WMatrix * v_posL;

	float distanceCameraToFragment = distance(vec4(u_cameraPos,1.0),wPos);
	
	float factor = clamp((distanceCameraToFragment-u_fogStart)/u_fogRange,0.0,1.0);
	vec3 colorFinal = mix(vec3(terrainColor.x,terrainColor.y,terrainColor.z),u_fogColor,factor);	

	gl_FragColor = vec4(colorFinal,1.0);
}