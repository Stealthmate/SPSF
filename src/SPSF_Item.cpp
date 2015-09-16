#include "SPSF_Item.hpp"
#include "Packers.hpp"

using namespace SPSF;

SPSF_Item SPSF_Item::createItemFromData(
	byte* data,
	int32_t n_elements,
	BitDepth bd_internal,
	BitDepth bd_provided,
	ColorType ct_internal,
	ColorType ct_provided,
	std::unique_ptr<byte> buffer
	)
{
	SPSF_Item item;
	item.data = std::move(buffer);
	item.size = std::ceil(n_elements * ct_internal * bd_internal / 8.0);
	int32_t element_stride_internal = std::ceil(ct_internal * bd_internal / 8.0);
	int32_t element_stride_provided = std::ceil(ct_provided * bd_provided / 8.0);

	std::unique_ptr<byte> temp_pack_buffer = std::make_unique<byte>(item.size);

	for (int i = 0;i <= n_elements - 1;i++)
	{
		repack(ct_provided, 
			bd_provided, 
			bd_internal, 
			data + (byte)(i * std::ceil(bd_provided/8.0)), 
			temp_pack_buffer.get() + (byte)(i * std::ceil(bd_internal/8.0)));
	}

	repack(n_elements, ct_provided * bd_provided, ct_internal * bd_internal, temp_pack_buffer.get(), item.data.get());

	return item;
}

SPSF_Item::SPSF_Item() : data()
{
	size = 0;
}

SPSF_Item::SPSF_Item(byte* data, size_t size)
{
	this->data = data;
	this->size = size;
}

SPSF_Item::SPSF_Item(const SPSF_Item &item)
{
	size = item.size;
	if (size > 0)
	{
		try
		{
			data = new byte[size];
		}
		catch (std::bad_alloc ba)
		{
			throw std::exception("Could not allocate memory for copying");
		}
		for (int i = 0;i <= size - 1;i++)
		{
			data[i] = item.data[i];
		}
	}
}

SPSF_Item& SPSF_Item::operator=(const SPSF_Item &item)
{
	size = item.size;
	std::unique_ptr<byte> _copy;
	if (size > 0)
	{
		try
		{
			_copy = std::make_unique<byte>(size);
		}
		catch (std::bad_alloc ba)
		{
			throw std::exception("Could not allocate memory for copy assignment");
		}
	}
	delete [] data;
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
