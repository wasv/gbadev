// Condensed copy of tonclib.
// Primarily types, macros, and globals.
//! \author J Vijn

#ifndef TONC_H
#define TONC_H

//! Force a variable to an \a n-byte boundary
#define ALIGN(n)	__attribute__((aligned(n)))

//! Force word alignment.
/*! \note	In the old days, GCC aggregates were always word aligned.
	  In the EABI environment (devkitPro r19 and higher), they are 
	  aligned to their widest member. While technically a good thing, 
	  it may cause problems for struct-copies. If you have aggregates 
	  that can multiples of 4 in size but don't have word members, 
	  consider using this attribute to make struct-copies possible again.
*/
#define ALIGN4		__attribute__((aligned(4)))

//! Pack aggregate members
/*! By default, members in aggregates are aligned to their native 
	  boundaries. Adding this prevents that. It will slow access though.
*/
#define PACKED		__attribute__((packed))

//! Deprecated notice.
/*! Indicates that this function/type/variable should not be used anymore.
	Replacements are (usually) present somewhere as well. 
*/
#define DEPRECATED	__attribute__((deprecated))

//! Inline function declarator
/*!	`inline' inlines the function when -O > 0 when called, 
	  but also creates a body for the function itself
	`static' removes the body as well
*/
#define INLINE static inline

/*	\}	*/


// --------------------------------------------------------------------
// TYPES
// --------------------------------------------------------------------
// === primary typedefs ===============================================

// Base types
typedef unsigned char  u8,  byte, uchar, echar;
typedef unsigned short u16, hword, ushort, eshort;
typedef unsigned int   u32, word, uint, eint;
typedef unsigned long long u64;

typedef signed char  s8;
typedef signed short s16;
typedef signed int   s32;
typedef signed long long s64;

// Volatile types
typedef volatile u8  vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;

typedef volatile s8  vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

// Const types
typedef const u8  cu8;
typedef const u16 cu16;
typedef const u32 cu32;
typedef const u64 cu64;

typedef const s8  cs8;
typedef const s16 cs16;
typedef const s32 cs32;
typedef const s64 cs64;

//! 8-word type for fast struct-copies
typedef struct { u32 data[8];  } BLOCK;

//! Type for consting a string.
typedef const char * const CSTR;

// === secondary typedefs =============================================

typedef s32 FIXED;					//!< Fixed point type
typedef u16 COLOR;					//!< Type for colors
typedef u16 SCR_ENTRY, SE;			//!< Type for screen entries
typedef u8  SCR_AFF_ENTRY, SAE;		//!< Type for affine screen entries

//! 4bpp tile type, for easy indexing and copying of 4-bit tiles
typedef struct { u32 data[8];  } TILE, TILE4;

//! 8bpp tile type, for easy indexing and 8-bit tiles
typedef struct { u32 data[16]; } TILE8;


#ifndef __cplusplus
//! Boolean type
typedef enum { false, true } bool;
#endif

#ifndef BOOL
typedef u8 BOOL;	// C++ bool == u8 too, that's why
#define TRUE 1
#define FALSE 0
#endif

// affine structs
//! Simple scale-rotation source struct.
typedef struct AFF_SRC
{
	s16 sx;		//!< Horizontal zoom	(8.8f)
	s16 sy;		//!< Vertical zoom		(8.8f)
	u16 alpha;	//!< Counter-clockwise angle ( range [0, 0xFFFF] )
} ALIGN4 AFF_SRC, ObjAffineSource;


//! Extended scale-rotate source struct
typedef struct AFF_SRC_EX
{
	s32 tex_x;	//!< Texture-space anchor, x coordinate	(.8f)
	s32 tex_y;	//!< Texture-space anchor, y coordinate	(.8f)
	s16 scr_x;	//!< Screen-space anchor, x coordinate	(.0f)
	s16 scr_y;	//!< Screen-space anchor, y coordinate	(.0f)
	s16 sx;		//!< Horizontal zoom	(8.8f)
	s16 sy;		//!< Vertical zoom		(8.8f)
	u16 alpha;	//!< Counter-clockwise angle ( range [0, 0xFFFF] )
} ALIGN4 AFF_SRC_EX, BgAffineSource;

