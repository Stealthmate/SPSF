#include "../src/SPSF.hpp"
#include <iostream>
#include <fstream>

using namespace SPSF;
std::string hex(unsigned char b);

template <typename T>
T* compressElement(int n_components, int bits_per_component_provided, int bits_per_component_new, T* data, T* new_data_buff = 0)
{
	int new_size = std::ceil((n_components*  bits_per_component_new) / (double)word<T>::MAX_BITS);
	T* new_data = new_data_buff;
	if (!new_data) new_data = new T[new_size];
	word<T> curr;
	curr.ptr = data;
	std::fill(new_data, new_data + new_size, 0);
	curr.used = word<T>::MAX_BITS;
	curr.available = word<T>::MAX_BITS - curr.used;

	word<T> i_write;
	i_write.ptr = new_data;
	i_write.used = 0;
	i_write.available = word<T>::MAX_BITS;


	unsigned long int mask_prov = pow(2, bits_per_component_provided) - 1;
	unsigned long int mask_new = pow(2, bits_per_component_new) - 1;

	for (int i = 0; i <= n_components - 1;i++)
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

		T shiftedb = *curr.ptr >> shift_prov;
		T maskedshiftedb = (shiftedb & mask_prov) & mask_new;

		if (i_write.available < bits_per_component_new)
		{
			unsigned long long mask_first = pow(2, i_write.available) - 1;
			unsigned long long mask_second = (unsigned long long )(pow(2, bits_per_component_new - i_write.available) - 1) << (i_write.available);
			T maskedb = maskedshiftedb & mask_first;
			T reshiftedb = maskedb << shift_new;
			*i_write.ptr = *i_write.ptr | reshiftedb;
			i_write.ptr++;

			maskedb = (maskedshiftedb & mask_second) >> i_write.available;
			reshiftedb = maskedb;

			*i_write.ptr = reshiftedb;
			i_write.used = bits_per_component_new - i_write.available;
			i_write.available = word<T>::MAX_BITS - i_write.used;
		}
		else
		{
			T reshiftedb = maskedshiftedb << shift_new;

			*i_write.ptr = *i_write.ptr | reshiftedb;
			i_write.used += bits_per_component_new;
			i_write.available = word<T>::MAX_BITS - i_write.used;
		}

		curr.available += bits_per_component_provided;
		curr.used = word<T>::MAX_BITS - curr.available;
	}

	return new_data;
}

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

template <typename T = byte>
void printData(int n, int m, T* data)
{
	for (int i = 0;i <= n - 1;i++)
	{
		for (int j = 0;j <= m - 1;j++)
		{
			std::cout << hex(data[m*i + j]) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int main()
{
	byte data[] = {
		0x0B, 0x0D, 0x0F,
		0x0A, 0x0B, 0x0C,
		0x0D, 0x0E, 0x0F
	};

	byte compressedData[6];
	byte fullyCompressedData[6];
	byte decompressedData[9];

	int n_elements = 3;
	int n_comp = 3;
	int bits_pre = 8;
	int bits_post = 4;


	std::cout << "Initial data:\n";
	printData(3, 3, data);

	for (int i = 0;i <= 2;i++)
	{
		int offset_pre = i * bits_pre *  n_comp;
		int offset_post = i * 2 * 8;
		if (i > 0)
		{
			offset_pre += offset_pre % 8;
			offset_post += offset_post % 8;
		}
		offset_pre /= 8;
		offset_post /= 8;

		std::cout << (uintptr_t)data + offset_pre << " "
			<< (uintptr_t)compressedData + offset_post << std::endl;

		compressElement<byte>(n_comp, bits_pre, bits_post,
			data + offset_pre,
			compressedData + offset_post);
	}

	std::cout << "Compressed Data:\n";
	printData(6, 1, compressedData);

	compressElement<int16_t>(3, 16, 12, reinterpret_cast<int16_t*>(compressedData), reinterpret_cast<int16_t*>(fullyCompressedData));
	printData(5, 1, fullyCompressedData);
	/*std::cout << "Decompressed data:\n";
	unsigned char * uncompressed_data = compressElement(n_comp, bits_post, bits_pre, compressed_data);
	for (int i = 0;i <= data_size_init - 1;i++) std::cout << hex(uncompressed_data[i]) << " ";
	std::cout << std::endl;*/

}