#include "stdafx.h"
#include "Pass.h"

#include "EffectManager.h"
#include "Shaders.h"
#include "FBO.h"
#include "Square.h"

Pass::Pass()
{

}

Pass::~Pass()
{	
}

void Pass::Init()
{

}

void Pass::Draw()
{	
	if(m_frameBufferId>1)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, EffectManager::GetInstance()->m_pFBOs[m_frameBufferId-1].get_hFrameBuffer());//set target
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);//set target
	}	

	glUseProgram(EffectManager::GetInstance()->m_pShaders[m_shaderId].program); //using shader 0

	glEnable(GL_TEXTURE_2D);

	int c = 0;
	//find color textures used..
	for(int i=0; i<3; i++)
	{
		if(m_passTexturesColor[i]>0)
		{
			glActiveTexture(GL_TEXTURE0 + c);
			glBindTexture(GL_TEXTURE_2D,EffectManager::GetInstance()->m_pFBOs[m_passTexturesColor[i]-1].get_hTextureColor());

			if(EffectManager::GetInstance()->m_pShaders[m_shaderId].uTexturePostLoc[c] != -1)
				glUniform1i(EffectManager::GetInstance()->m_pShaders[m_shaderId].uTexturePostLoc[c], c);

			c++;
		}

		if(m_passTexturesDepth[i]>0)
		{
			glActiveTexture(GL_TEXTURE0 + c);
			glBindTexture(GL_TEXTURE_2D,EffectManager::GetInstance()->m_pFBOs[m_passTexturesDepth[i]-1].get_hTextureDepth());

			if(EffectManager::GetInstance()->m_pShaders[m_shaderId].uTexturePostLoc[c] != -1)
				glUniform1i(EffectManager::GetInstance()->m_pShaders[m_shaderId].uTexturePostLoc[c], c);

			c++;
		}
	}

	//mandamos el OtherData de los efectos
	///BLUR STEP
	if(EffectManager::GetInstance()->m_pShaders[m_shaderId].uBlurStepLoc != -1)
	{			
		float x = 1.0f / Globals::screenWidth;
        float y = 1.0f / Globals::screenHeight;
        float z = sqrt(2.0f) / 2.0f * x;
        float w = sqrt(2.0f) / 2.0f * y;	

		glUniform4f(EffectManager::GetInstance()->m_pShaders[m_shaderId].uBlurStepLoc, m_otherData[0]*x, m_otherData[0]*y, m_otherData[0]*z, m_otherData[0]*w );
	}
	///BLOOM LIMIT
	if(EffectManager::GetInstance()->m_pShaders[m_shaderId].uBloomLimitLoc != -1)
	{			
		glUniform1f(EffectManager::GetInstance()->m_pShaders[m_shaderId].uBloomLimitLoc, m_otherData[0]);
	}
	////DOF NEAR
	if(EffectManager::GetInstance()->m_pShaders[m_shaderId].uNearLoc != -1)
		glUniform1f(EffectManager::GetInstance()->m_pShaders[m_shaderId].uNearLoc, m_otherData[1]);
	////DOF FAR
	if(EffectManager::GetInstance()->m_pShaders[m_shaderId].uFarLoc != -1)
		glUniform1f(EffectManager::GetInstance()->m_pShaders[m_shaderId].uFarLoc, m_otherData[2]);
	////DOF FADE
	if(EffectManager::GetInstance()->m_pShaders[m_shaderId].uFadeLoc != -1)
		glUniform1f(EffectManager::GetInstance()->m_pShaders[m_shaderId].uFadeLoc, m_otherData[3]);
	////DOF CLARITY
	if(EffectManager::GetInstance()->m_pShaders[m_shaderId].uClarityLoc != -1)
		glUniform1f(EffectManager::GetInstance()->m_pShaders[m_shaderId].uClarityLoc, m_otherData[4]);


	EffectManager::GetInstance()->m_pSquare->draw(EffectManager::GetInstance()->m_pShaders + m_shaderId);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glUseProgram(0);
}

