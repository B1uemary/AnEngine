#pragma once
#ifndef __MCAMERA_H__
#define __MCAMERA_H__

#include"onwind.h"
#include"DX.h"
using namespace DirectX;

class MCamera
{
	struct KeyPressed
	{
		bool w, a, s, d, up, left, down, right;
	};
	XMFLOAT3 initialPosition;	// ��ʼλ��
	XMFLOAT3 position;
	float yawRotate;	// ˮƽת��
	float pitchRotate;	// ��ֱת��
	XMFLOAT3 lookDirection;
	XMFLOAT3 upDirection;
	float moveSpeed;	// ����ƶ��ٶ�
	float turnSpeed;	// ���ת���ٶ�
	KeyPressed keyPressed;

	void Reset();

public:
	MCamera();
	~MCamera();

	void OnInit(XMFLOAT3 _position);
	void OnUpdate(float _elapsedSeconds);
	void OnRelease();

	void OnKeyUp(UINT8 _key);
	void OnKeyDown(UINT8 _key);

	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix(float _fov, float _aspectRatio, float _nearPlane = 1.0f, float _farPlane = 1000.0f);
	void SetMoveSpeed(float v);// ��λ��uint/s
	void SetTurnSpeed(float v);// ��λ��s^-1 || /s
};

#endif // !__MCAMERA_H__
