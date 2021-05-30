#include "gfx.h"

// === Global struct instances
palette_t *pal_bg_bank = (palette_t *)MEM_PAL;
palette_t *pal_obj_bank = (palette_t *)MEM_PAL_OBJ;

screen_t *screen = (screen_t *)MEM_VRAM;

tile4_t *tile4_bg = (tile4_t *)MEM_VRAM;
tile8_t *tile8_bg = (tile8_t *)MEM_VRAM;
tile4_t *tile4_obj = (tile4_t *)MEM_VRAM_OBJ;
tile8_t *tile8_obj = (tile8_t *)MEM_VRAM_OBJ;

obj_attr_t *obj_attr = (obj_attr_t *)MEM_OAM;
obj_affine_t *obj_affine = (obj_affine_t *)MEM_OAM;

// === Helper fns
void vid_vsync() {
    while (REG_VCOUNT < 160)
        ; // wait till VBlank
    while (REG_VCOUNT >= 160)
        ; // wait till VDraw
}

//! Initialize an array of count OBJ_ATTRs with with safe values.
void oam_init(uint32_t count) {
    // Hide all and set OBJ_AFFINEs to I
    for (int32_t i = count; i > 0; i -= 4) {
        obj_attr[i].attr0 = ATTR0_HIDE;
        obj_affine[i].pa = 0x0100;

        obj_attr[i + 1].attr0 = ATTR0_HIDE;
        obj_affine[i].pb = 0;

        obj_attr[i + 2].attr0 = ATTR0_HIDE;
        obj_affine[i].pc = 0;

        obj_attr[i + 3].attr0 = ATTR0_HIDE;
        obj_affine[i].pd = 0x0100;
    }
}
