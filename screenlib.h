#include <dc/biosfont.h>
#include <dc/video.h>
#include <dc/maple/controller.h>

#include <arch/arch.h>

#include <unistd.h>
#include <kos.h>
#include <kos/string.h>
#define WIDTH 640
#define HEIGHT 480
#define STARTLINE 20
#define CHARSPERLINE 40
#define CHARSPERTEST 120

#include <assert.h>


extern uint16		*vram_s;
int x, y, o;
void initScreen() {
    for(y = 0; y < 480; y++)
        for(x = 0; x < 640; x++) {
            int c = (x ^ y) & 255;
            vram_s[y * 640 + x] = ((c >> 3) << 12)
                                  | ((c >> 2) << 5)
                                  | ((c >> 3) << 0);
        }

    /* Set our starting offset to one letter height away from the 
       top of the screen and two widths from the left */
    o = (640 * BFONT_HEIGHT) + (BFONT_THIN_WIDTH * 2);
	/* Test with ISO8859-1 encoding */
    bfont_set_encoding(BFONT_CODE_ISO8859_1);
	bfont_draw_str(vram_s + o, 640, 1, "screenlib.h init completed");
}
void writeScreen(const char texttowrite) {
	 o += 640 * BFONT_HEIGHT;
	bfont_draw_str(vram_s + o, 640, 1, texttowrite);
}