#pragma once

#include "common.h"
#include "memmap.h"

// === Colorspace Conversion
#define RGB15(r, g, b) ((uint16_t)((b << 10) | (g << 5) | r))

// === structs
typedef struct {
    uint32_t data[8];
} tile4_t; //!< 4bpp tile
typedef struct {
    uint64_t data[8];
} tile8_t; //!< 8bpp tile

typedef struct {
    uint16_t x, y;
} ALIGN(4) u16pt_t;

typedef uint16_t color_t;

//! Palette bank type, for 16-color palette banks
typedef color_t palette_t[16];

//! Simple scale-rotation source struct.
typedef struct {
    int16_t sx;     //!< Horizontal zoom	(8.8f)
    int16_t sy;     //!< Vertical zoom		(8.8f)
    uint16_t alpha; //!< Counter-clockwise angle ( range [0, 0xFFFF] )
} ALIGN(4) aff_src_t;

//! Extended scale-rotate source struct
typedef struct {
    int32_t tex_x;  //!< Texture-space anchor, x coordinate	(.8f)
    int32_t tex_y;  //!< Texture-space anchor, y coordinate	(.8f)
    int16_t scr_x;  //!< Screen-space anchor, x coordinate	(.0f)
    int16_t scr_y;  //!< Screen-space anchor, y coordinate	(.0f)
    int16_t sx;     //!< Horizontal zoom	(8.8f)
    int16_t sy;     //!< Vertical zoom		(8.8f)
    uint16_t alpha; //!< Counter-clockwise angle ( range [0, 0xFFFF] )
} ALIGN(4) aff_src_ex_t;

//! Simple scale-rotation destination struct, BG version.
typedef struct {
    int16_t pa, pb;
    int16_t pc, pd;
} ALIGN(4) aff_dst_t;

//! Extended scale-rotate destination struct
typedef struct {
    int16_t pa, pb;
    int16_t pc, pd;
    int32_t dx, dy;
} ALIGN(4) aff_dst_ex_t;

//! Object attributes.
/*!	\note attribute 3 is padding for the interlace with OBJ_AFFINE. If
 *	not using affine objects, it can be used as a free field
 */
typedef struct {
    uint16_t attr0;
    uint16_t attr1;
    uint16_t attr2;
} ALIGN(4) obj_attr_t;

//! Object affine parameters.
/*!	\note most fields are padding for the interlace with OBJ_ATTR.
 */
typedef struct {
    uint16_t fill0[3];
    int16_t pa;
    uint16_t fill1[3];
    int16_t pb;
    uint16_t fill2[3];
    int16_t pc;
    uint16_t fill3[3];
    int16_t pd;
} ALIGN(4) obj_affine_t;

typedef uint16_t text_screen_t[4096];
typedef uint8_t rot_screen_t[16384];

typedef union {
    text_screen_t mode0[4];
    struct {
        text_screen_t text[2];
        rot_screen_t rot;
    } mode1;
    rot_screen_t mode2[2];
    color_t mode3[160][240];
    uint8_t mode4[2][160][240];
    color_t mode5[2][128][160];
} screen_t;

// === REG_DISPCNT fields
#define MODE_0 0x0
#define MODE_1 0x1
#define MODE_2 0x2
#define MODE_3 0x3
#define MODE_4 0x4
#define MODE_5 0x5

#define BACKBUFFER 0x10
#define H_BLANK_OAM 0x20

#define OBJ_MAP_2D 0x0
#define OBJ_MAP_1D 0x40

#define FORCE_BLANK 0x80

#define BG0_ENABLE 0x100
#define BG1_ENABLE 0x200
#define BG2_ENABLE 0x400
#define BG3_ENABLE 0x800
#define OBJ_ENABLE 0x1000

#define WIN1_ENABLE 0x2000
#define WIN2_ENABLE 0x4000
#define WINOBJ_ENABLE 0x8000

// --- Set Mode Macro
#define SetMode(mode) REG_DISPCNT = (mode)

// === Reg field values
#define SE_HFLIP 0x0400 //!< Horizontal flip
#define SE_VFLIP 0x0800 //!< Vertical flip

#define SE_ID_MASK 0x03FF
#define SE_ID_SHIFT 0

#define SE_FLIP_MASK 0x0C00
#define SE_FLIP_SHIFT 10

#define SE_PALBANK_MASK 0xF000
#define SE_PALBANK_SHIFT 12

