#include "../src/SPSF.hpp"
#include <iostream>
#include <fstream>

using namespace SPSF;
std::string hex(unsigned char b);

struct byte
{
	unsigned char *b;
	int bits_available;
	int bits_used;
};
unsigned char* compressElement(int n_components, int bits_per_component_provided, int bits_per_component_new, unsigned char* data)
{
	int new_size = std::ceil((n_components*  bits_per_component_new) / 8.0);
	unsigned char* new_data = new unsigned char[new_size];
	byte curr;
	curr.b = data;
	std::fill(new_data, new_data + new_size, 0);
	curr.bits_used = 8;
	curr.bits_available = 8 - curr.bits_used;

	byte i_write;
	i_write.b = new_data;
	i_write.bits_used = 0;
	i_write.bits_available = 8;
	std::cout << (unsigned int)*new_data << std::endl;


	unsigned int mask_prov = pow(2, bits_per_component_provided) - 1;
	unsigned int mask_new = pow(2, bits_per_component_new) - 1;

	for (int i = 0; i <= n_components - 1;i++)
	{
		if (curr.bits_available == 8)
		{
			curr.b++;
			curr.bits_used = 8;
			curr.bits_available = 8 - curr.bits_used;
		}

		if (i_write.bits_available == 0)
		{
			i_write.b++;
			i_write.bits_available = 8;
			i_write.bits_used = 0;
		}

		int shift_prov = curr.bits_available;
		int shift_new = i_write.bits_used;

		unsigned char shiftedb = *curr.b >> shift_prov;
		unsigned char maskedshiftedb = (shiftedb & mask_prov) & mask_new;
		unsigned char reshiftedb = maskedshiftedb << shift_new;
		std::cout << hex(*curr.b) << " " << hex(shiftedb) << " " << hex(maskedshiftedb)  << " " << hex(reshiftedb) << std::endl;

		//*i_write.b = *i_write.b | ((((*curr.b >> i * bits_per_component_provided) & bits_per_component_new) << i * bits_per_component_new));
		std::cout << "Prefinal: " << hex(*i_write.b) << std::endl;
		*i_write.b = *i_write.b | reshiftedb;
		std::cout << "Final: " << hex(*i_write.b) << std::endl;
		i_write.bits_used += bits_per_component_new;
		i_write.bits_available = 8 - i_write.bits_used;

		curr.bits_available += bits_per_component_provided;
		curr.bits_used = 8 - curr.bits_available;
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

int main()
{
	unsigned char data[] = {0x03, 0x02, 0x01, 0x02};
	unsigned char * compressed_data = compressElement(4, 8, 3, data);
	std::cout << hex(*compressed_data) << std::endl;

}