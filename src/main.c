#include "gba.h"
#include "tonc.h"
#include "keypad.h"
#include "screen.h"
#include "gfx/sprite.h"

#include <string.h>

#define RGB15(r,g,b) ((u16)(b << 10) | (g << 5) | r)

OBJ_ATTR obj_buffer[128];

void oam_init(OBJ_ATTR *obj, u32 count);

int main(void)
{
    // Places the tiles of a 4bpp boxed metroid sprite 
    //   into LOW obj memory (cbb == 4)
    memcpy(&tile_mem[4][512], spriteTiles, spriteTilesLen);
    memcpy(pal_obj_mem, spritePal, spritePalLen);

    // Initialize all sprites
    oam_init(obj_buffer, 128);

    int x=104, y= 32;
    u32 tid= 512, pb= 0;      // (3) tile id, pal-bank

    OBJ_ATTR *sprite= &obj_buffer[0];
    sprite->attr0 = ATTR0_SQUARE;             // Square, regular sprite
    sprite->attr1 = ATTR1_SIZE_32;            // 32x32p,
    sprite->attr2 = ATTR2_PALBANK(pb) | tid;  // palbank 0, tile 512

    // position sprite
    BFN_SET(sprite->attr0, y, ATTR0_Y);
    BFN_SET(sprite->attr1, x, ATTR1_X);

	BFN_SET2(sprite->attr0, ATTR0_REG, ATTR0_MODE);

    memcpy(oam_mem, obj_buffer, 8);

	// Write into the I/O registers, setting video display parameters.
	vu16 *vram = VideoBuffer;
	SetMode(BG2_ENABLE | MODE_3 | OBJ_ENABLE | OBJ_MAP_1D);

	u8 blink = 0;
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

//! Initialize an array of count OBJ_ATTRs with with safe values.
void oam_init(OBJ_ATTR *obj, u32 count)
{
	u32 nn= (count+3)>>2;
	u32 *dst= (u32*)obj;

	// Hide all and set OBJ_AFFINEs to I
	while(nn--)
	{
		*dst++= ATTR0_HIDE;
		*dst++= 0x0100<<16;
		*dst++= ATTR0_HIDE;
		*dst++= 0;
		*dst++= ATTR0_HIDE;
		*dst++= 0;
		*dst++= ATTR0_HIDE;
		*dst++= 0x0100<<16;
	}
	// init oam
	memcpy(oam_mem, obj, 8 * count);
}
