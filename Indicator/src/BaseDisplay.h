#ifndef BaseDisplay_h
#define BaseDisplay_h

#include "Indicator.h"

class BaseDisplay {
	
	public:
		Indicator* p_idk;
		BaseDisplay(Indicator* p);

		void fill_matrix(uint8_t red, uint8_t green, uint8_t blue );
};

#endif
