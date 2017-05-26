#pragma once
#ifndef __CUBE_H__
#define __CUBE_H__

#include"onwind.h"
#include"DX.h"
#include"RenderCore.h"
#include"MCamera.h"
#include"DTimer.h"
using namespace RenderCore;

class DrawCube :public D3D12AppBase, public D3D12Base
{
	struct VertexPositionColor
	{
		XMFLOAT4 position;
		XMFLOAT3 color;
	};

	struct VertexPositionUV
	{
		XMFLOAT4 position;
		XMFLOAT2 uv;
	};

	struct DrawConstantBuffer
	{
		XMMATRIX worldViewProjection;
	};

	enum RootParameters : UINT32
	{
		RootParameterUberShaderCB = 0,
		RootParameterCB,
		RootParameterSRV,
		RootParametersCount
	};

	CD3DX12_VIEWPORT viewport;
	CD3DX12_RECT scissorRect;
	ComPtr<IDXGISwapChain3> swapChain;
	ComPtr<ID3D12Device> device;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<ID3D12CommandAllocator> commandAllocators[DefaultFrameCount];
	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	ComPtr<ID3D12DescriptorHeap> srvHeap;
	ComPtr<ID3D12Resource> renderTargets[DefaultFrameCount];
	ComPtr<ID3D12Resource> intermediateRenderTarget;
	ComPtr<ID3D12RootSignature> rootSignature;
	UINT rtvDescriptorSize;
	UINT srvDescriptorSize;

	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12PipelineState> pipelineState;

	UINT frameIndex;

	ComPtr<ID3D12Fence> fence;
	HANDLE fenceEvent;
	UINT fenceValues[DefaultFrameCount];

	ComPtr<ID3D12Resource> vertexBuffer;
	ComPtr<ID3D12Resource> vertexIndexBuffer;
	D3D12_VERTEX_BUFFER_VIEW cubeVbv;
	D3D12_INDEX_BUFFER_VIEW cubeIbv;
	// Ӧ�ó�����Դ

	vector<VertexPositionColor> vertex;

	MCamera camera;
	XMMATRIX projectionMatrix;
	DTimer timer;

public:
	DrawCube(const HWND _hwnd, const UINT _width, const UINT _height);
	~DrawCube();


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

#endif // !__CUBE_H__
