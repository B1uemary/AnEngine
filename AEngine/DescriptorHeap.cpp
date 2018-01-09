#include "DescriptorHeap.hpp"
#include "RenderCore.h"
using namespace AEngine::RenderCore::Resource;

namespace AEngine::RenderCore::Heap
{
	//DescriptorHeapAllocator r_h_heapDescAllocator;

	void DescriptorHandle::SetCpuHandle(D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle)
	{
		m_cpuHandle = cpuHandle;
	}

	void DescriptorHandle::SetGpuHandle(D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle)
	{
		m_gpuHandle = gpuHandle;
	}

	DescriptorHandle::DescriptorHandle()
	{
		m_cpuHandle.ptr = GpuVirtualAddressUnknown;
		m_gpuHandle.ptr = GpuVirtualAddressUnknown;
	}

	DescriptorHandle::DescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) :
		m_cpuHandle(cpuHandle)
	{
		m_gpuHandle.ptr = GpuVirtualAddressUnknown;
	}

	DescriptorHandle::DescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle) :
		m_cpuHandle(cpuHandle), m_gpuHandle(gpuHandle)
	{
	}

	D3D12_CPU_DESCRIPTOR_HANDLE& DescriptorHandle::GetCpuHandle()
	{
		return m_cpuHandle;
	}

	D3D12_GPU_DESCRIPTOR_HANDLE& DescriptorHandle::GetGpuHandle()
	{
		return m_gpuHandle;
	}

	bool DescriptorHandle::IsNull()
	{
		return m_cpuHandle.ptr == GpuVirtualAddressUnknown;
	}

	bool DescriptorHandle::IsShaderVisible() const
	{
		return m_gpuHandle.ptr != GpuVirtualAddressUnknown;
	}

	void DescriptorHandle::operator+=(int offsetScaledByDescriptorSize)
	{
		if (m_cpuHandle.ptr != GpuVirtualAddressUnknown)
		{
			m_cpuHandle.ptr += offsetScaledByDescriptorSize;
		}
		if (m_gpuHandle.ptr != GpuVirtualAddressUnknown)
		{
			m_gpuHandle.ptr += offsetScaledByDescriptorSize;
		}
	}

	DescriptorHandle DescriptorHandle::operator+(int offsetScaledByDescriptorSize)
	{
		DescriptorHandle t = *this;
		t += offsetScaledByDescriptorSize;
		return t;
	}






	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	DescriptorHeapAllocator* DescriptorHeapAllocator::m_uniqueObj;

	//ID3D12DescriptorHeap* DescriptorHeapAllocator::RequestNewHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, ID3D12Device* device)
	void DescriptorHeapAllocator::RequestNewHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, ID3D12Device* device)
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.NodeMask = 1;
		desc.NumDescriptors = m_NumDescriptorPerHeap;
		desc.Type = type;

		ComPtr<ID3D12DescriptorHeap> cp_heap;
		ThrowIfFailed(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&cp_heap)));
		m_cp_descriptorHeapPool.emplace_back(cp_heap);
		ThrowIfFailed(cp_heap.As(&m_currentHeap[type]));
	}

	DescriptorHeapAllocator::~DescriptorHeapAllocator()
	{
		DestoryAll();
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapAllocator::Allocate(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t count)
	{
		ID3D12Device* device = r_graphicsCard[0]->GetDevice();
		lock_guard<mutex> lock(m_mutex);
		if (m_currentHeap == nullptr || m_remainingFreeHandles[type] < count)
		{
			// m_currentHeap[type] = RequestNewHeap(type, device);
			RequestNewHeap(type, device);
			m_currentHandle[type] = m_currentHeap[type]->GetCPUDescriptorHandleForHeapStart();
			m_remainingFreeHandles[type] = m_NumDescriptorPerHeap;

			(m_descriptorSize[type] == 0) ? m_descriptorSize[type] = device->GetDescriptorHandleIncrementSize(type) : (0);
		}

		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = m_currentHandle[type];
		m_currentHandle[type].ptr += count * m_descriptorSize[type];
		m_remainingFreeHandles[type] -= count;
		return cpuHandle;
	}

	void DescriptorHeapAllocator::DestoryAll()
	{
		m_cp_descriptorHeapPool.clear();
	}

	DescriptorHeapAllocator* DescriptorHeapAllocator::GetInstance()
	{
		if (m_uniqueObj == nullptr)
		{
			m_uniqueObj = new DescriptorHeapAllocator();
		}
		return m_uniqueObj;
	}
}