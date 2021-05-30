#include "gfx/sprite.h"
#include "sys/gba.h"

#include <string.h>

const uint16_t sprite_pallete = 0;
const uint16_t sprite_id = 512;

int main(void) {
    int16_t sprite_x = 104;
    int16_t sprite_y = 45;

    // Initialize tile and pallete data.
    memcpy(&tile4_obj[512], spriteTiles, spriteTilesLen);
    memcpy(&pal_obj_bank[0], spritePal, spritePalLen);

    // Initialize all sprites
    oam_init(128);

    // Set sprite attributes.
    obj_attr_t *sprite = &obj_attr[0];
    BFN_SET2(sprite->attr0, ATTR0_SQUARE, ATTR0_SHAPE);
    BFN_SET2(sprite->attr1, ATTR1_SIZE_32, ATTR1_SIZE);
    BFN_SET(sprite->attr2, sprite_id, ATTR2_ID);
    BFN_SET(sprite->attr2, sprite_pallete, ATTR2_PALBANK);

    // position sprite
    BFN_SET(sprite->attr0, sprite_y, ATTR0_Y);
    BFN_SET(sprite->attr1, sprite_x, ATTR1_X);

    // Set sprite as visible.
    BFN_SET2(sprite->attr0, ATTR0_REG, ATTR0_MODE);

    // Write into the I/O registers, setting video display parameters.
    SetMode(BG2_ENABLE | MODE_3 | OBJ_ENABLE | OBJ_MAP_1D);

    int8_t sprite_dy = -1;
    uint32_t frame = 0;

    // Write pixel colours into VRAM
    for (int i = 0; i < 80; i++) {
        screen->mode3[i][115] = RGB15(0, 0, 31);
        screen->mode3[i][120] = RGB15(0, 31, 0);
        screen->mode3[i][125] = RGB15(31, 0, 0);
    }
    // Repeat forever
    while (1) {
        if (frame % 120 == 0) {
            if (sprite_y > 48) {
                sprite_dy = -1;
            } else if (sprite_y < 0) {
                sprite_dy = 1;
            }
            sprite_y += sprite_dy;
            BFN_SET(sprite->attr0, sprite_y, ATTR0_Y);
        }
        vid_vsync();
        frame++;
    }

    return 0;
}
