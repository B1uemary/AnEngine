#pragma once
#ifndef __RENDERCORE_H__
#define __RENDERCORE_H__

#include"onwind.h"
#include"DX.h"
#include<dxgi1_6.h>
#include<mutex>
#include<atomic>
#include"ColorBuffer.h"
#include"DescriptorHeap.h"
#include"RenderCoreConstants.h"
using namespace Microsoft::WRL;
using namespace std;

namespace RenderCore
{
	namespace Heap
	{
		extern DescriptorAllocator r_h_heapDescAllocator;
	}
};

namespace RenderCore
{
	extern bool r_enableHDROutput;

	namespace Private
	{
		extern ComPtr<IDXGIFactory4> r_cp_dxgiFactory;
	}

	class CommandQueue
	{
		ComPtr<ID3D12CommandQueue> m_cp_commandQueue;

		ComPtr<ID3D12Fence> m_cp_fence;
		atomic_uint64_t m_nextFenceValue;
		atomic_uint64_t m_lastCompleteFenceValue;
		HANDLE m_fenceEvent;
		D3D12_COMMAND_LIST_TYPE m_type;

	public:
		CommandQueue() = default;
		~CommandQueue() = default;

		void Initialize(ID3D12Device2* device, D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);
		void Release();

		const ID3D12CommandQueue* GetCommandQueue() const;
		ID3D12CommandQueue* GetCommandQueue();
		D3D12_COMMAND_LIST_TYPE GetType();
	};

	// �Կ��豸�ӿڡ�
	class GraphicCard
	{
		ComPtr<ID3D12Device2> m_cp_device;

		CommandQueue m_renderCommandQueue;	// ��Ⱦ��ɫ����������С�
		CommandQueue m_computeCommandQueue;	// ������ɫ����������С�
		CommandQueue m_copyCommandQueue;	// �����������

		D3D12_FEATURE_DATA_D3D12_OPTIONS m_featureDataOptions;

		bool m_isTypedUAVLoadSupport_R11G11B10_FLOAT;
		bool m_isTypedUAVLoadSupport_R16G16B16A16_FLOAT;
		bool m_stableFlag;	// ����GPU�Ƿ�Ϊ�ȶ��ģ���Ϊ�ȶ��ģ������ƹ����Ա��ⳬƵ��Ƶ��

		void CreateDevice();
		void CreateCommandQueue(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);

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

	public:
		GraphicCard();
		GraphicCard(const GraphicCard& graphicCard);
		~GraphicCard() = default;

		void Initialize(bool compute = false, bool copy = false);
		const ID3D12CommandQueue* GetCommandQueue(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT) const;
		ID3D12CommandQueue* GetCommandQueue(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);
		const ID3D12Device2* GetDevice() const;
		ID3D12Device2* GetDevice();

		void IsStable(bool isStable);
	};

	extern vector<GraphicCard> r_renderCore;
	extern ComPtr<IDXGISwapChain1> r_cp_swapChain;
	extern Resource::ColorBuffer r_displayPlane[r_cnt_SwapChainBufferCount];

	void InitializeRender(int graphicCardCount = 1, bool isStable = false);

	void InitializeSwapChain(int width, int height, HWND hwnd, DXGI_FORMAT dxgiFormat = r_cnt_DefaultSwapChainFormat);
}


#endif // !__RENDERCORE_H__
