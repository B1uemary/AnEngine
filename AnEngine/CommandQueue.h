#pragma once
#ifndef __COMMANDQUEUE_H__
#define __COMMANDQUEUE_H__


#include"DX.h"
#include<atomic>

namespace AnEngine::RenderCore
{
	class CommandQueue
	{
		ComPtr<ID3D12CommandQueue> m_cp_commandQueue;

		ComPtr<ID3D12Fence> m_cp_fence;
		atomic_uint64_t m_nextFenceValue;
		atomic_uint64_t m_lastCompleteFenceValue;
		HANDLE m_fenceEvent;
		D3D12_COMMAND_LIST_TYPE m_type;

	public:
		// ����Ҫ�޸�
		CommandQueue() = default;
		~CommandQueue() = default;

		void Initialize(ID3D12Device2* device, D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);
		void Release();

		const ID3D12CommandQueue* GetCommandQueue() const;
		ID3D12CommandQueue* GetCommandQueue();
		D3D12_COMMAND_LIST_TYPE GetType();
	};
}

#endif // !__COMMANDQUEUE_H__