//! Simple scale-rotation destination struct, BG version.
typedef struct AFF_DST
{
	s16 pa, pb;
	s16 pc, pd;
} ALIGN4 AFF_DST, ObjAffineDest;

//! Extended scale-rotate destination struct
typedef struct AFF_DST_EX
{
	s16 pa, pb;
	s16 pc, pd;
	s32 dx, dy;
} ALIGN4 AFF_DST_EX, BgAffineDest;

// === memory map structs  ============================================

// Tertiary types

//! Regular bg points; range: :0010 - :001F
typedef struct POINT16 { s16 x, y; } ALIGN4 POINT16, BG_POINT;

//! Affine parameters for backgrounds; range : 0400:0020 - 0400:003F
typedef struct AFF_DST_EX BG_AFFINE;

//!	DMA struct; range: 0400:00B0 - 0400:00DF
typedef struct DMA_REC
{
	const void *src;
	void *dst;
	u32 cnt;
} DMA_REC;

//! Timer struct, range: 0400:0100 - 0400:010F
/* The attribute is required, because union's counted as u32 otherwise.
*/
typedef struct TMR_REC 
{
	union { u16 start, count; } PACKED;
	u16 cnt;
} ALIGN4 TMR_REC;

//\}


//! PAL types 
//! Palette bank type, for 16-color palette banks 
typedef COLOR PALBANK[16];

//\}


/*! \name VRAM array types
*	These types allow VRAM access as arrays or matrices in their 
*	  most natural types.
*/
//\{
typedef SCR_ENTRY	SCREENLINE[32];
typedef SCR_ENTRY	SCREENMAT[32][32];
typedef SCR_ENTRY	SCREENBLOCK[1024];

typedef COLOR		M3LINE[240];
typedef u8			M4LINE[240];	// NOTE: u8, not u16!!
typedef COLOR		M5LINE[160];

typedef TILE		CHARBLOCK[512];
typedef TILE8		CHARBLOCK8[256];

//\}


/*! \name OAM structs 
*	\note These OBJ_ATTR and OBJ_AFFINE structs are interlaced in OAM. 
*	When using affine objs, struct/DMA/mem copies will give bad results.
*/
//\{

//! Object attributes.
/*!	\note attribute 3 is padding for the interlace with OBJ_AFFINE. If 
*	not using affine objects, it can be used as a free field
*/
typedef struct OBJ_ATTR
{
	u16 attr0;
	u16 attr1;
	u16 attr2;
	s16 fill;
} ALIGN4 OBJ_ATTR;


//! Object affine parameters.
/*!	\note most fields are padding for the interlace with OBJ_ATTR. 
*/
typedef struct OBJ_AFFINE
{
	u16 fill0[3];	s16 pa;
	u16 fill1[3];	s16 pb;
	u16 fill2[3];	s16 pc;
	u16 fill3[3];	s16 pd;
} ALIGN4 OBJ_AFFINE;


// --------------------------------------------------------------------
// DEFINES 
// --------------------------------------------------------------------

// --- Simple bit macros ---
#define BIT(n)					( 1<<(n) )
#define BIT_SHIFT(a, n)			( (a)<<(n) )
#define BIT_SET(word, flag)		( word |=  (flag) )
#define BIT_CLEAR(word, flag)	( word &= ~(flag) )
#define BIT_FLIP(word, flag)	( word ^=  (flag) )
#define BIT_EQ(word, flag)		( ((word)&(flag)) == (flag) )

#define BFN_PREP(x, name)		( ((x)<<name##_SHIFT) & name##_MASK )
#define BFN_GET(y, name)			( ((y) & name##_MASK)>>name##_SHIFT )
#define BFN_SET(y, x, name)		(y = ((y)&~name##_MASK) | BFN_PREP(x,name) )

#define BFN_PREP2(x, name)		( (x) & name##_MASK )
#define BFN_GET2(y, name)		( (y) & name##_MASK )
#define BFN_SET2(y, x, name)		(y = ((y)&~name##_MASK) | BFN_PREP2(x,name) )

// --- Reg screen entries ----------------------------------------------
#define SE_HFLIP		0x0400	//!< Horizontal flip
#define SE_VFLIP		0x0800	//!< Vertical flip

