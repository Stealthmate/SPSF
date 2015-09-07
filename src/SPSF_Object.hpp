#ifndef SPSF_SPSF_OBJECT_H
#define SPSF_SPSF_OBJECT_H

#include "Config.hpp"

#include <vector>
#include <iostream>

#include "SPSF_Lane.hpp"

namespace SPSF
{
	class SPSF_Object
	{
	public:

		static SPSF_API SPSF_Object createObjectFromData(
			owned_data data,

			int32_t n_lanes,
			int32_t *n_items,
			int16_t *item_widths,
			int16_t *item_heights,

			ColorType *color_types_internal,
			BitDepth *bit_depths_internal,
			ColorType *color_types_provided,
			BitDepth *bit_depths_provided
			);

	private:

		friend SPSF_API std::istream& operator>>(std::istream &in, SPSF_Object &obj);
		friend SPSF_API std::ostream& operator<<(std::ostream& out, const SPSF_Object &obj);

		size_t total_size;
		int32_t n_Lanes;

		std::vector<SPSF_Lane> lanes;

	public:

		SPSF_API SPSF_Object();

		SPSF_API SPSF_Object(std::vector<SPSF_Lane> &&lanes);

		SPSF_Object(const SPSF_Object &spsf) = default;
		SPSF_Object(SPSF_Object &&spsf) = default;

		SPSF_Object& operator=(const SPSF_Object &obj) = default;
		SPSF_Object& operator=(SPSF_Object &&obj) = default;

		SPSF_API size_t getTotalSize() const;
		SPSF_API int32_t getN_Lanes() const;
		SPSF_API const std::vector<SPSF_Lane>& getLanes() const;

	};

	SPSF_API std::istream& operator>>(std::istream &in, SPSF_Object &obj);
	SPSF_API std::ostream& operator<<(std::ostream &out, const SPSF_Object &spsf);
}

#endif //SPSF_SPSF_OBJECT_H