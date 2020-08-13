#include "stdafx.h"
#include "Camera.h"
#include "Globals.h"

Camera::Camera()
{		
}

bool Camera::Init(float i_near, float i_far, float i_fov, float i_speed, float i_speedRot)
{
	m_positionInfo = Vector3(0.0f,1.0f,5.0f);
	m_rotateInfo = Vector3(0.0f,0.0f,0.0f);	
		
	m_near = i_near;
	m_far = i_far;
	m_fov = i_fov;
	m_speed = i_speed;
	m_speedCamRot = i_speedRot;
	m_projMatrix.SetPerspective(m_fov,(float)Globals::screenWidth / Globals::screenHeight,m_near,m_far);

	return true;
}

void Camera::Update(float dTime)
{

}

void Camera::move_Forward(float deltaTime)
{
	Vector4 moveL;
	Vector4 moveW;
	Matrix worldMatrix;	

	worldMatrix = getWorldMatrix();
	moveL = Vector4(0,0,-deltaTime * m_speed, 1);
	moveW = moveL * worldMatrix;

	m_positionInfo.x += moveW.x;
	m_positionInfo.y += moveW.y;
	m_positionInfo.z += moveW.z;
}

void Camera::move_Backward(float deltaTime)
{
	Vector4 moveL;
	Vector4 moveW;
	Matrix worldMatrix;	

	worldMatrix = getWorldMatrix();
	moveL = Vector4(0,0,deltaTime * m_speed, 1);
	moveW = moveL * worldMatrix;

	m_positionInfo.x += moveW.x;
	m_positionInfo.y += moveW.y;
	m_positionInfo.z += moveW.z;
}

void Camera::move_Left(float deltaTime)
{
	Vector4 moveL;
	Vector4 moveW;
	Matrix worldMatrix;	

	worldMatrix = getWorldMatrix();
	moveL = Vector4(-deltaTime * m_speed, 0,0,1);
	moveW = moveL * worldMatrix;

	m_positionInfo.x += moveW.x;
	m_positionInfo.y += moveW.y;
	m_positionInfo.z += moveW.z;
}

void Camera::move_Right(float deltaTime)
{
	Vector4 moveL;
	Vector4 moveW;
	Matrix worldMatrix;	

	worldMatrix = getWorldMatrix();
	moveL = Vector4(deltaTime * m_speed, 0,0,1);
	moveW = moveL * worldMatrix;

	m_positionInfo.x += moveW.x;
	m_positionInfo.y += moveW.y;
	m_positionInfo.z += moveW.z;
}

void Camera::rotate_Up(float deltaTime)
{
	float angle = (float) (m_rotateInfo.x * 180/M_PI);
	if(angle <= 90)
		m_rotateInfo.x += (deltaTime * m_speedCamRot * M_PI / 180.0f);
}

void Camera::rotate_Down(float deltaTime)
{
	float angle = (float) (m_rotateInfo.x * 180/M_PI);
	if(angle >= -90)
		m_rotateInfo.x += (-deltaTime * m_speedCamRot * M_PI / 180.0f);
}

void Camera::rotate_Left(float deltaTime)
{
	float angle = (float) (m_rotateInfo.y * 180/M_PI);
	m_rotateInfo.y += (deltaTime * m_speedCamRot * M_PI / 180.0f);
}

void Camera::rotate_Right(float deltaTime)
{
	float angle = (float) (m_rotateInfo.y * 180/M_PI);	
	m_rotateInfo.y += (-deltaTime * m_speedCamRot * M_PI / 180.0f);
}

Matrix Camera::getWorldMatrix()
{
	Matrix worldMatrix;

	m_rotationXMatrix.SetIdentity();
	m_rotationYMatrix.SetIdentity();
	m_translationMatrix.SetIdentity();

	m_rotationXMatrix.SetRotationX(m_rotateInfo.x);
	m_rotationYMatrix.SetRotationY(m_rotateInfo.y);

	worldMatrix = m_rotationXMatrix * m_rotationYMatrix * m_translationMatrix;

	return worldMatrix;
}

Matrix Camera::getViewMatrix()
{
	Matrix viewMatrix;

	m_rotationXMatrix.SetIdentity();
	m_rotationYMatrix.SetIdentity();
	m_translationMatrix.SetIdentity();
	
	m_translationMatrix.SetTranslation(-m_positionInfo.x, -m_positionInfo.y, -m_positionInfo.z);
	m_rotationXMatrix.SetRotationX(-m_rotateInfo.x);
	m_rotationYMatrix.SetRotationY(-m_rotateInfo.y);

	viewMatrix = m_translationMatrix * m_rotationYMatrix * m_rotationXMatrix;
	
	return viewMatrix;
}

Matrix Camera::getProjMatrix()
{
	return m_projMatrix;
}