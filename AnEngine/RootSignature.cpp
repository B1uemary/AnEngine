#include "RootSignature.h"
#include "RenderCore.h"
namespace AnEngine::RenderCore
{
	RootParameter::RootParameter()
	{
		m_rootParam.ParameterType = (D3D12_ROOT_PARAMETER_TYPE)0xFFFFFFFF;
	}

	RootParameter::~RootParameter()
	{
		Clear();
	}

	void RootParameter::Clear()
	{
		if (m_rootParam.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
			delete[] m_rootParam.DescriptorTable.pDescriptorRanges;

		m_rootParam.ParameterType = (D3D12_ROOT_PARAMETER_TYPE)0xFFFFFFFF;
	}

	void RootParameter::InitAsConstants(uint32_t Register, uint32_t NumDwords, D3D12_SHADER_VISIBILITY Visibility)
	{
		m_rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		m_rootParam.ShaderVisibility = Visibility;
		m_rootParam.Constants.Num32BitValues = NumDwords;
		m_rootParam.Constants.ShaderRegister = Register;
		m_rootParam.Constants.RegisterSpace = 0;
	}

	void RootParameter::InitAsConstantBuffer(uint32_t Register, D3D12_SHADER_VISIBILITY Visibility)
	{
		m_rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		m_rootParam.ShaderVisibility = Visibility;
		m_rootParam.Descriptor.ShaderRegister = Register;
		m_rootParam.Descriptor.RegisterSpace = 0;
	}

	void RootParameter::InitAsBufferSRV(uint32_t Register, D3D12_SHADER_VISIBILITY Visibility)
	{
		m_rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
		m_rootParam.ShaderVisibility = Visibility;
		m_rootParam.Descriptor.ShaderRegister = Register;
		m_rootParam.Descriptor.RegisterSpace = 0;
	}

	void RootParameter::InitAsBufferUAV(uint32_t Register, D3D12_SHADER_VISIBILITY Visibility)
	{
		m_rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
		m_rootParam.ShaderVisibility = Visibility;
		m_rootParam.Descriptor.ShaderRegister = Register;
		m_rootParam.Descriptor.RegisterSpace = 0;
	}

	void RootParameter::InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE Type, uint32_t Register, uint32_t Count, D3D12_SHADER_VISIBILITY Visibility)
	{
		InitAsDescriptorTable(1, Visibility);
		SetTableRange(0, Type, Register, Count);
	}

	void RootParameter::InitAsDescriptorTable(uint32_t RangeCount, D3D12_SHADER_VISIBILITY Visibility)
	{
		m_rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		m_rootParam.ShaderVisibility = Visibility;
		m_rootParam.DescriptorTable.NumDescriptorRanges = RangeCount;
		m_rootParam.DescriptorTable.pDescriptorRanges = new D3D12_DESCRIPTOR_RANGE[RangeCount];
	}

	void RootParameter::SetTableRange(uint32_t RangeIndex, D3D12_DESCRIPTOR_RANGE_TYPE Type, uint32_t Register, uint32_t Count, uint32_t Space)
	{
		D3D12_DESCRIPTOR_RANGE* range = const_cast<D3D12_DESCRIPTOR_RANGE*>(m_rootParam.DescriptorTable.pDescriptorRanges + RangeIndex);
		range->RangeType = Type;
		range->NumDescriptors = Count;
		range->BaseShaderRegister = Register;
		range->RegisterSpace = Space;
		range->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}
}

namespace AnEngine::RenderCore
{
	RootSignature::RootSignature()
	{
		var device = r_graphicsCard[0]->GetDevice();

		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));

		device->CreateRootSignature(r_graphicsCard[0]->GetNodeNum(), signature->GetBufferPointer(), 
			signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature_cp));
	}

	void RootSignature::Reset()
	{
	}

	ID3D12RootSignature* RootSignature::GetRootSignature()
	{
		return m_rootSignature_cp.Get();
	}
	ID3D12RootSignature** RootSignature::operator&() throw()
	{
		return &m_rootSignature_cp;
	}
}