#define SE_ID_MASK		0x03FF
#define SE_ID_SHIFT			 0
#define SE_ID(n)		((n)<<SE_ID_SHIFT)

#define SE_FLIP_MASK	0x0C00
#define SE_FLIP_SHIFT		10
#define SE_FLIP(n)		((n)<<SE_FLIP_SHIFT)

#define SE_PALBANK_MASK		0xF000
#define SE_PALBANK_SHIFT		12
#define SE_PALBANK(n)		((n)<<SE_PALBANK_SHIFT)

// --- OAM attribute 0 -------------------------------------------------
#define ATTR0_REG				 0	//!< Regular object
#define ATTR0_AFF			0x0100	//!< Affine object
#define ATTR0_HIDE			0x0200	//!< Inactive object
#define ATTR0_AFF_DBL		0x0300	//!< Double-size affine object
#define ATTR0_AFF_DBL_BIT	0x0200
#define ATTR0_BLEND			0x0400	//!< Enable blend
#define ATTR0_WINDOW		0x0800	//!< Use for object window
#define ATTR0_MOSAIC		0x1000	//!< Enable mosaic
#define ATTR0_4BPP				 0	//!< Use 4bpp (16 color) tiles
#define ATTR0_8BPP			0x2000	//!< Use 8bpp (256 color) tiles
#define ATTR0_SQUARE			 0	//!< Square shape
#define ATTR0_WIDE			0x4000	//!< Tall shape (height &gt; width)
#define ATTR0_TALL			0x8000	//!< Wide shape (height &lt; width)

#define ATTR0_Y_MASK		0x00FF
#define ATTR0_Y_SHIFT			 0
#define ATTR0_Y(n)			((n)<<ATTR0_Y_SHIFT)

#define ATTR0_MODE_MASK		0x0300
#define ATTR0_MODE_SHIFT		 8
#define ATTR0_MODE(n)		((n)<<ATTR0_MODE_SHIFT)

#define ATTR0_SHAPE_MASK	0xC000
#define ATTR0_SHAPE_SHIFT		14
#define ATTR0_SHAPE(n)		((n)<<ATTR0_SHAPE_SHIFT)

// --- OAM attribute 1 -------------------------------------------------
#define ATTR1_HFLIP			0x1000	//!< Horizontal flip (reg obj only)
#define ATTR1_VFLIP			0x2000	//!< Vertical flip (reg obj only)
// Base sizes
#define ATTR1_SIZE_8			 0
#define ATTR1_SIZE_16		0x4000
#define ATTR1_SIZE_32		0x8000
#define ATTR1_SIZE_64		0xC000
// Square sizes
#define ATTR1_SIZE_8x8			 0	//!< Size flag for  8x8 px object
#define ATTR1_SIZE_16x16	0x4000	//!< Size flag for 16x16 px object
#define ATTR1_SIZE_32x32	0x8000	//!< Size flag for 32x32 px object
#define ATTR1_SIZE_64x64	0xC000	//!< Size flag for 64x64 px object
// Tall sizes
#define ATTR1_SIZE_8x16			 0	//!< Size flag for  8x16 px object
#define ATTR1_SIZE_8x32		0x4000	//!< Size flag for  8x32 px object
#define ATTR1_SIZE_16x32	0x8000	//!< Size flag for 16x32 px object
#define ATTR1_SIZE_32x64	0xC000	//!< Size flag for 32x64 px object
// Wide sizes
#define ATTR1_SIZE_16x8			 0	//!< Size flag for 16x8 px object
#define ATTR1_SIZE_32x8		0x4000	//!< Size flag for 32x8 px object
#define ATTR1_SIZE_32x16	0x8000	//!< Size flag for 32x16 px object
#define ATTR1_SIZE_64x32	0xC000	//!< Size flag for 64x64 px object

#define ATTR1_X_MASK		0x01FF
#define ATTR1_X_SHIFT			 0
#define ATTR1_X(n)			((n)<<ATTR1_X_SHIFT)

#define ATTR1_AFF_ID_MASK	0x3E00
#define ATTR1_AFF_ID_SHIFT		 9
#define ATTR1_AFF_ID(n)		((n)<<ATTR1_AFF_ID_SHIFT)

