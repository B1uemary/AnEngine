#include"RenderCore.h"

namespace RenderCore
{
	void GraphicCard::CreateDevice()
	{
		UINT dxgiFactoryFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
		ComPtr<ID3D12Debug> d3dDebugController;
		if (D3D12GetDebugInterface(IID_PPV_ARGS(&d3dDebugController)))
		{
			d3dDebugController->EnableDebugLayer();
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
#endif
		// ����Debugģʽ

		ComPtr<IDXGIFactory4> dxgiFactory;
		CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&dxgiFactory));

		ComPtr<IDXGIAdapter1> hardwareAdapter;
		GetHardwareAdapter(dxgiFactory.Get(), &hardwareAdapter);
		D3D12CreateDevice(hardwareAdapter.Get(), RenderCore::MinD3DFeatureLevel, IID_PPV_ARGS(&device));

		if (device.Get() == nullptr)
		{
			ComPtr<IDXGIAdapter> warpAdapter;
			dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter));
			D3D12CreateDevice(warpAdapter.Get(), RenderCore::MinD3DFeatureLevel, IID_PPV_ARGS(&device));
		}

		device->SetStablePowerState(stableFlag);

#if defined(DEBUG) || defined(_DEBUG)
		ID3D12InfoQueue* compInfoQueue;
		device->QueryInterface(IID_PPV_ARGS(&compInfoQueue));
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };

		// ͨ��ID�����������Ϣ��
		D3D12_MESSAGE_ID denyMessageIds[] =
		{
			// ��������������δ��ʼ����������ʱ����ʹ��ɫ�������ʣ�Ҳ�ᷢ������������������������л���ɫ�������ж����Ǹı�̫��Ĵ��롢���°�����Դ��
			D3D12_MESSAGE_ID_INVALID_DESCRIPTOR_HANDLE,
			// ����ɫ���������ȾĿ���������ɫ�������������RGBд��R10G10B10A2������ʱ������Alphaʱ������
			D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_PS_OUTPUT_RT_OUTPUT_MISMATCH,
			// ��ʹ����ɫ��������ȱ�ٵ���������Ҳ������������δ��ʱ��������ͬ����ɫ��֮�乲��ĸ�ǩ����������Ҫ��ͬ���͵���Դ��
			D3D12_MESSAGE_ID_COMMAND_LIST_DESCRIPTOR_TABLE_NOT_SET,

			(D3D12_MESSAGE_ID)1008,
		};
		D3D12_INFO_QUEUE_FILTER newFilter = {};
		newFilter.DenyList.NumSeverities = _countof(severities);
		newFilter.DenyList.pSeverityList = severities;
		newFilter.DenyList.NumIDs = _countof(denyMessageIds);
		newFilter.DenyList.pIDList = denyMessageIds;

		compInfoQueue->PushStorageFilter(&newFilter);
		compInfoQueue->Release();
#endif
		if (SUCCEEDED(device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &featureDataOptions, sizeof(featureDataOptions))))
		{
			if (featureDataOptions.TypedUAVLoadAdditionalFormats)
			{
				D3D12_FEATURE_DATA_FORMAT_SUPPORT formatSupport =
				{
					DXGI_FORMAT_R11G11B10_FLOAT,
					D3D12_FORMAT_SUPPORT1_NONE,
					D3D12_FORMAT_SUPPORT2_NONE
				};
				if (SUCCEEDED(device->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &formatSupport, sizeof(formatSupport))) && (formatSupport.Support2 & D3D12_FORMAT_SUPPORT2_UAV_TYPED_LOAD) != 0)
				{
					isTypedUAVLoadSupport_R11G11B10_FLOAT = true;
				}
				formatSupport.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
				if (SUCCEEDED(device->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &formatSupport, sizeof(formatSupport))) && (formatSupport.Support2 & D3D12_FORMAT_SUPPORT2_UAV_TYPED_LOAD) != 0)
				{
					isTypedUAVLoadSupport_R16G16B16A16_FLOAT = true;
				}
			}
		}
	}

	void GraphicCard::CreateCommandQueue(D3D12_COMMAND_LIST_TYPE type)
	{
		switch (type)
		{
		case D3D12_COMMAND_LIST_TYPE_DIRECT:
		{
			renderCommandQueue.Initialize(device.Get());
			break;
		}
		case D3D12_COMMAND_LIST_TYPE_BUNDLE:
		{
			break;
		}
		case D3D12_COMMAND_LIST_TYPE_COMPUTE:
		{
			//D3D12_COMMAND_QUEUE_DESC queueDesc = {};
			//queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			//queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
			//device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(computeCommandQueue.GetAddressOf()));
			computeCommandQueue.Initialize(device.Get(), D3D12_COMMAND_LIST_TYPE_COMPUTE);
			break;
		}
		case D3D12_COMMAND_LIST_TYPE_COPY:
		{
			copyCommandQueue.Initialize(device.Get(), D3D12_COMMAND_LIST_TYPE_COPY);
			break;
		}
		default:
			break;
		}
	}

	const ID3D12Device2* GraphicCard::GetDevice() const
	{
		return device.Get();
	}

	void GraphicCard::IsStable(bool isStable)
	{
		stableFlag = isStable;
	}

	const ID3D12CommandQueue* GraphicCard::GetCommandQueue(D3D12_COMMAND_LIST_TYPE type) const
	{
		switch (type)
		{
		case D3D12_COMMAND_LIST_TYPE_DIRECT:
		{
			return renderCommandQueue.GetCommandQueue();
			break;
		}
		case D3D12_COMMAND_LIST_TYPE_BUNDLE:
		{
			break;
		}
		case D3D12_COMMAND_LIST_TYPE_COMPUTE:
		{
			return computeCommandQueue.GetCommandQueue();
			break;
		}
		case D3D12_COMMAND_LIST_TYPE_COPY:
		{
			return copyCommandQueue.GetCommandQueue();
			break;
		}
		default:
			break;
		}
		return nullptr;
	}

	GraphicCard::GraphicCard() :
		stableFlag(false)
	{
	}

	GraphicCard::GraphicCard(const GraphicCard & graphicCard)
	{
	}

	void GraphicCard::Initialize(bool compute, bool copy)
	{
		CreateDevice();
		CreateCommandQueue();
		if (compute) CreateCommandQueue(D3D12_COMMAND_LIST_TYPE_COMPUTE);
		if (copy) CreateCommandQueue(D3D12_COMMAND_LIST_TYPE_COPY);
	}
}

namespace RenderCore
{
	void CommandQueue::Initialize(ID3D12Device2* device, D3D12_COMMAND_LIST_TYPE type)
	{
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
		device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(commandQueue.GetAddressOf()));
	}

	void CommandQueue::Release()
	{
	}

	const ID3D12CommandQueue* CommandQueue::GetCommandQueue() const
	{
		return nullptr;
	}

	D3D12_COMMAND_LIST_TYPE CommandQueue::GetType()
	{
		return D3D12_COMMAND_LIST_TYPE();
	}
}

namespace RenderCore
{
	vector<GraphicCard> r_renderCore;
	ComPtr<IDXGISwapChain1> r_swapChain = nullptr;

	void InitializeRender(int graphicCardCount, bool isStable)
	{
		while (graphicCardCount--)
		{
			GraphicCard aRender;
			aRender.IsStable(isStable);
			aRender.Initialize();
			r_renderCore.push_back(aRender);
		}
	}

	void InitializeSwapChain()
	{
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.BufferCount = SwapChainBufferCount;
	}
}