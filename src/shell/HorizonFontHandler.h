#ifndef HORIZON_FONT_HANDLER_H
#define HORIZON_FONT_HANDLER_H

#include <stdbool.h>
#include <X11/Xlib.h>

typedef struct HorizonFontHandler {
	XFontStruct *font;
} HorizonFontHandler;

bool horizon_font_handler_init(HorizonFontHandler *handler, Display *display);
void horizon_font_handler_destroy(HorizonFontHandler *handler, Display *display);
int horizon_font_handler_text_width(const HorizonFontHandler *handler, const char *text);
int horizon_font_handler_ascent(const HorizonFontHandler *handler);
int horizon_font_handler_descent(const HorizonFontHandler *handler);
void horizon_font_handler_set_gc_font(const HorizonFontHandler *handler, Display *display, GC gc);

#endif