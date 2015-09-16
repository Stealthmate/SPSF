#include "Packers.hpp"

#include <algorithm>

#include <iostream>
#include <string>

using namespace SPSF;

char hexarr[] = {
	'0', '1', '2', '3', '4',
	'5', '6', '7', '8', '9',
	'A', 'B', 'C', 'D', 'E', 'F'
};

std::string hex(unsigned char b)
{
	unsigned char ub = b;
	std::string sb = "0x";
	sb += hexarr[ub / 16];
	sb += hexarr[ub % 16];
	return  sb;
}

template<typename T> byte* actual_repack(int n_elements, int element_input_size_bits, int element_output_size_bits, byte* input_data, byte* output_data)
{
	int new_size = std::ceil((n_elements *  element_output_size_bits) / (double)word<T>::MAX_BITS);
	T* new_data = reinterpret_cast<T*>(output_data);
	if (!new_data) new_data = new T[new_size];

	word<T> curr;
	curr.ptr = reinterpret_cast<T*>(input_data);
	std::fill(new_data, new_data + new_size, 0);
	curr.used = word<T>::MAX_BITS;
	curr.available = word<T>::MAX_BITS - curr.used;

	word<T> i_write;
	i_write.ptr = new_data;
	i_write.used = 0;
	i_write.available = word<T>::MAX_BITS;

	unsigned long int mask_input = pow(2, element_input_size_bits) - 1;
	unsigned long int mask_output = pow(2, element_output_size_bits) - 1;

	for (int i = 0; i <= n_elements - 1;i++)
	{
		if (curr.available == word<T>::MAX_BITS)
		{
			curr.ptr++;
			curr.used = word<T>::MAX_BITS;
			curr.available = word<T>::MAX_BITS - curr.used;
		}

		if (i_write.available == 0)
		{
			i_write.ptr++;
			i_write.available = word<T>::MAX_BITS;
			i_write.used = 0;
		}

		int shift_prov = curr.available;
		int shift_new = i_write.used;

		T shifted_bits_input = *curr.ptr >> shift_prov;
		T masked_bits = (shifted_bits_input & mask_input) & mask_output;

		if (i_write.available < element_output_size_bits)
		{
			unsigned long long mask_first = pow(2, i_write.available) - 1;
			unsigned long long mask_second = (unsigned long long)(pow(2, element_output_size_bits - i_write.available) - 1) << (i_write.available);
			T maskedb = masked_bits & mask_first;
			T reshiftedb = maskedb << shift_new;
			*i_write.ptr = *i_write.ptr | reshiftedb;
			i_write.ptr++;

			maskedb = (masked_bits & mask_second) >> i_write.available;
			reshiftedb = maskedb;

			*i_write.ptr = reshiftedb;
			i_write.used = element_output_size_bits - i_write.available;
			i_write.available = word<T>::MAX_BITS - i_write.used;
		}
		else
		{
			T shifted_bits_output = masked_bits << shift_new;

			*i_write.ptr = *i_write.ptr | shifted_bits_output;
			i_write.used += element_output_size_bits;
			i_write.available = word<T>::MAX_BITS - i_write.used;
		}

		curr.available += element_input_size_bits;
		curr.used = word<T>::MAX_BITS - curr.available;
	}

	return reinterpret_cast<byte*>(new_data);
}

byte* SPSF::repack(int n_elements, int element_input_size, int element_output_size, byte* input_data, byte* output_data)
{
	int size = std::max(element_input_size, element_output_size);
	if (size <= 8) return actual_repack<uint8_t>(n_elements, element_input_size, element_output_size, input_data, output_data);
	else if (size <= 16) return actual_repack<uint16_t>(n_elements, element_input_size, element_output_size, input_data, output_data);
	else if (size <= 32) return actual_repack<uint32_t>(n_elements, element_input_size, element_output_size, input_data, output_data);
}

byte* SPSF::pack(int n_elements, int element_input_size, int element_output_size, byte* input_data, byte* output_data)
{
	if (element_input_size < element_output_size) throw std::exception("Cannot pack in bigger size\n");
	return repack(n_elements, element_input_size, element_output_size, input_data, output_data);
}

byte* SPSF::unpack(int n_elements, int element_input_size, int element_output_size, byte* input_data, byte* output_data)
{
	if (element_input_size > element_output_size) throw std::exception("Cannot unpack in smaller size\n");
	return repack(n_elements, element_input_size, element_output_size, input_data, output_data);
}