#include "HorizonFontHandler.h"

#include <string.h>

bool horizon_font_handler_init(HorizonFontHandler *handler, Display *display)
{
	static const char *candidates[] = {"fixed", "6x13", "9x15", "monospace"};
	if (handler == NULL || display == NULL) {
		return false;
	}

	handler->font = NULL;
	for (unsigned int index = 0; index < sizeof(candidates) / sizeof(candidates[0]); ++index) {
		handler->font = XLoadQueryFont(display, candidates[index]);
		if (handler->font != NULL) {
			return true;
		}
	}

	return false;
}

void horizon_font_handler_destroy(HorizonFontHandler *handler, Display *display)
{
	if (handler == NULL || display == NULL) {
		return;
	}

	if (handler->font != NULL) {
		XFreeFont(display, handler->font);
		handler->font = NULL;
	}
}

int horizon_font_handler_text_width(const HorizonFontHandler *handler, const char *text)
{
	if (handler == NULL || handler->font == NULL || text == NULL) {
		return 0;
	}

	return XTextWidth(handler->font, text, (int)strlen(text));
}

int horizon_font_handler_ascent(const HorizonFontHandler *handler)
{
	if (handler == NULL || handler->font == NULL) {
		return 0;
	}

	return handler->font->ascent;
}

int horizon_font_handler_descent(const HorizonFontHandler *handler)
{
	if (handler == NULL || handler->font == NULL) {
		return 0;
	}

	return handler->font->descent;
}

void horizon_font_handler_set_gc_font(const HorizonFontHandler *handler, Display *display, GC gc)
{
	if (handler == NULL || handler->font == NULL || display == NULL || gc == 0) {
		return;
	}

	XSetFont(display, gc, handler->font->fid);
}