#include "sys/gba.h"
#include "gfx/sprite.h"

#include <string.h>

const uint32_t sprite_x = 104;
const uint32_t sprite_y = 32;
const uint32_t sprite_pallete = 0;
const uint32_t sprite_id = 512;

int main(void)
{
    memcpy(&tile4_obj[512], spriteTiles, spriteTilesLen);
    memcpy(&pal_obj_bank[0], spritePal, spritePalLen);

    // Initialize all sprites
    oam_init(128);

    obj_attr_t* sprite = &obj_attr[0];
    BFN_SET2(sprite->attr0, ATTR0_SQUARE, ATTR0_SHAPE);
    BFN_SET2(sprite->attr1, ATTR1_SIZE_32, ATTR1_SIZE);
    BFN_SET(sprite->attr2, sprite_id, ATTR2_ID);
    BFN_SET(sprite->attr2, sprite_pallete, ATTR2_PALBANK);

    // position sprite
    BFN_SET(sprite->attr0, sprite_y, ATTR0_Y);
    BFN_SET(sprite->attr1, sprite_x, ATTR1_X);

	BFN_SET2(sprite->attr0, ATTR0_REG, ATTR0_MODE);

	// Write into the I/O registers, setting video display parameters.
	SetMode(BG2_ENABLE | MODE_3 | OBJ_ENABLE | OBJ_MAP_1D);

	uint8_t blink = 0;
	// Repeat forever
	while(1) {
		// Write pixel colours into VRAM
		for(int i = 0; i < 80; i++) {
			screen->mode3[i][115] = RGB15(0,0,31);
			screen->mode3[i][120] = RGB15(0,31,0);
			screen->mode3[i][121] = blink * RGB15(31,31,31);
			screen->mode3[i][125] = RGB15(31,0,0);
		}
		if(KeyPressed(ANY)) {
			blink = 1;
		} else {
			blink = 0;
		}
		vid_vsync();
	}

	return 0;
}
