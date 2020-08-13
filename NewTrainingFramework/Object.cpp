#include "stdafx.h"
#include "Object.h"

#include "Model.h"
#include "Texture.h"
#include "Shaders.h"
#include "Camera.h"
#include "Light.h"

#include "ResourceManager.h"
#include "SceneManager.h"

Object::~Object()
{	
	//destroy arrays de IDs
	if(m_num2DTextures>0 && m_2DTexturesIds!= NULL)
	{
		delete[] m_2DTexturesIds;
		m_2DTexturesIds = NULL;
	}	
	if(m_numCTextures>0 && m_CTexturesIds!= NULL)
	{
		delete[] m_CTexturesIds;
		m_CTexturesIds = NULL;
	}
	if(m_numObjLights>0 && m_objLightsIds!= NULL)
	{
		delete[] m_objLightsIds;
		m_objLightsIds = NULL;
	}
	//Destroy Arrays de Objetos	
	if(m_num2DTextures>0 && m_2DTextures != NULL)
	{			
		delete[] m_2DTextures;
		m_2DTextures = NULL;
	}
	if(m_numCTextures>0 && m_CTextures != NULL)
	{		
		delete[] m_CTextures;
		m_CTextures = NULL;
	}
	if(m_numObjLights>0 && m_pObjLights != NULL)
	{		
		delete[] m_pObjLights;
		m_pObjLights = NULL;
	}

	//Nota: ni los modelos ni los shaders en el scene manager se borran porque se borran primero desde
	//el resource manager y queda liberada la memoria de los que se estaban usando en scene
}

void Object::Init()
{
	//m_tilingFactor = 64.0f; ya esta especificado en el sceneManager	
	m_updateWM = false;
	m_positionMatrix.SetIdentity();
	m_rotationXMatrix.SetIdentity();
	m_rotationYMatrix.SetIdentity();
	m_rotationZMatrix.SetIdentity();
	m_scaleMatrix.SetIdentity();

	m_worldMatrix = CalculateWorldMatrix();

	//////////////////////
	//m_pModel = new Model();
	m_pModel = ResourceManager::GetInstance()->getModelById(m_modelId);

	if(m_modelId == 5 || m_modelId == 6) //Fire Object  
		m_hasAnimTex = true;
	else
		m_hasAnimTex = false;

	printf("RMan el path del MODELO es: %s",m_pModel->m_path,sizeof(m_pModel->m_path));	
	//////////////////////
	if(m_num2DTextures > 0)
	{
		m_2DTextures = new Texture*[m_num2DTextures];
	}
	for(int i=0; i<m_num2DTextures; i++)
	{
		Texture * temp = ResourceManager::GetInstance()->getTextureById(m_2DTexturesIds[i]);		
		m_2DTextures[i] = temp;
	}
	//////////////////////
	if(m_numCTextures > 0)
	{
		m_CTextures = new Texture*[m_numCTextures];
	}
	for(int i=0; i<m_numCTextures; i++)
	{
		m_CTextures[i] = ResourceManager::GetInstance()->getCubeTextureById(m_CTexturesIds[i]);		
	}
	/////////////////////
	//m_pShader = new Shaders();
	m_pShader = ResourceManager::GetInstance()->getShadersById(m_ShadersId);
	////////////////////
	if(m_numObjLights > 0)
	{
		m_pObjLights = new Light*[m_numObjLights];
	}
	for(int i=0; i<m_numObjLights; i++)
	{
		int theID		= m_objLightsIds[i];

		Light * temp = SceneManager::GetInstance()->m_pLights+theID;// this case, the ID is the same as its position in array 				
		m_pObjLights[i] = temp;
	}
}

void Object::Update(float time)
{
	if(m_updateWM)
		CalculateWorldMatrix();

	CalculateLightRotation(time);
}

