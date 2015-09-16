#include "../src/SPSF.hpp"
#include "../src/Packers.hpp"
#include <iostream>
#include <fstream>

using namespace SPSF;
std::string hex(unsigned char b);

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
		0x01, 0x02, 0x03, 
		0x04, 0x05, 0x06,
		0x07, 0x08, 0x09
	};

	constexpr int n_elements = 3;
	constexpr int n_comp = 3;

	constexpr int bits_pre_comp = 8;
	constexpr int bits_post_comp = 4;
	constexpr int bits_pre_elem = n_comp * bits_pre_comp;
	constexpr int bits_post_elem = n_comp * bits_post_comp;

	constexpr int stride_pre = bits_pre_comp * n_comp + ((bits_pre_comp * n_comp) % 8);
	constexpr int stride_post = bits_post_comp * n_comp + ((bits_post_comp * n_comp) % 8);

	constexpr int elem_size = bits_post_elem / 8;
	constexpr int element_has_padding = bits_post_elem % 8 != 0;
	constexpr int elem_data_size = n_elements * elem_size + n_elements * element_has_padding;

	byte compressedComp[n_elements * (stride_post / 8)];
	byte compressedElem[elem_data_size];
	byte new_data[n_elements * (bits_pre_elem / 8) + (bool) (bits_pre_elem % 8 != 0)];

	std::cout << "Initial data: " << sizeof data << " bytes\n";
	printData(n_elements, n_comp, data);
	for (int i = 0;i <= n_elements - 1;i++)
	{
		int offset_pre = i * stride_pre;
		int offset_post = i * stride_post;
		offset_pre /= 8;
		offset_post /= 8;

		//std::cout << offset_post << " " << (uintptr_t)data + offset_pre << " " << (uintptr_t)compressedComp + offset_post << std::endl;

		pack(n_comp, bits_pre_comp, bits_post_comp,
			data + offset_pre,
			compressedComp + offset_post);
	}

	std::cout << "Pack components: " << sizeof compressedComp << " bytes\n";
	printData(n_elements, n_elements * stride_post / 8, compressedComp);

	pack(4, 16, 12, compressedComp, compressedElem);

	std::cout << "Pack elements: " << sizeof compressedElem << " bytes\n";
	printData(1, elem_data_size, compressedElem);

	unpack(n_elements * n_comp, 4, 8, compressedElem, new_data);
	std::cout << "Unpack: " << sizeof new_data << " bytes " << std::endl;
	printData(n_elements, n_comp, new_data);
}