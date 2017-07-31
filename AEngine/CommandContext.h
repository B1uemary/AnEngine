#pragma once
#ifndef __COMMANDCONTEXT_H__
#define __COMMANDCONTEXT_H__

#include"DX.h"
#include<queue>
#include<mutex>

namespace RenderCore
{
	// ��Ⱦ�̶߳�ռһ��CommandList
	class CommandListPool
	{
		queue<ComPtr<ID3D12CommandList>> m_cp_commandListPool;
		std::mutex m_mutex;
		
	public:
		CommandListPool() = default;
		~CommandListPool() = default;
	};
}

#endif // !__COMMANDCONTEXT_H__