void Object::Draw()
{	
	glUseProgram(m_pShader->program);	
	glBindBuffer(GL_ARRAY_BUFFER,m_pModel->m_hVertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_pModel->m_hIndexBuffer);

	/////////////////////	
	for(int i=0; i<m_num2DTextures; i++)
	{		
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_2DTextures[i]->m_hTexture);
		if(m_pShader->uTextureLoc[i] != -1)
			glUniform1i(m_pShader->uTextureLoc[i], i);		
	}
	
	if(m_numCTextures > 0)
	{
		
		glActiveTexture(GL_TEXTURE0 + m_num2DTextures + 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_CTextures[0]->m_hTexture);
		
		if(m_pShader->uCubeTextureLoc != -1)
			glUniform1i(m_pShader->uCubeTextureLoc, m_num2DTextures + 0 );
		
	}
	/////////////////////

	if(this->m_modelId == 5) // fire object type
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	}
	
	if(m_pShader->a_position != -1)
	{
		glEnableVertexAttribArray(m_pShader->a_position);
		glVertexAttribPointer(m_pShader->a_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if(m_pShader->a_uv != -1)
	{
		glEnableVertexAttribArray(m_pShader->a_uv);
		glVertexAttribPointer(m_pShader->a_uv, 2, GL_FLOAT,GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float)));
	}	

	if(m_pShader->a_normal != -1)
	{
		glEnableVertexAttribArray(m_pShader->a_normal);
		glVertexAttribPointer(m_pShader->a_normal, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (void*)(5*sizeof(float)));
	}

	if(m_pShader->a_binormal != -1)
	{
		glEnableVertexAttribArray(m_pShader->a_binormal);
		glVertexAttribPointer(m_pShader->a_binormal, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (void*)(10*sizeof(float)));
	}

	if(m_pShader->a_tgt != -1)
	{
		glEnableVertexAttribArray(m_pShader->a_tgt);
		glVertexAttribPointer(m_pShader->a_tgt, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (void*)(13*sizeof(float)));
	}

	if(m_pShader->uWorldMatrixLoc != -1)
	{		
		m_worldMatrix = CalculateWorldMatrix();	
		glUniformMatrix4fv(m_pShader->uWorldMatrixLoc,1,GL_FALSE, &m_worldMatrix.m[0][0]);
	}	

	if(m_pShader->uWLPLoc != -1)
	{
		Matrix temp_wvp = CalculateWVPMatrix();
		glUniformMatrix4fv(m_pShader->uWLPLoc,1,GL_FALSE, &temp_wvp.m[0][0]);
	}

	if(m_pShader->uTilingFactorLoc != -1)
	{			
		glUniform1f(m_pShader->uTilingFactorLoc,m_tilingFactor);
	}	

	if(m_pShader->uCameraPosLoc != -1)
	{			
		float pos[3] = {SceneManager::GetInstance()->m_pCamera->m_positionInfo.x,
					   SceneManager::GetInstance()->m_pCamera->m_positionInfo.y,
					   SceneManager::GetInstance()->m_pCamera->m_positionInfo.z};

		glUniform3fv(m_pShader->uCameraPosLoc,1,&pos[0]);
	}

	if(m_pShader->uFogColorLoc != -1)
	{
		float color[3] = {SceneManager::GetInstance()->m_fogColor.x,
					      SceneManager::GetInstance()->m_fogColor.y,
					      SceneManager::GetInstance()->m_fogColor.z};

		glUniform3fv(m_pShader->uFogColorLoc,1,&color[0]);
	}

	if(m_pShader->uFogStartLoc != -1)
	{			
		glUniform1f(m_pShader->uFogStartLoc,SceneManager::GetInstance()->m_fogStart);
	}

	if(m_pShader->uFogRangeLoc != -1)
	{			
		glUniform1f(m_pShader->uFogRangeLoc,SceneManager::GetInstance()->m_fogRange);
	}

	if(m_hasAnimTex && m_pShader->uTimeLoc != -1)
	{
		glUniform1f(m_pShader->uTimeLoc,SceneManager::GetInstance()->m_totalTime);
	}

	SendLightsData();

	

	
	glDrawElements(GL_TRIANGLES,m_pModel->m_noIndices,GL_UNSIGNED_INT, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(0);
}

Matrix Object::CalculateWorldMatrix()
{
	m_positionMatrix.SetTranslation(m_position);
	m_rotationXMatrix.SetRotationX(m_rotation.x);
	m_rotationYMatrix.SetRotationY(m_rotation.y);
	m_rotationZMatrix.SetRotationZ(m_rotation.z);
	m_scaleMatrix.SetScale(m_scale);

	return m_scaleMatrix * m_rotationXMatrix * m_rotationYMatrix * m_rotationZMatrix * m_positionMatrix;
}

Matrix Object::CalculateWVPMatrix()
{	
	return m_worldMatrix * SceneManager::GetInstance()->m_pCamera->getViewMatrix() * SceneManager::GetInstance()->m_pCamera->getProjMatrix();
}

void Object::SendLightsData()
{
	if(m_pShader->uAmbientColorLoc != -1)
	{
		float color[3] = {SceneManager::GetInstance()->m_ambientColor.x,
					      SceneManager::GetInstance()->m_ambientColor.y,
					      SceneManager::GetInstance()->m_ambientColor.z};

		glUniform3fv(m_pShader->uAmbientColorLoc,1,&color[0]);
	}
	if(m_pShader->uAmbientWeightLoc != -1)
	{
		glUniform1f(m_pShader->uAmbientWeightLoc,SceneManager::GetInstance()->m_ambientW);	
	}
	if(m_pShader->uSpecularPowerLoc != -1)
	{
		glUniform1f(m_pShader->uSpecularPowerLoc,m_specularPower);	
	}
	if(m_pShader->uNumObjLightsLoc != -1)
	{		
		glUniform1i(m_pShader->uNumObjLightsLoc,m_numObjLights);
	}

	for(int i=0; i<m_numObjLights; i++)
	{
		if(m_pShader->uLightTypeLoc[i] != -1)
		{
			glUniform1i(m_pShader->uLightTypeLoc[i],m_pObjLights[i]->m_lightType);
		}
		if(m_pShader->uLightPosDirLoc[i] != -1)
		{
			float temp[3] = {m_pObjLights[i]->m_posDir.x,
					         m_pObjLights[i]->m_posDir.y,
					         m_pObjLights[i]->m_posDir.z};
			glUniform3fv(m_pShader->uLightPosDirLoc[i],1,&temp[0]);
		}
		if(m_pShader->uLightColorLoc[i] != -1)
		{
			float temp[3] = {m_pObjLights[i]->m_color.x,
					         m_pObjLights[i]->m_color.y,
					         m_pObjLights[i]->m_color.z};
			glUniform3fv(m_pShader->uLightColorLoc[i],1,&temp[0]);
		}
	}

}

void Object::CalculateLightRotation(float time)
{
	for(int i=0; i<m_numObjLights; i++)
	{
		if(m_pObjLights[i]->m_lightType == 1)
		{			
			float rad = m_pObjLights[i]->m_radius;
			float angle = time;// * m_pObjLights[i]->m_speed;

			m_pObjLights[i]->m_posDir.x = (rad * cosf(angle)) + m_position.x;
			m_pObjLights[i]->m_posDir.z = (rad * sinf(angle)) + m_position.z;
		}
	}

}