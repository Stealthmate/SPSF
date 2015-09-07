#include "SPSF_Item.hpp"

using namespace SPSF;

SPSF_Item::SPSF_Item() : data()
{
	size = 0;
}

SPSF_Item::SPSF_Item(std::unique_ptr<byte> data, size_t size)
{
	this->data = std::move(data);
	this->size = size;
}

SPSF_Item::SPSF_Item(const SPSF_Item &item)
{
	size = item.size;
	if (size > 0)
	{
		try
		{
			data = std::make_unique<byte>(size);
		}
		catch (std::bad_alloc ba)
		{
			throw std::exception("Could not allocate memory for copying");
		}
		for (int i = 0;i <= size - 1;i++)
		{
			data.get()[i] = item.data.get()[i];
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
	data.reset();
	data = std::move(_copy);
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
	return data.get();
}

byte* SPSF_Item::getData()
{
	return data.get();
}

std::istream& SPSF::operator>>(std::istream &in, SPSF_Item &item)
{
	if (!in.good()) throw std::exception("SPSF: Stream state is not good!\n");
	item.data = std::make_unique<byte>(item.size);
	std::cout << "Size of item " << item.size << std::endl;
	in.read(reinterpret_cast<char*>(item.data.get()), item.size);
	return in;
}

std::ostream& SPSF::operator<<(std::ostream &out, const SPSF_Item &item)
{
	if (!out.good()) throw std::exception("SPSF: Stream state is not good!\n");
	out.write(reinterpret_cast<char*>(item.data.get()), item.getSizeInBytes());
	out.flush();
	return out;
}
