#ifndef SPSF_SPSF_ITEM_H
#define SPSF_SPSF_ITEM_H

#include "Config.hpp"

#include <iostream>

#include "SPSF_Element.hpp"

namespace SPSF
{
	class SPSF_Item
	{
	public:

		static SPSF_API SPSF_Item pack(
			owned_data data,
			int32_t n_elements,
			int8_t element_size_bits_internal,
			int8_t element_size_bits_provided
			);

	private:

		friend SPSF_API std::istream& operator>>(std::istream &in, SPSF_Item &item);
		friend SPSF_API std::ostream& operator<<(std::ostream& out, const SPSF_Item &item);

		friend class SPSF_Lane;
		friend class SPSF_Object;

		friend SPSF_API std::istream& operator>>(std::istream &in, SPSF_Lane &lane);

		owned_data data;
		int32_t n_elements;
		size_t size;

	public:

		SPSF_API SPSF_Item();

		SPSF_API SPSF_Item(owned_data data, size_t size);

		SPSF_API SPSF_Item(const SPSF_Item &item);
		SPSF_Item(SPSF_Item &&item) = default;

		SPSF_API SPSF_Item& operator=(const SPSF_Item &item);
		SPSF_Item& operator=(SPSF_Item &&item) = default;

		SPSF_API int32_t getN_Elements() const;
		SPSF_API size_t getSizeInBytes() const;

		SPSF_API const byte* getData() const;
		SPSF_API byte* getData();

	};

	SPSF_API std::istream& operator>>(std::istream &in, SPSF_Item &item);
	SPSF_API std::ostream& operator<<(std::ostream &out, const SPSF_Item &item);
}



#endif //SPSF_SPSF_ITEM_H