//void Pass::Draw2(int num)
//{
//	int shader_to_use = 0;
//	/*Buscamos el shader que necesitamos*/
//	for(int i = 0; i < num; i++)
//	{
//		if(EffectsManager::GetInstance()->getShaders(i)->getShaderInfo().ID == ShaderId)
//		{
//			shader_to_use = i;
//			break;
//		}
//	}
//	/*Iteramos el numero de FBO que tengamos en este caso 3*/
//	for(int i = 0; i < MAX_TEXT_POST; i++)
//	{
//		/*Si el valor de la textura es mayor a cero hacemos un bind al frame buffer object target, siempre y cuando el target no sea el cero (pintar a pantalla)*/
//		if(Textures[i]>0)
//		{
//			if(Target_FBO_ID>1)
//				glBindFramebuffer(GL_FRAMEBUFFER, EffectsManager::GetInstance()->getFBO(Target_FBO_ID-1)->get_hFrameBuffer() /*Target_FBO_ID*/);//set target
//			else
//				glBindFramebuffer(GL_FRAMEBUFFER, 0);//set target
//
//			/*Empezamos a usar el shader elejido para esta pasada*/
//			glUseProgram(EffectsManager::GetInstance()->getShaders(shader_to_use)->getShaderInfo().program);
//			/*Vamos a indicar al shader que informacion va a utilizar y mandamos uniforms*/
//			for(int i = 0; i < MAX_TEXT_POST; i++)
//			{
//				glEnable(GL_TEXTURE_2D);
//				glActiveTexture(GL_TEXTURE0+i);
//				/*Seleccionamos el tipo de textura a utilizar, Color o Depth*/
//				if(Color_Dept_Conf[i] == COLOR_TEXTURE) 
//					glBindTexture(GL_TEXTURE_2D,EffectsManager::GetInstance()->getFBO(Textures[i]-1)->get_hTextureColor());
//				else if(Color_Dept_Conf[i] == DEPTH_TEXTURE)
//					glBindTexture(GL_TEXTURE_2D,EffectsManager::GetInstance()->getFBO(Textures[i]-1)->get_hTextureDepth());
//				
//				switch(i)
//				{
//					case 0:
//					if(EffectsManager::GetInstance()->getShaders(shader_to_use)->getShaderInfo().uTex1Loc != -1)
//						glUniform1i(EffectsManager::GetInstance()->getShaders(shader_to_use)->getShaderInfo().uTex1Loc, 0);
//					break;
//					case 1:
//					if(EffectsManager::GetInstance()->getShaders(shader_to_use)->getShaderInfo().uTex2Loc != -1)
//						glUniform1i(EffectsManager::GetInstance()->getShaders(shader_to_use)->getShaderInfo().uTex2Loc, 1);
//					break;
//					case 2:
//					if(EffectsManager::GetInstance()->getShaders(shader_to_use)->getShaderInfo().uTex3Loc != -1)
//						glUniform1i(EffectsManager::GetInstance()->getShaders(shader_to_use)->getShaderInfo().uTex3Loc, 2);
//					break;
//				}//switch
//			}//for()
//			/****Aquí manda todos los demas uniforms que necesites****/
//			/*En esta parte mandamos a dibujar utilizando la clase cuadrado*/
//			EffectsManager::GetInstance()->getSquare()->draw(EffectsManager::GetInstance()->getShaders(shader_to_use));
//			glBindTexture(GL_TEXTURE_2D, 0);
//		}
//	}
//	glUseProgram(0);
//}

void Pass::SetTexturesColor(int * color)
{
	for(int i=0; i<3; i++)
		m_passTexturesColor[i] = color[i];
}

void Pass::SetTexturesDepth(int * depth)
{
	for(int i=0; i<3; i++)
		m_passTexturesDepth[i] = depth[i];
}

void Pass::SetOtherData(float * other)
{
	for(int i=0; i<m_numOtherData; i++)
	{
		m_otherData[i] = other[i];
	}
	
}