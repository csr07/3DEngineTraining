// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include "Globals.h"
#include <conio.h>
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

#include "ResourceManager.h"
#include "SceneManager.h"
#include "EffectManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

int keyPressed=0;
int keyPressedPost = 96;
int Init ( ESContext *esContext )
{		
	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
	glEnable(GL_DEPTH_TEST);
	
	ResourceManager::GetInstance()->Init(".\\RM.txt");
	ResourceManager::GetInstance()->InitResources();	
	SceneManager::GetInstance()->Init(".\\SM.txt");

	//EffectManager::GetInstance()->Init(".\\EM.txt");
	EffectManager::GetInstance()->Init(".\\EM.txt");

	//creation of shaders and program 
	return 0;
}

bool ActivatePostProcess()
{
	if(EffectManager::GetInstance()->m_numEffects > 0)
	{
		if(keyPressedPost > Globals::KEY_NUMPAD_0 && keyPressedPost <= Globals::KEY_NUMPAD_4)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void Draw ( ESContext *esContext )
{	
	EffectManager::GetInstance()->PreDraw(ActivatePostProcess());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	SceneManager::GetInstance()->Draw();

	if(ActivatePostProcess())
	{
		EffectManager::GetInstance()->Draw(keyPressedPost);
	}

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void ProcessUserInput(float delta)
{
	Camera * cam = SceneManager::GetInstance()->m_pCamera;

	if((keyPressed & (1<<Globals::_KEY_W)) != 0)
		cam->move_Forward(delta);
	if((keyPressed & (1<<Globals::_KEY_A)) != 0)
		cam->move_Left(delta);
	if((keyPressed & (1<<Globals::_KEY_S)) != 0)
		cam->move_Backward(delta);
	if((keyPressed & (1<<Globals::_KEY_D)) != 0)
		cam->move_Right(delta);
	if((keyPressed & (1<<Globals::_KEY_UP)) != 0)
		cam->rotate_Up(delta);
	if((keyPressed & (1<<Globals::_KEY_DOWN)) != 0)
		cam->rotate_Down(delta);
	if((keyPressed & (1<<Globals::_KEY_LEFT)) != 0)
		cam->rotate_Left(delta);
	if((keyPressed & (1<<Globals::_KEY_RIGHT)) != 0)
		cam->rotate_Right(delta);
}

void Update ( ESContext *esContext, float deltaTime )
{	
	ProcessUserInput(deltaTime);	
	SceneManager::GetInstance()->Update(deltaTime);
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	if(bIsPressed)
	{		
		switch(key)
		{
		case Globals::KEY_W:
			keyPressed |= 1<<Globals::_KEY_W;		
		break;
		case Globals::KEY_A:
			keyPressed |= 1<<Globals::_KEY_A;		
		break;
		case Globals::KEY_S:
			keyPressed |= 1<<Globals::_KEY_S;		
		break;
		case Globals::KEY_D:
			keyPressed |= 1<<Globals::_KEY_D;		
		break;
		case Globals::KEY_UP:
			keyPressed |= 1<<Globals::_KEY_UP;		
		break;
		case Globals::KEY_DOWN:
			keyPressed |= 1<<Globals::_KEY_DOWN;		
		break;
		case Globals::KEY_LEFT:
			keyPressed |= 1<<Globals::_KEY_LEFT;		
		break;
		case Globals::KEY_RIGHT:
			keyPressed |= 1<<Globals::_KEY_RIGHT;		
		break;

		case Globals::KEY_NUMPAD_0:
			keyPressedPost = Globals::KEY_NUMPAD_0;		
		break;
		case Globals::KEY_NUMPAD_1:
			keyPressedPost = Globals::KEY_NUMPAD_1;		
		break;
		case Globals::KEY_NUMPAD_2:
			keyPressedPost = Globals::KEY_NUMPAD_2;		
		break;
		case Globals::KEY_NUMPAD_3:
			keyPressedPost = Globals::KEY_NUMPAD_3;		
		break;
		case Globals::KEY_NUMPAD_4:
			keyPressedPost = Globals::KEY_NUMPAD_4;		
		break;
		}
	}
	else
	{
		switch(key)
		{
		case Globals::KEY_W:
			keyPressed &=~ (1<<Globals::_KEY_W);
		break;
		case Globals::KEY_S:
			keyPressed &=~ (1<<Globals::_KEY_S);
		break;
		case Globals::KEY_A:
			keyPressed &=~ (1<<Globals::_KEY_A);
		break;
		case Globals::KEY_D:
			keyPressed &=~ (1<<Globals::_KEY_D);
		break;
		case Globals::KEY_UP:
			keyPressed &=~ (1<<Globals::_KEY_UP);
		break;
		case Globals::KEY_DOWN:
			keyPressed &=~ (1<<Globals::_KEY_DOWN);
		break;
		case Globals::KEY_LEFT:
			keyPressed &=~ (1<<Globals::_KEY_LEFT);
		break;
		case Globals::KEY_RIGHT:
			keyPressed &=~ (1<<Globals::_KEY_RIGHT);
		break;
		}
	}
}

void CleanUp()
{
	
}

int _tmain(int argc, _TCHAR* argv[])
{
	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Hello Basic Engine", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	ResourceManager::DestroyInstance();
	SceneManager::DestroyInstance();
	EffectManager::DestroyInstance();
	CleanUp();
	//identifying memory leaks	
	
	MemoryDump();

	printf("Press any key...\n");
	_getch();
	
	return 0;
}

