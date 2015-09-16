#include "SPSF_Object.hpp"

#include <fstream>

using namespace SPSF;

SPSF_Object SPSF_Object::createObjectFromData(
	byte*** data,

	int32_t n_lanes,
	int32_t *n_items,
	int16_t *item_widths,
	int16_t *item_heights,

	ColorType *color_types_internal,
	BitDepth *bit_depths_internal,
	ColorType *color_types_provided,
	BitDepth *bit_depths_provided)
{
	SPSF_Object spsfobj;
	spsfobj.n_Lanes = n_lanes;
	std::vector<SPSF_Lane> lanes;
	for (int i = 0;i <= n_lanes - 1;i++)
	{
		lanes.push_back(createLaneFromData());
	}
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