#include "SPSF.hpp"

#include <fstream>

using namespace SPSF;

SPSF_Item::SPSF_Item() : data()
{
	size = 0;
}

SPSF_Item::SPSF_Item(std::unique_ptr<char[]> data, size_t size)
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
			data = std::make_unique<char[]>(size);
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
	std::unique_ptr<char[]> _copy;
	if (size > 0)
	{
		try
		{
			_copy = std::make_unique<char[]>(size);
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

size_t SPSF_Item::getSize() const
{
	return size;
}

const char* SPSF_Item::arr() const
{
	return data.get();
}

char* SPSF_Item::arr() 
{
	return data.get();
}

SPSF_Lane::SPSF_Lane() : items()
{
	ct = ColorType::CT_NULL;
	bd = BitDepth::BD_NULL;
	item_width = 0;
	item_height = 0;
	n_Items = 0;
}

SPSF_Lane::SPSF_Lane(ColorType ct, BitDepth bd, int16_t item_width, int16_t item_height, std::vector<SPSF_Item> &&items)
{
	this->ct = ct;
	this->bd = bd;
	this->item_width = item_width;
	this->item_height = item_height;
	this->n_Items = items.size();
	this->items = items;

	for (int i = 0;i <= n_Items - 1;i++)
	{
		this->items[i].size = getItemSize();
	}
}

ColorType SPSF_Lane::getColorType() const
{
	return ct;
}

BitDepth SPSF_Lane::getBitDepth() const
{
	return bd;
}

int16_t SPSF_Lane::getItemWidth() const
{
	return item_width;
}

int16_t SPSF_Lane::getItemHeight() const
{
	return item_height;
}

int32_t SPSF_Lane::getN_Items() const
{
	return n_Items;
}

const std::vector<SPSF_Item>& SPSF_Lane::getItems() const
{
	return items;
} 

size_t SPSF_Lane::getItemSize() const
{
	size_t data_size = ct * bd * item_width * item_height;
	data_size = (data_size / 8) + ((bool)(data_size % 8));
	return data_size;
}

size_t SPSF_Lane::getSize() const
{
	size_t s = (sizeof ct + sizeof bd + sizeof item_width + sizeof item_height + sizeof n_Items);
	return s + getItemSize();
}

SPSF_Object::SPSF_Object() : lanes()
{
	total_size = 0;
	n_Lanes = 0;
}

SPSF_Object::SPSF_Object(std::vector<SPSF_Lane> &&lanes)
{
	this->n_Lanes = lanes.size();
	this->lanes = lanes;
	size_t total = sizeof SPSF_HEADER_ULONG + +sizeof total_size + sizeof n_Lanes;
	for (int i = 0;i <= n_Lanes - 1;i++)
	{
		total += this->lanes[i].getSize();
	}
	this->total_size = total;
}

size_t SPSF_Object::getTotalSize() const
{
	return total_size;
}

int32_t SPSF_Object::getN_Lanes() const
{
	return n_Lanes;
}

const std::vector<SPSF_Lane>& SPSF_Object::getLanes() const
{
	return lanes;
}

#define wr(a) out.write((char*)&a, sizeof a)

std::ostream& SPSF::operator<<(std::ostream &out, const SPSF_Item &item)
{
	if (!out.good()) throw std::exception("SPSF: Stream state is not good!\n");

	out.write(item.arr(), item.getSize());
	out.flush();
	return out;
}

std::ostream& SPSF::operator<<(std::ostream &out, const SPSF_Lane &lane)
{
	if (!out.good()) throw std::exception("SPSF: Stream state is not good!\n");

	wr(lane.ct);
	wr(lane.bd);
	wr(lane.item_width);
	wr(lane.item_height);
	wr(lane.n_Items);

	auto items = lane.getItems();
	for (int i = 0;i <= lane.getN_Items() - 1;i++)
	{
		out << items[i];
	}
	out.flush();
	return out;
}

std::ostream& SPSF::operator<<(std::ostream &out, const SPSF_Object &spsf)
{
	if (!out.good()) throw std::exception("SPSF: Stream state is not good!\n");

	wr(SPSF_HEADER_ULONG);
	wr(spsf.total_size);
	wr(spsf.n_Lanes);

	auto lanes = spsf.getLanes();
	for (int i = 0;i <= spsf.getN_Lanes() - 1;i++)
	{
		out << lanes[i];
	}
	return out;
}

#define rd(a) in.read((char*)&(a), sizeof a)

#include <iostream>
std::istream& SPSF::operator>>(std::istream &in, SPSF_Item &item)
{
	if (!in.good()) throw std::exception("SPSF: Stream state is not good!\n");
	item.data = std::make_unique<char[]>(item.size);
	std::cout << "Size of item " << item.size << std::endl;
	in.read(item.arr(), item.size);
	return in;
}

std::istream& SPSF::operator>>(std::istream& in, SPSF_Lane &lane)
{
	if (!in.good()) throw std::exception("SPSF: Stream state is not good!\n");

	rd(lane.ct);
	rd(lane.bd);
	rd(lane.item_width);
	rd(lane.item_height);
	rd(lane.n_Items);

	std::vector<SPSF_Item> &items = lane.items;
	items.resize(lane.n_Items);

	for (int i = 0;i <= lane.n_Items - 1;i++)
	{
		items[i].size = lane.getItemSize();
		in >> items[i];
	}
	return in;
}

std::istream& SPSF::operator>>(std::istream& in, SPSF_Object &obj)
{
	if (!in.good()) throw std::exception("SPSF Stream state is not good!\n");

	unsigned long long h = 0;
	rd(h);
	if (h != SPSF_HEADER_ULONG) throw std::exception("SPSF Header does not match. Are you sure the file is in the SPSF format?");

	rd(obj.total_size);
	rd(obj.n_Lanes);

	std::vector<SPSF_Lane> &lanes = obj.lanes;
	lanes.resize(obj.n_Lanes);
	for (int i = 0;i <= obj.n_Lanes - 1;i++)
	{
		in >> obj.lanes[i];
	}

	return in;
}