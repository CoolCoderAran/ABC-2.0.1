#ifndef ABC_BHDRS_BGFX_HEADER
#define ABC_BHDRS_BGFX_HEADER

/*
 * Shared data between graphics routines.
 */

typedef struct ivector
{
	int x;
	int y;
} ivector;

extern ivector dev_origin; /* Lower left corner of device */
extern ivector dev_corner; /* Upper right corner of device */

extern bool enter_gfx(void);
extern Procedure exit_gfx(void);

extern int gfx_mode;
#define TEXT_MODE 1
#define GFX_MODE 2
#define SPLIT_MODE 3

#endif /* HEADER GUARD */
