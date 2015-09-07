#ifndef SPSF_SPSF_ELEMENT_H
#define SPSF_SPSF_ELEMENT_H

#include "Config.hpp"

namespace SPSF
{

	class SPSF_Element
	{
		typedef char element_component;
		element_component components[4];
		char n_components;

	public:

		SPSF_API SPSF_Element();
		SPSF_API SPSF_Element(element_component components[4], char n_components);
		SPSF_API SPSF_Element(const SPSF_Element &element) = default;
		SPSF_API SPSF_Element(SPSF_Element &&element) = default;

		SPSF_Element& operator=(const SPSF_Element &element) = default;
		SPSF_Element& operator=(SPSF_Element &&element) = default;

		SPSF_API element_component getComponent(int i) const;
		SPSF_API char getN_Components() const;

		SPSF_API const element_component* getComponents() const;
		SPSF_API element_component* getComponents();
	};


}

#endif //SPSF_SPSF_ELEMENT_H