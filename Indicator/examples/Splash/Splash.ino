#include <Indicator.h>
#include <BaseDisplay.h>

const uint8_t lat = 5, clk_v = 3, clk_h = 4, dat = 2;

Indicator idk(lat, clk_v, clk_h, dat);
BaseDisplay dsp(&idk);

void setup() {
	idk.init();
	dsp.fill_matrix(MAX_BRT,0,0);
}

void loop() {
	idk.render_matrix();
}
