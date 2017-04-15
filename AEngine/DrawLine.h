#pragma once
#ifndef __DRAWLINE_H__
#define __DRAWLINE_H__

#include"onwind.h"
#include"DX.h"
#include"Input.h"

class DrawLine :public D3D12AppBase, public D3D12Base
{
	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	UINT frameIndex;

	CD3DX12_VIEWPORT viewport;
	CD3DX12_RECT scissorRect;
	ComPtr<IDXGISwapChain3> swapChain;
	ComPtr<ID3D12Device> device;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<ID3D12CommandAllocator> commandAllocators[DefaultFrameCount];
	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	ComPtr<ID3D12DescriptorHeap> srvHeap;
	ComPtr<ID3D12Resource> renderTargets[DefaultFrameCount];
	ComPtr<ID3D12RootSignature> rootSignature;
	UINT rtvDescriptorSize;
	UINT srvDescriptorSize;

	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12PipelineState> pipelineState;

	ComPtr<ID3D12Fence> fence;
	HANDLE fenceEvent;
	UINT fenceValues[DefaultFrameCount];

	ComPtr<ID3D12Resource> vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	// Ӧ�ó�����Դ

	vector<Vertex> vertex;

public:
	DrawLine(const HWND _hwnd, const UINT _width, const UINT _height);
	~DrawLine();


	// ͨ�� D3D12AppBase �̳�
	virtual void OnInit() override;

	virtual void OnRelease() override;

	virtual void OnUpdate() override;

	virtual void OnRender() override;

	virtual void OnKeyDown(UINT8 key) override;

	virtual void OnKeyUp(UINT8 key) override;


	// ͨ�� D3D12Base �̳�
	virtual void InitializePipeline() override;

	virtual void InitializeAssets() override;

	virtual void WaitForGpu() override;

	virtual void PopulateCommandList() override;

	virtual void MoveToNextFrame() override;

	virtual void WaitForRenderContext() override;

};
#endif // !__DRAWLINE_H__