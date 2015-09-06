#include "../src/SPSF.hpp"
#include <iostream>
#include <fstream>

using namespace SPSF;

int main()
{
	std::unique_ptr<char[]> data = std::make_unique<char[]>(16);
	for (int i = 0;i <= 15;i++)
	{
		data[i] = i*3;
	}

	SPSF_Item item(std::move(data), 16);
	std::vector<SPSF_Item> items;
	items.push_back(item);

	SPSF_Lane lane(ColorType::RED_GREEN_BLUE_ALPHA, BitDepth::BD_8, 1, 4, std::move(items));

	std::vector<SPSF_Lane> lanes;
	lanes.push_back(lane);

	SPSF_Object obj(std::move(lanes));

	std::ofstream ofs("SPSF.bin", std::ios::binary | std::ios::trunc);

	if (ofs.is_open())
	{
		ofs << obj;
	}

}