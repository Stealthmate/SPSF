#ifndef SPSF_CONFIG_H
#define SPSF_CONFIG_H

#ifdef _WIN32
	#ifdef SPSF_BUILD
		#ifdef SPSF_BUILD_SHARED
			#define SPSF_API __declspec(dllexport)
		#else
			#define SPSF_API __declspec(dllimport)
		#endif
	#else
		#define SPSF_API
	#endif
#else
	#define SPSF_API
#endif

#include <memory>

namespace SPSF
{
	const unsigned long long SPSF_HEADER_ULONG = *reinterpret_cast<const unsigned long long*>("SPSFONTS");

	enum ColorType : int8_t
	{
		CT_NULL = 0,
		GRAYSCALE = 1,
		GRAYSCALE_ALPHA = 2,
		RED_GREEN_BLUE = 3,
		RED_GREEN_BLUE_ALPHA = 4
	};

	enum BitDepth : int8_t
	{
		BD_NULL = 0,
		BD_1 = 1,
		BD_2 = 2,
		BD_4 = 4,
		BD_8 = 8
	};

	template <typename T>
	struct word
	{
		static constexpr int MAX_BITS = sizeof T * 8;
		T* ptr = 0;
		int used = 0;
		int available = 0;
	};

	typedef uint8_t byte;
	typedef uint32_t dword;
	typedef std::unique_ptr<byte> owned_data;
}

#endif //SPSF_CONFIG_H