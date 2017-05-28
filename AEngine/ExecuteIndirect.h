#pragma once
#ifndef __EXECUTEINDIRECT_H__
#define __EXECUTEINDIRECT_H__

#include"DX.h"
#include"RenderCore.h"
using namespace RenderCore;

class ExecuteIndirect :public D3D12AppBase, public D3D12Base
{
	struct Vertex
	{
		XMFLOAT3 position;
	};
	struct SceneConstantBuffer
	{
		XMFLOAT4 velocity;
		XMFLOAT4 offset;
		XMFLOAT4 color;
		XMFLOAT4X4 projection;	// ͶӰ
		float padding[36];	// ������������256�ֽڶ��롣
	};
	struct CSRootConstants
	{
		float xOffset;
		float zOffset;
		float cullOffset;
		float commandCount;
	};	// ������ɫ���ĸ�������
	struct IndirectCommand
	{
		D3D12_GPU_VIRTUAL_ADDRESS cbv;
		D3D12_DRAW_ARGUMENTS drawArguments;
	};	// ���ݽṹƥ��ExecuteIndirect������ǩ����
	enum GraphicsRootParameters
	{
		Cbv,
		GraphicsRootParametersCount
	};	// ͼ�θ�ǩ������ƫ����
	enum ComputeRootParameters
	{
		SrvUavTable,
		RootConstants,			// �ṩ���������ζ�����޳�ƽ�����ɫ����Ϣ�ĸ�����
		ComputeRootParametersCount
	};	// �����ǩ������ƫ����
	enum HeapOffsets
	{
		CbvSrvOffset = 0,													// SRVָ����Ⱦ�߳�ʹ�õĳ�����������
		CommandsOffset = CbvSrvOffset + 1,									// SRVָ�����м�����
		ProcessedCommandsOffset = CommandsOffset + 1,						// UAV��¼ʵ����Ҫִ�е�����
		CbvSrvUavDescriptorCountPerFrame = ProcessedCommandsOffset + 1		// 2��SRV��1��UAV���ڼ�����ɫ��
	};	// CBV��SRV��UAV��������ƫ��

	std::vector<SceneConstantBuffer> constantBufferData;
	UINT8* pCbvDataBegin;
	// ÿ��������ÿ֡����Լ��ĳ�����������

	static const UINT triangleCount = 1024;
	static const UINT triangleResourceCount = triangleCount*DefaultFrameCount;
	static const UINT commandSizePerFrame;			// �����ڵ�������л������������εļ������Ĵ�С��
	static const UINT commandBufferCounterOffset;	// �ڴ����������uva��������ƫ������
	static const UINT computeThreadBlockSize = 128;	// Ӧ��compute.hlsl�е�ֵƥ�䡣
	static const float triangleHalfWidth;			// �����ζ���ʹ�õ�x��yƫ�ơ�
	static const float triangleDepth;				// �����ζ���ʹ�õ�zƫ�ơ�
	static const float cullingCutoff;				// ���ȿռ��еļ���ƽ��ġ�xƫ��[-1,1]��

	UINT frameIndex;

	D3D12_RECT cullingScissorRect;
	CD3DX12_VIEWPORT viewport;
	CD3DX12_RECT scissorRect;
	ComPtr<IDXGISwapChain3> swapChain;
	ComPtr<ID3D12Device> device;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	ComPtr<ID3D12DescriptorHeap> cbvSrvUavHeap;
	ComPtr<ID3D12Resource> renderTargets[DefaultFrameCount];
	ComPtr<ID3D12CommandAllocator> commandAllocators[DefaultFrameCount];
	UINT rtvDescriptorSize;
	UINT dsvDescriptorSize;
	UINT cbvSrvUavDescriptorSize;
	ComPtr<ID3D12RootSignature> rootSignature;

	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12PipelineState> pipelineState;
	ComPtr<ID3D12CommandSignature> commandSignature;

	ComPtr<ID3D12GraphicsCommandList> computeCommandList;
	ComPtr<ID3D12CommandQueue> computeCommandQueue;
	ComPtr<ID3D12CommandAllocator> computeCommandAllocators[DefaultFrameCount];
	ComPtr<ID3D12RootSignature> computeRootSignature;
	ComPtr<ID3D12PipelineState> computeState;

	ComPtr<ID3D12Fence> fence;
	ComPtr<ID3D12Fence> computeFence;
	UINT64 fenceValues[DefaultFrameCount];
	HANDLE fenceEvent;

	ComPtr<ID3D12Resource> vertexBuffer;
	ComPtr<ID3D12Resource> constantBuffer;
	ComPtr<ID3D12Resource> depthStencil;
	ComPtr<ID3D12Resource> commandBuffer;
	ComPtr<ID3D12Resource> processedCommandBuffers[DefaultFrameCount];
	ComPtr<ID3D12Resource> processedCommandBufferCounterReset;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	CSRootConstants csRootConstants;	// ������ɫ���ĳ���
	bool enableCulling;	// ������ɫ���Ƿ�Ԥ����������

	virtual void InitializePipeline();
	virtual void InitializeAssets();
	virtual void PopulateCommandList();
	virtual void WaitForGpu();
	virtual void MoveToNextFrame();
	virtual void WaitForRenderContext();

	static UINT AlignForUavCounter(UINT bufferSize);
	// ���ǽ�UAV�������������������ͬ�Ļ������У�������Ϊ�䴴��������64K��Դ�ѡ�
	// ������������4K�߽��϶��룬������������������������Ҫ����ʹ�ü������������ڻ������е���Чλ�á�

public:
	ExecuteIndirect(const HWND _hwnd, const UINT _width, const UINT _height, const wstring& _title);
	~ExecuteIndirect();

	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnRelease();

	virtual void OnKeyUp(UINT8 key);
	virtual void OnKeyDown(UINT8 key);
};

#endif // !__EXECUTEINDIRECT_H__