#include "SPSF.hpp"

#include <fstream>

/*

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


SPSF_Lane SPSF_Lane::createLaneFromData(
	owned_bytes data,

	int16_t item_width,
	int16_t item_height,
	int32_t n_items,

	ColorType ct_internal,
	BitDepth bd_internal,
	ColorType ct_provided,
	BitDepth bd_provided
	)
{
	SPSF_Lane lane;
	lane.item_width = item_width;
	lane.item_height = item_height;
	lane.n_Items = n_items;
	lane.ct = ct_internal;
	lane.bd = bd_internal;

	lane.items.resize(n_items);

	for (int i = 0;i <= n_items - 1;i++)
	{
		lane.items[i] = SPSF_Item::pack(std::move(data), item_width*item_height, ct_internal * bd_internal, ct_provided*bd_provided);
	}
}

SPSF_Object SPSF_Object::createObjectFromData(
	owned_bytes data,

	int32_t n_lanes,
	int32_t *n_items,
	int16_t *item_widths,
	int16_t *item_heights,

	ColorType *color_types_internal,
	BitDepth *bit_depths_internal,
	ColorType *color_types_provided,
	BitDepth *bit_depths_provided
	)
{
	SPSF_Object obj;
	obj.n_Lanes = n_lanes;
	obj.lanes.resize(n_lanes);
	size_t total_size = sizeof obj.n_Lanes + sizeof obj.total_size;
	size_t lane_item_size = 0;
	size_t offset = 0;
	for (int i = 0;i <= n_lanes - 1;i++)
	{
		obj.lanes[i].n_Items = n_items[i];
		obj.lanes[i].item_width = item_widths[i];
		obj.lanes[i].item_height = item_heights[i];
		obj.lanes[i].ct = color_types_internal[i];
		obj.lanes[i].bd = bit_depths_internal[i];

		lane_item_size = obj.lanes[i].getItemSize();

		createLaneFromData();

		offset += lane_item_size;

		total_size += obj.lanes[i].getSize();
	}
	obj.total_size = total_size;
}*/