#define ATTR1_FLIP_MASK		0x3000
#define ATTR1_FLIP_SHIFT		12
#define ATTR1_FLIP(n)		((n)<<ATTR1_FLIP_SHIFT)

#define ATTR1_SIZE_MASK		0xC000
#define ATTR1_SIZE_SHIFT		14
#define ATTR1_SIZE(n)		((n)<<ATTR1_SIZE_SHIFT)

// --- OAM attribute 2 -------------------------------------------------
#define ATTR2_ID_MASK		0x03FF
#define ATTR2_ID_SHIFT			 0
#define ATTR2_ID(n)			((n)<<ATTR2_ID_SHIFT)

#define ATTR2_PRIO_MASK		0x0C00
#define ATTR2_PRIO_SHIFT		10
#define ATTR2_PRIO(n)		((n)<<ATTR2_PRIO_SHIFT)

#define ATTR2_PALBANK_MASK	0xF000
#define ATTR2_PALBANK_SHIFT		12
#define ATTR2_PALBANK(n)	((n)<<ATTR2_PALBANK_SHIFT)

#ifndef NULL
#define NULL (void*)0
#endif

// --------------------------------------------------------------------
//  STRUCTURED MEMORY MAP 
// --------------------------------------------------------------------

//! Main sections
#define MEM_EWRAM	0x02000000	//!< External work RAM
#define MEM_IWRAM	0x03000000	//!< Internal work RAM
#define MEM_IO		0x04000000	//!< I/O registers
#define MEM_PAL		0x05000000	//!< Palette. Note: no 8bit write !!
#define MEM_VRAM	0x06000000	//!< Video RAM. Note: no 8bit write !!
#define MEM_OAM		0x07000000	//!< Object Attribute Memory (OAM) Note: no 8bit write !!
#define MEM_ROM		0x08000000	//!< ROM. No write at all (duh)
#define MEM_SRAM	0x0E000000	//!< Static RAM. 8bit write only


//! Main section sizes
#define EWRAM_SIZE	0x40000
#define IWRAM_SIZE	0x08000
#define PAL_SIZE	0x00400
#define VRAM_SIZE	0x18000
#define OAM_SIZE	0x00400
#define SRAM_SIZE	0x10000

//! Sub section sizes
#define PAL_BG_SIZE		0x00200		//!< BG palette size
#define PAL_OBJ_SIZE	0x00200		//!< Object palette size
#define CBB_SIZE		0x04000		//!< Charblock size
#define SBB_SIZE		0x00800		//!< Screenblock size
#define VRAM_BG_SIZE	0x10000		//!< BG VRAM size
#define VRAM_OBJ_SIZE	0x08000		//!< Object VRAM size
#define M3_SIZE			0x12C00		//!< Mode 3 buffer size
#define M4_SIZE			0x09600		//!< Mode 4 buffer size
#define M5_SIZE			0x0A000		//!< Mode 5 buffer size
#define VRAM_PAGE_SIZE	0x0A000		//!< Bitmap page size

//! Sub sections
#define REG_BASE		MEM_IO

#define MEM_PAL_BG		(MEM_PAL)					//!< Background palette address
#define MEM_PAL_OBJ		(MEM_PAL + PAL_BG_SIZE)		//!< Object palette address
#define MEM_VRAM_FRONT	(MEM_VRAM)					//!< Front page address
#define MEM_VRAM_BACK	(MEM_VRAM + VRAM_PAGE_SIZE)	//!< Back page address
#define MEM_VRAM_OBJ	(MEM_VRAM + VRAM_BG_SIZE)	//!< Object VRAM address

//! Palette

//! Background palette.
/*! pal_bg_mem[i]	= color i					( COLOR )
*/
#define pal_bg_mem		((COLOR*)MEM_PAL)

//! Object palette. 
/*! pal_obj_mem[i]	= color i					( COLOR )
*/
#define pal_obj_mem		((COLOR*)MEM_PAL_OBJ)

//! Background palette matrix. 
/*! pal_bg_bank[y]		= bank y				( COLOR[ ] )<br>
	pal_bg_bank[y][x]	= color color y*16+x	( COLOR )
*/
#define pal_bg_bank		((PALBANK*)MEM_PAL)

