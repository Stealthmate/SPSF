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

const char* SPSF_Item::operator&() const
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
	size_t data_size = ct * bd * item_width * item_height;
	data_size = (data_size / 8) + (bool)(data_size % 8);

	for (int i = 0;i <= n_Items - 1;i++)
	{
		this->items[i].size = data_size;
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
#include <iostream>
size_t SPSF_Lane::getSize() const
{
	size_t s = (sizeof ct + sizeof bd + sizeof item_width + sizeof item_height + sizeof n_Items);
	size_t data_size = ct * bd * item_width * item_height;
	data_size = (data_size / 8) + ((bool)(data_size % 8));
	return s + data_size;
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
	size_t total = sizeof SPSF_HEADER + +sizeof total_size + sizeof n_Lanes;
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

	out.write(&item, item.getSize());
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
	return out;
}

std::ostream& SPSF::operator<<(std::ostream &out, const SPSF_Object &spsf)
{
	if (!out.good()) throw std::exception("SPSF: Stream state is not good!\n");

	out << SPSF_HEADER;
	wr(spsf.total_size);
	wr(spsf.n_Lanes);

	auto lanes = spsf.getLanes();
	for (int i = 0;i <= spsf.getN_Lanes() - 1;i++)
	{
		out << lanes[i];
	}
	return out;
}