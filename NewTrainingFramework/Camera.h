#pragma once
#include "../Utilities/utilities.h"

static const float M_PI = 3.141592f;

class Camera
{
public:	
	Camera();

	bool Init(float i_near, float i_far, float i_fov, float i_speed, float i_speedRot);
	void Update(float dTime);

	void move_Forward(float deltaTime);
	void move_Backward(float deltaTime);
	void move_Left(float deltaTime);
	void move_Right(float deltaTime);
	void rotate_Up(float deltaTime);
	void rotate_Down(float deltaTime);
	void rotate_Left(float deltaTime);
	void rotate_Right(float deltaTime);
	
	Matrix getWorldMatrix(void);
	Matrix getViewMatrix(void);
	Matrix getProjMatrix(void);

	///////////////////////////////////////

	Matrix m_rotationXMatrix;
	Matrix m_rotationYMatrix;
	Matrix m_translationMatrix;

	Matrix m_worldMatrix;
	Matrix m_viewMatrix;
	Matrix m_projMatrix;

	Vector3 m_positionInfo;
	Vector3 m_rotateInfo;

	float	m_near; 
	float   m_far;
	float   m_fov; 
	float   m_speed;
	float   m_speedCamRot;
};

