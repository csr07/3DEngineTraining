precision mediump float;

varying vec3		v_posW;
varying vec2		v_uv;
varying vec3		v_normalW;
varying vec3		v_binormalW;
varying vec3		v_tgtW;

uniform sampler2D	u_s_texture[4];
uniform vec3		u_cameraPos;

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
	temp = u_lightColor[i] * intensity; //vec3(0.0,1.0,0.0) * intensity;// 
	return temp;	
}

void main()
{	
	vec3 normalT = texture2D(u_s_texture[1],v_uv).xyz;

	normalT = (normalT*2.0) - 1.0;
	mat3 TBN = mat3(normalize(v_tgtW), normalize(v_binormalW), normalize(v_normalW));		
	vec3 normal				= normalize(TBN * normalT); // using new normals..

	vec3 totalDiffuse		= vec3(0.0, 0.0,0.0);
	vec3 totalSpecular		= vec3(0.0, 0.0,0.0);
	vec3 lightDirection[5];

	for(int i=0; i<u_numObjLights; i++)
	{
		lightDirection[i]	=  getLightDirection(i);
		totalDiffuse		+= diffuseCI(normal, lightDirection[i], i);
		totalSpecular		+= specular(normal, lightDirection[i], i);
	}

	vec4 ObjColor = texture2D(u_s_texture[0], v_uv);	
	
	gl_FragColor = vec4(  (u_ambientWeight * u_ambientColor + (1.0-u_ambientWeight) * totalDiffuse) * ObjColor.xyz + totalSpecular, ObjColor.w);
}