//! Object palette matrix. 
/*!	pal_obj_bank[y]		= bank y				( COLOR[ ] )<br>
	pal_obj_bank[y][x]	= color y*16+x			( COLOR )
*/
#define pal_obj_bank	((PALBANK*)MEM_PAL_OBJ)


//! VRAM

//!	Charblocks, 4bpp tiles.
/*!	tile_mem[y]		= charblock y				( TILE[ ] )<br>
	tile_mem[y][x]	= block y, tile x			( TILE )
*/
#define tile_mem		( (CHARBLOCK*)MEM_VRAM)

//!	Charblocks, 8bpp tiles.
/*!	tile_mem[y]		= charblock y				( TILE[ ] )<br>
	tile_mem[y][x]	= block y, tile x			( TILE )
*/
#define tile8_mem		((CHARBLOCK8*)MEM_VRAM)

//!	Object charblocks, 4bpp tiles.
/*!	tile_mem[y]		= charblock y				( TILE[ ] )<br>
	tile_mem[y][x]	= block y, tile x			( TILE )
*/
#define tile_mem_obj	( (CHARBLOCK*)MEM_VRAM_OBJ)

//!	Object charblocks, 4bpp tiles.
/*!	tile_mem[y]		= charblock y				( TILE[ ] )<br>
	tile_mem[y][x]	= block y, tile x			( TILE )
*/
#define tile8_mem_obj	((CHARBLOCK8*)MEM_VRAM_OBJ)

//! Screenblocks as arrays
/*!	se_mem[y]		= screenblock y				( SCR_ENTRY[ ] )<br>
*	se_mem[y][x]	= screenblock y, entry x	( SCR_ENTRY )
*/
#define se_mem			((SCREENBLOCK*)MEM_VRAM)

//! Screenblock as matrices
/*!	se_mat[s]		= screenblock s					( SCR_ENTRY[ ][ ] )<br>
	se_mat[s][y][x]	= screenblock s, entry (x,y)	( SCR_ENTRY )
*/
#define se_mat			((SCREENMAT*)MEM_VRAM)

//! Main mode 3/5 frame as an array
/*!	vid_mem[i]		= pixel i						( COLOR )
*/
#define vid_mem			((COLOR*)MEM_VRAM)

//! Mode 3 frame as a matrix
/*!	m3_mem[y][x]	= pixel (x, y)					( COLOR )
*/
#define m3_mem			((M3LINE*)MEM_VRAM)

//! Mode 4 first page as a matrix
/*!	m4_mem[y][x]	= pixel (x, y)					( u8 )
*	\note	This is a byte-buffer. Not to be used for writing.
*/
#define m4_mem			((M4LINE*)MEM_VRAM)

//! Mode 5 first page as a matrix
/*!	m5_mem[y][x]	= pixel (x, y)					( COLOR )
*/
#define m5_mem			((M5LINE*)MEM_VRAM)

//! First page array
#define vid_mem_front	((COLOR*)MEM_VRAM)

//! Second page array
#define vid_mem_back	((COLOR*)MEM_VRAM_BACK)

//! Mode 4 second page as a matrix
/*!	m4_mem[y][x]	= pixel (x, y)					( u8 )
*	\note	This is a byte-buffer. Not to be used for writing.
*/
#define m4_mem_back		((M4LINE*)MEM_VRAM_BACK)

//! Mode 5 second page as a matrix
/*!	m5_mem[y][x]	= pixel (x, y)					( COLOR )
*/
#define m5_mem_back		((M5LINE*)MEM_VRAM_BACK)

//! Object attribute memory
/*!	oam_mem[i]		= object i						( OBJ_ATTR )
*/
#define oam_mem			((OBJ_ATTR*)MEM_OAM)
#define obj_mem			((OBJ_ATTR*)MEM_OAM)

//! Object affine memory
/*!	obj_aff_mem[i]		= object matrix i			( OBJ_AFFINE )	
*/
#define obj_aff_mem		((OBJ_AFFINE*)MEM_OAM)

//! ROM pointer
#define rom_mem			((u16*)MEM_ROM)

//! SRAM pointer
#define sram_mem		((u8*)MEM_SRAM)

#endif
