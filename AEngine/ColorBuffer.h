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
			D3D12_RESOURCE_FLAGS CombineResourceFlags() const;

		public:
			ColorBuffer() = default;
			~ColorBuffer() = default;

			// �ӽ�����������������ɫ������������������ޡ�
			void CreateFromSwapChain(const wstring& name, ID3D12Resource* baseResource);
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
