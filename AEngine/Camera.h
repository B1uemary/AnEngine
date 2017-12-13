#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "BaseBehaviour.h"

namespace AEngine::Game
{
	class Camera : public BaseBehaviour
	{

	public:
		Camera() = default;
		~Camera() = default;

		// ͨ�� BaseBehaviour �̳�
		virtual void OnInit() override;
		virtual void OnRunning() override;
		virtual void OnRelease() override;
	};
}

#endif // !__CAMERA_H__
