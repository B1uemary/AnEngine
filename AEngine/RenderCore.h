#pragma once
#ifndef __RENDERCORE_H__
#define __RENDERCORE_H__

#include"onwind.h"
#include"d3dx12.h"
#include<dxgi1_4.h>
#include<mutex>
using namespace Microsoft::WRL;

namespace RenderCore
{
	static const UINT DefaultFrameCount = 2;
	static const UINT SwapChainBufferCount = 3;
	static constexpr UINT DefaultThreadCount = 1;
	static const float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const D3D_FEATURE_LEVEL MinD3DFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	static const bool IsUseWarpDevice = false;

	class CommandQueue
	{
		ComPtr<ID3D12CommandQueue> commandQueue;

		mutex fenceMutex;
		mutex eventMutex;

		ComPtr<ID3D12Fence> fence;
		uint64_t nextFenceValue;
		uint64_t lastCompleteFenceValue;
		HANDLE fenceEvent;

	public:
		CommandQueue() = default;
		~CommandQueue() = default;

		void Initialize(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);
		void Release();

		const ID3D12CommandQueue* GetCommandQueue() const;
		D3D12_COMMAND_LIST_TYPE GetType();
	};

	class GraphicCard
	{
		ComPtr<ID3D12Device> device;

		CommandQueue renderCommandQueue;
		CommandQueue computeCommandQueue;
		CommandQueue copyCommandQueue;

		void CreateDevice();
		void CreateCommandQueue(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);

	public:
		GraphicCard() = default;
		GraphicCard(const GraphicCard& graphicCard);
		~GraphicCard() = default;

		void Initialize(bool compute = false, bool copy = false);
		const ID3D12CommandQueue* GetCommandQueue(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT) const;
		const ID3D12Device* GetDevice() const;
	};

	extern vector<GraphicCard> r_renderCore;
	extern ComPtr<IDXGISwapChain1> r_swapChain;

	void InitializeRender(int graphicCardCount = 1);

	inline void GetHardwareAdapter(_In_ IDXGIFactory2* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter)
	{
		ComPtr<IDXGIAdapter1> adapter;
		*ppAdapter = nullptr;

		for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				// ��ѡ�������ʾ���������������ʹ�������Ⱦ�����������м���"/warp"
				continue;
			}

			// ����Կ��Ƿ�֧��DX 12�����ǲ�������ʵ���Կ�
			if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			{
				break;
			}
		}
		*ppAdapter = adapter.Detach();
	}

	void InitializeSwapChain();
}

using namespace RenderCore;

#endif // !__RENDERCORE_H__
