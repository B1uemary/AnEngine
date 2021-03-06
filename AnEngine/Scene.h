#pragma once
#ifndef __SCENE_H__
#define __SCENE_H__

#include"onwind.h"
#include"BaseBehaviour.h"
#include"GameObject.h"
#include"Camera.h"

namespace AnEngine::Game
{
	class Scene :public BaseBehaviour//, public NonCopyable
	{
		std::vector<BaseBehaviour*> m_objects;
		Camera* defaultCamera;

		//protected:
		// 通过 BaseBehaviour 继承
		virtual void OnInit() override;
		virtual void BeforeUpdate();
		virtual void OnUpdate() override;
		virtual void AfterUpdate();
		virtual void OnRelease() override;

	public:
		Scene(std::wstring _name);
		~Scene() = default;

		std::wstring name;

		void AddObject(GameObject* obj);
		void RemoveObject(GameObject* obj);
	};
}

#endif // !__SCENE_H__
