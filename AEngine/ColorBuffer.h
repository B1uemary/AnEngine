#pragma once
#ifndef __COLORBUFFER_H__
#define __COLORBUFFER_H__

#include"DX.h"
#include"PixelBuffer.h"
#include"Color.h"
using namespace std;
using namespace RenderCore;

namespace RenderCore
{
	namespace Resource
	{
		class ColorBuffer: public PixelBuffer
		{
		protected:
			Color m_clearColor;
			D3D12_CPU_DESCRIPTOR_HANDLE m_srvHandle;
			D3D12_CPU_DESCRIPTOR_HANDLE m_rtvHandle;
			D3D12_CPU_DESCRIPTOR_HANDLE m_uavHandle[12];
			uint32_t m_numMipMaps;	//�����������
			uint32_t m_fragmentCount;
			uint32_t m_sampleCount;

			D3D12_RESOURCE_FLAGS CombineResourceFlags() const;
			static uint32_t ComputeNumMips(uint32_t width, uint32_t height);
			void CreateDerivedViews(ID3D12Device* device, DXGI_FORMAT& format, 
				uint32_t arraySize, uint32_t numMips = 1);

		public:
			ColorBuffer() = delete;
			~ColorBuffer() = default;
			ColorBuffer(Color clearColor = Color(0.0f, 0.0f, 0.0f, 0.0f));

			// �ӽ�����������������ɫ������������������ޡ�
			void CreateFromSwapChain(const wstring& name, ID3D12Resource* baseResource, ID3D12Device* device);
			// ������ɫ������������ṩ�˵�ַ�򲻻�����ڴ档�����ַ���������������������ڿ�Խ֡����ESRAM�ر����ã�������
			void Create(const wstring& name, uint32_t _width, uint32_t _height, uint32_t numMips,
				DXGI_FORMAT& format, D3D12_GPU_VIRTUAL_ADDRESS vidMemPtr = RenderCore::Resource::GpuVirtualAddressUnknown);
			// ������ɫ�����������ڴ���䵽ESRAM��Xbox One������Windows�ϣ����������Create()û����Ƶ��ַʱ��ͬ�� ??
			//void Create(const std::wstring& name, uint32_t _width, uint32_t _height, uint32_t numMips,
			//	DXGI_FORMAT& format, EsramAllocator& allocator);

			// ������ɫ������������ṩ�˵�ַ�򲻻�����ڴ档�����ַ���������������������ڿ�Խ֡����ESRAM�ر����ã�������
			void CreateArray(const wstring& name, uint32_t _width, uint32_t _height, uint32_t arrayCount,
				DXGI_FORMAT& format, D3D12_GPU_VIRTUAL_ADDRESS vidMemPtr = RenderCore::Resource::GpuVirtualAddressUnknown);

			// ��ȡCPU�ɷ��ʵľ��
			const D3D12_CPU_DESCRIPTOR_HANDLE& GetSRV() const;
			// ��ȡCPU�ɷ��ʵľ��
			const D3D12_CPU_DESCRIPTOR_HANDLE& GetRTV() const;
			// ��ȡCPU�ɷ��ʵľ��
			const D3D12_CPU_DESCRIPTOR_HANDLE& GetUAV() const;
			void SetClearColor(Color clearColor);
			void SetMsaaMode(uint32_t numColorSample, uint32_t numCoverageSample);
			Color GetClearColor() const;
		};
	}
}

#endif // !__COLORBUFFER_H__
