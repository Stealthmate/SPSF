#ifndef SPSF_PACKERS_H
#define SPSF_PACKERS_H

#include "Config.hpp"

namespace SPSF
{
	SPSF_API byte* pack(int n_elements, int element_input_size, int element_output_size, byte* input_data, byte* output_data = 0);
	SPSF_API byte* unpack(int n_elements, int element_input_size, int element_output_size, byte* input_data, byte* output_data = 0);

}

#endif //SPSF_PACKERS_H