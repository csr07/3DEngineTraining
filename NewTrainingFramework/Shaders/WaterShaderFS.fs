precision mediump float;

varying vec3		v_posW;
varying vec2		v_uv;
varying vec3		v_normalW;
varying vec3		v_binormalW;
varying vec3		v_tgtW;
varying vec2		v_uvTiling;

uniform sampler2D	u_s_texture[4];
uniform vec3		u_cameraPos;
uniform samplerCube u_s_cubeTexture;

uniform float u_time;

uniform vec3		u_ambientColor;
uniform float		u_ambientWeight;
uniform float		u_specularPower;
uniform int			u_numObjLights;

uniform int			u_lightType[5];
uniform vec3		u_lightPosDir[5];
uniform vec3		u_lightColor[5];

vec3 getLightDirection(int i)
{
	vec3 temp;
	if(u_lightType[i] == 0)	//direction
	{
		temp = normalize(u_lightPosDir[i]);
	}
	else //position
	{
		temp = normalize(v_posW - u_lightPosDir[i]);
	}
	return temp;
}

vec3 diffuseCI(vec3 norm, vec3 lightDir, int i)
{
	vec3	temp;
	float	intensity = max(dot(norm,-lightDir),0.0);
	temp = u_lightColor[i] * intensity;
	return temp;
}

vec3 specular(vec3 norm, vec3 lightDir, int i)
{
	vec3 temp;
	

	vec3 reflectDir = normalize(reflect(lightDir,norm));
	
	vec3 toEye = normalize(u_cameraPos - v_posW);

	float intensity = pow(max(dot(reflectDir,toEye),0.0),20.0);
	temp = vec3(0.0,1.0,0.0) * intensity;// u_lightColor[i] * intensity;
	return temp;	
}

void main()
{
		

	//water animation stuff	
	vec2 disp				= texture2D( u_s_texture[2], vec2(v_uv.x + u_time, v_uv.y + u_time)).xy;
	float dMaxReflection     = 0.2;
	vec2 offsetReflection	= dMaxReflection * (2.0 * disp - 1.0);


	vec3 normalTgt			= texture2D(u_s_texture[0],v_uv + offsetReflection).xyz;	
	normalTgt				= ( normalTgt*2.0 ) - 1.0;
	mat3  TBN				= mat3(normalize(v_tgtW), normalize(v_binormalW), normalize(v_normalW));
	vec3  normal			= normalize(TBN * normalTgt);	
	vec3  toEye				= u_cameraPos - v_posW;		
	vec3  reflect			= reflect(normalize(-toEye), normalize(normal));

	//refraction Stuff
	vec4 waterColor						= vec4(0.0, 0.0, 0.25,1.0);
	float u_depthAdjust					= 0.1;
	float depthFactor					= clamp( u_depthAdjust * v_uv.x, 0.0, 1.0);
	float u_depthAdjustDisplacement		= 0.025; //??
	float dMaxRefraction				= v_uv.x * u_depthAdjustDisplacement;
	vec2 offsetRefraction				= dMaxRefraction * (2.0 * disp -1.0);

	vec4 bottomColor		= texture2D(u_s_texture[1],(v_uv + offsetRefraction)); //Rock.tga
	vec4 refractionColor	= mix( bottomColor, waterColor, depthFactor );
	
	//mixing textures stuff
	float fresnelPower		= 2.0;
	float fresnelTerm		= pow((1.0 - abs(dot(normal, normalize(toEye)))),fresnelPower);	
	vec4 reflectColor		= textureCube(u_s_cubeTexture, reflect);		

	//lighting stuff
	vec3 totalDiffuse		= vec3(0.0, 0.0,0.0);
	vec3 totalSpecular		= vec3(0.0, 0.0,0.0);
	vec3 lightDirection[5];

	for(int i=0; i<u_numObjLights; i++)
	{
		lightDirection[i]	=  getLightDirection(i);
		totalDiffuse		+= diffuseCI(normal, lightDirection[i], i);
		//totalSpecular		+= specular(normal, lightDirection[i], i);
	}

	refractionColor = vec4(  (u_ambientWeight * u_ambientColor + (1.0-u_ambientWeight) * totalDiffuse) * refractionColor.xyz, refractionColor.w); 		
	gl_FragColor  = vec4( mix(refractionColor, reflectColor, fresnelTerm));
}