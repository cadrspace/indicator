#include "BaseDisplay.h"

BaseDisplay::BaseDisplay(Indicator* _idk) {
	p_idk = _idk;
}

void BaseDisplay::fill_matrix(uint8_t red, uint8_t green, uint8_t blue ) {
	for (uint8_t i = 0; i < MATRIX_SIZE; i++) {
		p_idk->set_pixel(i, red, green, blue );
	}
}

