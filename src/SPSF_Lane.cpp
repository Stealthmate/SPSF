#include "SPSF_Lane.hpp"
#include "Packers.hpp"
using namespace SPSF;

SPSF_Lane SPSF_Lane::createLaneFromData(
	byte** data,
	int16_t item_width,
	int16_t item_height,
	int32_t n_items,
	ColorType ct_internal,
	BitDepth bd_internal,
	ColorType ct_provided,
	BitDepth bd_provided
	)
{

	size_t itemsize = std::ceil(item_width * item_height * ct_internal * bd_internal / (double)(sizeof int32_t * 8));


	SPSF_Lane lane;
	lane.items.reserve(n_items);

	lane.ct = ct_internal;
	lane.bd = bd_internal;
	lane.n_Items = n_items;
	lane.item_width = item_width;
	lane.item_height = item_height;

	for (int i = 0;i <= n_items;i++)
	{
		repack(item_width*item_height*n_items,
			ct_provided * bd_provided,
			ct_internal * bd_internal,
			data[i], new_data.get());
		lane.items.push_back(createItemFromData);
	}

	return lane;
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
		this->items[i].size = getItemSizeInBytes();
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

size_t SPSF_Lane::getItemSizeInBytes() const
{
	size_t data_size_bits = ct * bd * item_width * item_height;
	size_t data_size_bytes = (data_size_bits / 8) + ((bool)(data_size_bits % 8));
	return data_size_bytes;
}

size_t SPSF_Lane::getSizeInBytes() const
{
	size_t s = (sizeof ct + sizeof bd + sizeof item_width + sizeof item_height + sizeof n_Items);
	return s + getItemSizeInBytes();
}

int32_t SPSF_Lane::getN_ElementsPerItem() const
{
	return item_width * item_height;
}

std::istream& SPSF::operator>>(std::istream& in, SPSF_Lane &lane)
{
	if (!in.good()) throw std::exception("SPSF: Stream state is not good!\n");
	/*
	rd(lane.ct);
	rd(lane.bd);
	rd(lane.item_width);
	rd(lane.item_height);
	rd(lane.n_Items);
	*/
	std::vector<SPSF_Item> &items = lane.items;
	items.resize(lane.n_Items);

	for (int i = 0;i <= lane.n_Items - 1;i++)
	{
		items[i].size = lane.getItemSizeInBytes();
		in >> items[i];
	}
	return in;
}


std::ostream& SPSF::operator<<(std::ostream &out, const SPSF_Lane &lane)
{
	if (!out.good()) throw std::exception("SPSF: Stream state is not good!\n");
	/*
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
	out.flush();*/
	return out;
}