// --- OAM attribute 0
#define ATTR0_REG 0          //!< Regular object
#define ATTR0_AFF 0x0100     //!< Affine object
#define ATTR0_HIDE 0x0200    //!< Inactive object
#define ATTR0_AFF_DBL 0x0300 //!< Double-size affine object
#define ATTR0_AFF_DBL_BIT 0x0200
#define ATTR0_BLEND 0x0400  //!< Enable blend
#define ATTR0_WINDOW 0x0800 //!< Use for object window
#define ATTR0_MOSAIC 0x1000 //!< Enable mosaic
#define ATTR0_4BPP 0        //!< Use 4bpp (16 color) tiles
#define ATTR0_8BPP 0x2000   //!< Use 8bpp (256 color) tiles
#define ATTR0_SQUARE 0      //!< Square shape
#define ATTR0_WIDE 0x4000   //!< Tall shape (height > width)
#define ATTR0_TALL 0x8000   //!< Wide shape (height < width)

#define ATTR0_Y_MASK 0x00FF
#define ATTR0_Y_SHIFT 0

#define ATTR0_MODE_MASK 0x0300
#define ATTR0_MODE_SHIFT 8

#define ATTR0_SHAPE_MASK 0xC000
#define ATTR0_SHAPE_SHIFT 14

// --- OAM attribute 1
#define ATTR1_HFLIP 0x1000 //!< Horizontal flip (reg obj only)
#define ATTR1_VFLIP 0x2000 //!< Vertical flip (reg obj only)
// Base sizes
#define ATTR1_SIZE_8 0
#define ATTR1_SIZE_16 0x4000
#define ATTR1_SIZE_32 0x8000
#define ATTR1_SIZE_64 0xC000
// Square sizes
#define ATTR1_SIZE_8x8 0        //!< Size flag for  8x8 px object
#define ATTR1_SIZE_16x16 0x4000 //!< Size flag for 16x16 px object
#define ATTR1_SIZE_32x32 0x8000 //!< Size flag for 32x32 px object
#define ATTR1_SIZE_64x64 0xC000 //!< Size flag for 64x64 px object
// Tall sizes
#define ATTR1_SIZE_8x16 0       //!< Size flag for  8x16 px object
#define ATTR1_SIZE_8x32 0x4000  //!< Size flag for  8x32 px object
#define ATTR1_SIZE_16x32 0x8000 //!< Size flag for 16x32 px object
#define ATTR1_SIZE_32x64 0xC000 //!< Size flag for 32x64 px object
// Wide sizes
#define ATTR1_SIZE_16x8 0       //!< Size flag for 16x8 px object
#define ATTR1_SIZE_32x8 0x4000  //!< Size flag for 32x8 px object
#define ATTR1_SIZE_32x16 0x8000 //!< Size flag for 32x16 px object
#define ATTR1_SIZE_64x32 0xC000 //!< Size flag for 64x64 px object

#define ATTR1_X_MASK 0x01FF
#define ATTR1_X_SHIFT 0

#define ATTR1_AFF_ID_MASK 0x3E00
#define ATTR1_AFF_ID_SHIFT 9

#define ATTR1_FLIP_MASK 0x3000
#define ATTR1_FLIP_SHIFT 12

#define ATTR1_SIZE_MASK 0xC000
#define ATTR1_SIZE_SHIFT 14

// --- OAM attribute 2
#define ATTR2_ID_MASK 0x03FF
#define ATTR2_ID_SHIFT 0

#define ATTR2_PRIO_MASK 0x0C00
#define ATTR2_PRIO_SHIFT 10

#define ATTR2_PALBANK_MASK 0xF000
#define ATTR2_PALBANK_SHIFT 12

// --- Memory sub section sizes
#define PAL_BG_SIZE 0x00200    //!< BG palette size
#define PAL_OBJ_SIZE 0x00200   //!< Object palette size
#define CBB_SIZE 0x04000       //!< Charblock size
#define SBB_SIZE 0x00800       //!< Screenblock size
#define VRAM_BG_SIZE 0x10000   //!< BG VRAM size
#define VRAM_OBJ_SIZE 0x08000  //!< Object VRAM size
#define M3_SIZE 0x12C00        //!< Mode 3 buffer size
#define M4_SIZE 0x09600        //!< Mode 4 buffer size
#define M5_SIZE 0x0A000        //!< Mode 5 buffer size
#define VRAM_PAGE_SIZE 0x0A000 //!< Bitmap page size

// --- Memory sub sections
#define MEM_PAL_BG (MEM_PAL)                   //!< Background palette address
#define MEM_PAL_OBJ (MEM_PAL + PAL_BG_SIZE)    //!< Object palette address
#define MEM_VRAM_OBJ (MEM_VRAM + VRAM_BG_SIZE) //!< Object VRAM address

// === Global struct instances
extern palette_t *pal_bg_bank;
extern palette_t *pal_obj_bank;
extern screen_t *screen;
extern tile4_t *tile4_bg;
extern tile8_t *tile8_bg;
extern tile4_t *tile4_obj;
extern tile8_t *tile8_obj;
extern obj_attr_t *obj_attr;
extern obj_affine_t *obj_affine;

void vid_vsync();
void oam_init(uint32_t count);
