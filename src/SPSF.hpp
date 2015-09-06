#ifndef SPSF_H
#define SPSF_H

#include <stdint.h>
#include <string>
#include <vector>
#include <memory>

#include "Config.hpp"

namespace SPSF
{

	const char* SPSF_HEADER_STRING = "SPSFONTS";
	const unsigned long long SPSF_HEADER_ULONG = *reinterpret_cast<const unsigned long long*>(SPSF_HEADER_STRING);

	enum ColorType : int8_t
	{
		CT_NULL = 0,
		GRAYSCALE = 1,
		GRAYSCALE_ALPHA = 2,
		RED_GREEN_BLUE = 3,
		RED_GREEN_BLUE_ALPHA = 4
	};

	enum BitDepth : int8_t
	{
		BD_NULL = 0,
		BD_1 = 1,
		BD_2 = 2,
		BD_4 = 4,
		BD_8 = 8
	};

	class SPSF_Item
	{
		std::unique_ptr<char[]> data;
		size_t size;
		friend class SPSF_Lane;

		friend SPSF_API std::ostream& operator<<(std::ostream& out, const SPSF_Item &item);
		friend SPSF_API std::istream& operator>>(std::istream &in, SPSF_Item &item);
		friend SPSF_API std::istream& operator>>(std::istream &in, SPSF_Lane &lane);
	public:

		SPSF_API SPSF_Item();

		SPSF_API SPSF_Item(std::unique_ptr<char[]> data, size_t size);

		SPSF_API SPSF_Item(const SPSF_Item &item);
		SPSF_Item(SPSF_Item &&item) = default;

		SPSF_API SPSF_Item& operator=(const SPSF_Item &item);
		SPSF_Item& operator=(SPSF_Item &&item) = default;

		SPSF_API size_t getSize() const;

		SPSF_API const char* arr() const;
		SPSF_API char* arr();
	};

	class SPSF_Lane
	{
		ColorType ct;
		BitDepth bd;

		int16_t item_width;
		int16_t item_height;

		int32_t n_Items;

		std::vector<SPSF_Item> items;
		friend SPSF_API std::ostream& operator<<(std::ostream& out, const SPSF_Lane &lane);
		friend SPSF_API std::istream& operator>>(std::istream &in, SPSF_Lane &lane);

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

		SPSF_API size_t getItemSize() const;
		SPSF_API size_t getSize() const;
	};

	class SPSF_Object
	{

		size_t total_size;
		int32_t n_Lanes;

		std::vector<SPSF_Lane> lanes;

		friend SPSF_API std::ostream& operator<<(std::ostream& out, const SPSF_Object &obj);
		friend SPSF_API std::istream& operator>>(std::istream &in, SPSF_Object &obj);

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

	SPSF_API std::ostream& operator<<(std::ostream &out, const SPSF_Item &item);
	SPSF_API std::ostream& operator<<(std::ostream &out, const SPSF_Lane &lane);
	SPSF_API std::ostream& operator<<(std::ostream &out, const SPSF_Object &spsf);

	SPSF_API std::istream& operator>>(std::istream &in, SPSF_Item &item);
	SPSF_API std::istream& operator>>(std::istream &in, SPSF_Lane &lane);
	SPSF_API std::istream& operator>>(std::istream &in, SPSF_Object &obj);
}

#endif // SPSF_H