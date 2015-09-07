#ifndef SPSF_SPSF_LANE_H
#define SPSF_SPSF_LANE_H

#include "Config.hpp"

#include <vector>
#include <iostream>

#include "SPSF_Item.hpp"

namespace SPSF
{
	class SPSF_Lane
	{
	public:

		static SPSF_API SPSF_Lane createLaneFromData(
			owned_data data,

			int16_t item_width,
			int16_t item_height,
			int32_t n_items,

			ColorType ct_internal,
			BitDepth bd_internal,
			ColorType ct_provided,
			BitDepth bd_provided
			);

	private:

		friend SPSF_API std::ostream& operator<<(std::ostream& out, const SPSF_Lane &lane);
		friend SPSF_API std::istream& operator>>(std::istream &in, SPSF_Lane &lane);

		friend class SPSF_Object;

		ColorType ct;
		BitDepth bd;

		int16_t item_width;
		int16_t item_height;

		int32_t n_Items;

		std::vector<SPSF_Item> items;

	public:

		SPSF_API SPSF_Lane();

		SPSF_API SPSF_Lane(ColorType ct, BitDepth bd, int16_t item_width, int16_t item_height, std::vector<SPSF_Item> &&items);

		SPSF_Lane(const SPSF_Lane &lane) = default;
		SPSF_Lane(SPSF_Lane &&lane) = default;

		SPSF_Lane& operator=(const SPSF_Lane &lane) = default;
		SPSF_Lane& operator=(SPSF_Lane &&lane) = default;

		SPSF_API ColorType getColorType() const;
		SPSF_API BitDepth getBitDepth() const;
		SPSF_API int16_t getItemWidth() const;
		SPSF_API int16_t getItemHeight() const;
		SPSF_API int32_t getN_Items() const;
		SPSF_API const std::vector<SPSF_Item>& getItems() const;

		SPSF_API size_t getItemSizeInBytes() const;
		SPSF_API size_t getSizeInBytes() const;
		SPSF_API int32_t getN_ElementsPerItem() const;
	};
	SPSF_API std::istream& operator>>(std::istream &in, SPSF_Lane &lane);
	SPSF_API std::ostream& operator<<(std::ostream &out, const SPSF_Lane &lane);
}

#endif //SPSF_SPSF_LANE_H