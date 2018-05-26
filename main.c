#include "gba.h"
#include "keypad.h"
#include "screenmode.h"

#define RGB15(r,g,b) ((u16)(b << 10) | (g << 5) | r)

static inline void vid_vsync()
{
	while(REG_VCOUNT < 160);	// wait till VBlank
	while(REG_VCOUNT >= 160);	// wait till VDraw
}

int main(void)
{
	// Write into the I/O registers, setting video display parameters.
	SetMode(BG2_ENABLE | MODE_3); // Use video mode 3 (in BG2, a 16bpp bitmap in VRAM)

	u8 blink = 0;
	vu16 *vram = VideoBuffer;

	// Repeat forever
	while(1) {
		// Write pixel colours into VRAM
		for(int i = 0; i < 80; i++) {
			vram[i*240 + 115] = RGB15(0,0,31);
			vram[i*240 + 120] = RGB15(0,31,0);
			vram[i*240 + 121] = blink * RGB15(31,31,31);
			vram[i*240 + 125] = RGB15(31,0,0);
		}
		if(KeyPressed(KEY_ANY)) {
			blink = 1;
		} else {
			blink = 0;
		}
		vid_vsync();
	}

	return 0;
}
