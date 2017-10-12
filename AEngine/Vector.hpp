#pragma once
#ifndef __AENGEIN_VECTOR_HPP__
#define __AENGINE_VECTOR_HPP__

#include"../Utility/onwind.h"
#include<DirectXMath.h>
using namespace DirectX;

/*(http://blog.csdn.net/zjjzhaohang/article/details/68936082)
Ϊ��Ч���������DX12�д���XMVECTOR ʱ����һ������ĵ��÷�ʽ���Ա�����SSE/SSE2Ӳ�����١�
1��ǰ����XMVECTOR ��������ʱ��Ӧ��ʹ��FXMVECTOR
2�����ĸ�XMVECTOR������Ӧ��ʹ��GXMVECTOR
3�����塢����XMVECTOR������Ӧ��ʹ��HXMVECTOR
4��ʣ�µĶ���Ĳ���ʹ��CXMVECTOR
5�������д������������Ͳ��������������������������Ч
*/

namespace Math
{
	template<size_t n>
	class Vector
	{
		XMFLOAT2 a;
	public:
		Vector()
		{
			if (n > 4) throw std::exception("Too many number for a sse vector.");
			if (n <= 0) throw std::exception("Too few number for a sse vector.")
		}

		static inline XMVECTOR __vectorcall Cross(XMVECTOR v1, XMVECTOR v2)
		{
		}
	};

	class Vector2 :Vector<2>
	{
		XMFLOAT2 m_vector;
	public:
		Vector2() = default;
		Vector2(Vector2&& v) :m_vector(v.m_vector)
		{
		}
	};

	class Vector3 :Vector<3>
	{
		XMFLOAT3 m_vector;
	public:
		Vector3() = default;
		Vector3(Vector3&& v) :m_vector(v.m_vector)
		{
		}
	};


}

#endif // !__AENGEIN_VECTOR_H__
