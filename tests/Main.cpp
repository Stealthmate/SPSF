#include "../src/SPSF.hpp"
#include <iostream>
#include <fstream>

using namespace SPSF;

char hexarr[] = {
	'0', '1', '2', '3', '4',
	'5', '6', '7', '8', '9',
	'A', 'B', 'C', 'D', 'E', 'F'
};

std::string hex(char b)
{
	unsigned char ub = static_cast<unsigned char>(b);
	std::string sb = "";
	sb += hexarr[ub / 16];
	sb += hexarr[ub % 16];
	return  sb;
}

int main()
{
	std::unique_ptr<char[]> data = std::make_unique<char[]>(256);
	for (int i = 0;i <= 255;i++)
	{
		data[i] = i*3;
	}

	SPSF_Item item(std::move(data), 256);
	std::vector<SPSF_Item> items;
	items.push_back(item);

	SPSF_Lane lane(ColorType::RED_GREEN_BLUE_ALPHA, BitDepth::BD_8, 8, 8, std::move(items));

	std::vector<SPSF_Lane> lanes;
	lanes.push_back(lane);

	SPSF_Object obj(std::move(lanes));

	std::ofstream ofs("SPSF.bin", std::ios::binary | std::ios::trunc);

	if (ofs.is_open())
	{
		ofs << obj;
	}
	
	SPSF_Object obj_read;

	std::ifstream ifs("SPSF.bin", std::ios::in | std::ios::binary);
	if (ifs.is_open()) ifs >> obj_read;
	std::cout << "Read:"
		<< "\nTotal Size: " << obj_read.getTotalSize()
		<< "\nNumber of lanes: " << obj_read.getN_Lanes() << std::endl;

	auto lanes_read = obj_read.getLanes();
	for (int i = 0;i <= obj_read.getN_Lanes() - 1;i++)
	{
		std::cout
			<< "\n\tLane: " << i + 1
			<< "\n\tColor Type: " << lanes_read[i].getColorType()
			<< "\n\tBitDepth: " << lanes_read[i].getBitDepth()
			<< "\n\tItem Width: " << lanes_read[i].getItemWidth()
			<< "\n\tItem Height: " << lanes_read[i].getItemHeight()
			<< "\n\tItem Size: " << lanes_read[i].getItemSize()
			<< "\n\tNumber of Items: " << lanes_read[i].getN_Items();

		auto items_read = lanes_read[i].getItems();
		for (int j = 0;j <= lanes_read[i].getN_Items() - 1;j++)
		{
			std::cout << "\n\t\tItem: " << j + 1;
			std::cout << "\n\t\t\t";
			auto item_read = &items_read[j];
			for (int k = 0; k <= lanes_read[i].getItemSize() - 1;k++)
			{
				std::cout << hex(item.arr()[k]) << " ";
			}
		}


	}

}