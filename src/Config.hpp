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

namespace SPSF
{

}

#endif //SPSF_CONFIG_H