#pragma once
#ifndef __ONWIND_H__
#define __ONWIND_H__

#include<iostream>
#include<fstream>
#include<ctime>
#include<cstring>
#include<cmath>
#include<cstdlib>
#include<algorithm>
#include<memory>
#include<string>
#include<vector>
#include<functional>

#define var auto
#define let auto

#ifdef _WIN64

#include<windows.h>
#include<wrl.h>
#include<tchar.h>

#ifdef UNICODE

#define SOLUTION_DIR L"C:\\Users\\PC\\Documents\\Code\\VSProject\\AnEngine"

#define Strcpy(a,b) wcscpy_s(a, b)
#define ERRORBLOCK(a) MessageBox(NULL, ToLPCWSTR(a), _T("Error"), 0)
#define __FasterFunc(func) inline func __vectorcall

#if defined _DEBUG || defined DEBUG
#define ERRORBREAK(a) {\
						ERRORBLOCK(a); \
						throw std::exception(); \
					  }
#else
#define ERRORBREAK(a) (a)
#endif // _DEBUG || DEBUG


inline LPCWSTR ToLPCWSTR(std::string& orig)
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t) *(origsize - 1));
	mbstowcs_s(nullptr, wcstring, origsize, orig.c_str(), _TRUNCATE);
	return wcstring;
}

inline LPCWSTR ToLPCWSTR(char* l)
{
	std::string orig(l);
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t) *(orig.length() - 1));
	mbstowcs_s(nullptr, wcstring, origsize, orig.c_str(), _TRUNCATE);
	return wcstring;
}

inline LPCWSTR ToLPCWSTR(const char* l)
{
	std::string orig(l);
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t) *(orig.length() - 1));
	mbstowcs_s(nullptr, wcstring, origsize, orig.c_str(), _TRUNCATE);
	return wcstring;
}

inline void GetAssetsPath(_Out_writes_(pathSize) WCHAR* path, UINT pathSize)
{
	if (path == nullptr)
	{
		throw std::exception();
	}

	DWORD size = GetModuleFileName(nullptr, path, pathSize);
	if (size == 0 || size == pathSize)
	{
		// Method failed or path was truncated.
		throw std::exception();
	}

	WCHAR* lastSlash = wcsrchr(path, L'\\');
	if (lastSlash)
	{
		*(lastSlash + 1) = L'\0';
	}
}

inline HRESULT ReadDataFromFile(LPCWSTR filename, byte** data, UINT* size)
{
	using namespace Microsoft::WRL;

	CREATEFILE2_EXTENDED_PARAMETERS extendedParams = {};
	extendedParams.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
	extendedParams.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
	extendedParams.dwFileFlags = FILE_FLAG_SEQUENTIAL_SCAN;
	extendedParams.dwSecurityQosFlags = SECURITY_ANONYMOUS;
	extendedParams.lpSecurityAttributes = nullptr;
	extendedParams.hTemplateFile = nullptr;

	Wrappers::FileHandle file(CreateFile2(filename, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, &extendedParams));
	if (file.Get() == INVALID_HANDLE_VALUE)
	{
		throw std::exception();
	}

	FILE_STANDARD_INFO fileInfo = {};
	if (!GetFileInformationByHandleEx(file.Get(), FileStandardInfo, &fileInfo, sizeof(fileInfo)))
	{
		throw std::exception();
	}

	if (fileInfo.EndOfFile.HighPart != 0)
	{
		throw std::exception();
	}

	*data = reinterpret_cast<byte*>(malloc(fileInfo.EndOfFile.LowPart));
	*size = fileInfo.EndOfFile.LowPart;

	if (!ReadFile(file.Get(), *data, fileInfo.EndOfFile.LowPart, nullptr, nullptr))
	{
		throw std::exception();
	}

	return S_OK;
}

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw std::exception("一个奇怪的错误");
	}
}

inline void ThrowIfFailed(HRESULT hr, std::function<void(void)> f)
{
	if (FAILED(hr))
	{
		f();
		throw std::exception("一个奇怪的错误");
	}
}

template <typename T>
inline T* SafeAcquire(T* newObject)
{
	if (newObject != nullptr) ((IUnknown*)newObject)->AddRef();
	return newObject;
}

#else
#define Strcpy(a,b) wcscpy(a,b);
#define SOLUTION_DIR "C:\\Users\\PC\\Documents\\Code\\VSProject\\AnEngine"
#endif // !UNICODE
#else
#ifndef UNICODE
#define Strcpy(a,b) strcpy_s(a,b);
#define stl(l) (l.c_str())

#endif
#endif // !_WIN64

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

template<typename T>
struct Range
{
	T m_maxn, m_minn;

	Range(const T& _minn, const T& _maxn) :maxn(_maxn), minn(_minn)
	{
		if (m_minn > m_maxn) throw std::exception("Min argument is greater than max argument");
	}

	bool Has(T& value)
	{
		if (value < minn) return false;
		if (value > maxn) return false;
		return true;
	}
};

__FasterFunc(void) Randomize()
{
	srand((unsigned)time(nullptr));
}

__FasterFunc(int) Random(int a)
{
	return rand() % a;
}

__FasterFunc(float) Random()
{
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

__FasterFunc(int) Random(int a, int b)
{
	int c = rand() % b;
	while (c < a) c = rand() % b;
	return c;
}

__FasterFunc(float) Random(float a, float b)
{
	float scale = static_cast<float>(rand()) / RAND_MAX;
	float range = b - a;
	return scale * range + a;
}

struct NonCopyable
{
	NonCopyable() = default;
	NonCopyable(const NonCopyable&) = delete;
	~NonCopyable() = default;

	NonCopyable& operator=(const NonCopyable&) = delete;
};

template<typename T>
class Singleton : public NonCopyable
{
	inline static T* m_uniqueObj = nullptr;

public:
	static T* GetInstance()
	{
		if (m_uniqueObj == nullptr)
		{
			m_uniqueObj = new T();
		}
		return m_uniqueObj;
	}
};

#endif // !__ONWIND_H__