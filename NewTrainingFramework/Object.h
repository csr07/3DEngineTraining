#pragma once
#include "../Utilities/utilities.h"

class Model;
class Texture;
class Shaders;
class Light;

class Object
{
public:
				Object(){}
				~Object();
	void		Init();
	void		Update(float time);
	void		Draw();	
	Matrix		CalculateWorldMatrix();
	Matrix		CalculateWVPMatrix();
	void		SendLightsData();
	void		CalculateLightRotation(float time);

	int         m_id;
	int			m_modelId;
	int *		m_2DTexturesIds;
	int *		m_CTexturesIds;
	int			m_ShadersId;

	int			m_num2DTextures;	
	int			m_numCTextures;
	Texture**	m_2DTextures;
	Texture**	m_CTextures;

	Model *		m_pModel;
	Shaders *	m_pShader;

	Vector3     m_position;
	Vector3		m_rotation;
	Vector3     m_scale;

	Matrix		m_positionMatrix;
	Matrix		m_rotationXMatrix;
	Matrix		m_rotationYMatrix;
	Matrix		m_rotationZMatrix;
	Matrix		m_scaleMatrix;
	Matrix		m_worldMatrix;	

	float		m_tilingFactor;
	bool		m_updateWM;
	bool		m_hasAnimTex;

	////Light variables		
	/*Vector3 *	m_lightColor;
	Vector3 *	m_lightPosDir;
	int		*	m_ligthType;
	int		*	m_lightMoveType;
	float	*	m_lightRadius;
	float	*	m_lightSpeed;*/

	int			m_numObjLights;
	int		*   m_objLightsIds;
	Light	**  m_pObjLights;

	//Specific Parameters
	float		m_specularPower;
};