#include "SPSF_Item.hpp"
#include "Packers.hpp"
#include <string>

using namespace SPSF;
static char hexarr[] = {
	'0', '1', '2', '3', '4',
	'5', '6', '7', '8', '9',
	'A', 'B', 'C', 'D', 'E', 'F'
};

static std::string hex(unsigned char b)
{
	unsigned char ub = b;
	std::string sb = "0x";
	sb += hexarr[ub / 16];
	sb += hexarr[ub % 16];
	return  sb;
}
SPSF_Item SPSF_Item::createItemFromData(
	byte* data,
	int32_t n_elements,
	BitDepth bd_internal,
	BitDepth bd_provided,
	ColorType ct_internal,
	ColorType ct_provided,
	byte* buffer
	)
{
	SPSF_Item item;
	item.data = buffer;
	item.size = std::ceil(n_elements * ct_internal * bd_internal / 8.0);
	int32_t element_stride_internal = std::ceil(ct_internal * bd_internal / 8.0);
	int32_t element_stride_provided = std::ceil(ct_provided * bd_provided / 8.0);
	std::cout << n_elements << " elements with ct_prov " << ct_provided << " and ct_int " << ct_internal
		<< " and bd_prov " << bd_provided << " and bd_int " << bd_internal
		<< " and el_str_int " << element_stride_internal
		<< " and el_str_prov " << element_stride_provided << std::endl;


	std::unique_ptr<byte[]> temp_pack_buffer = std::make_unique<byte[]>(item.size);
	for (int i = 0;i <= n_elements - 1;i++)
	{
		repack(ct_internal,
			std::ceil(bd_provided / 8.0),
			std::ceil(bd_internal / 8.0),
			data + (size_t)(i * element_stride_provided),
			temp_pack_buffer.get() + (size_t)(i * element_stride_internal));
		std::cout << hex(temp_pack_buffer[i]) << " " << hex(temp_pack_buffer[i+1]) << std::endl;
	}

	repack(n_elements, ct_provided * bd_provided, ct_internal * bd_internal, temp_pack_buffer.get(), item.data);

	return item;
}

SPSF_Item::SPSF_Item() : data()
{
	size = 0;
}

SPSF_Item::SPSF_Item(byte* data, size_t size)
{
	this->data = new byte[size];
	std::copy(data, data + size, this->data);
	this->size = size;
}

SPSF_Item::SPSF_Item(const SPSF_Item &item)
{
	size = item.size;
	if (size > 0)
	{
		data = new byte[size];
		for (int i = 0;i <= size - 1;i++)
		{
			data[i] = item.data[i];
		}
	}
}

SPSF_Item& SPSF_Item::operator=(const SPSF_Item &item)
{
	size = item.size;
	std::unique_ptr<byte[]> _copy;
	if (size > 0)
	{
		_copy = std::make_unique<byte[]>(size);
	}
	data = _copy.get();
	return *this;
}

int32_t SPSF_Item::getN_Elements() const
{
	return n_elements;
}

size_t SPSF_Item::getSizeInBytes() const
{
	return size;
}

const byte* SPSF_Item::getData() const
{
	return data;
}

byte* SPSF_Item::getData()
{
	return data;
}

std::istream& SPSF::operator>>(std::istream &in, SPSF_Item &item)
{
	if (!in.good()) throw std::exception("SPSF: Stream state is not good!\n");
	item.data = new byte[item.size];
	std::cout << "Size of item " << item.size << std::endl;
	in.read(reinterpret_cast<char*>(item.data), item.size);
	return in;
}

std::ostream& SPSF::operator<<(std::ostream &out, const SPSF_Item &item)
{
	if (!out.good()) throw std::exception("SPSF: Stream state is not good!\n");
	out.write(reinterpret_cast<char*>(item.data), item.getSizeInBytes());
	out.flush();
	return out;
}
