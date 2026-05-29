#ifndef HORIZON_DESKTOP_H
#define HORIZON_DESKTOP_H

#include <stdbool.h>
#include <X11/Xlib.h>

#include "../shell/HorizonFontHandler.h"

typedef struct HorizonRect {
	int x;
	int y;
	int width;
	int height;
} HorizonRect;

typedef struct HorizonDesktop {
	Display *display;
	int screen;
	Window root;
	Window window;
	Colormap colormap;
	GC gc;
	HorizonFontHandler font_handler;
	Atom wm_delete_window;
	Pixmap backbuffer;
	int width;
	int height;
	int taskbar_height;
	bool running;
	bool start_menu_open;
	bool terminal_visible;
	int mouse_x;
	int mouse_y;
	char clock_text[16];
	unsigned long background_top;
	unsigned long background_high;
	unsigned long background_mid;
	unsigned long background_warm;
	unsigned long background_horizon;
	unsigned long water_deep;
	unsigned long water_mid;
	unsigned long water_light;
	unsigned long water_haze;
	unsigned long mountain;
	unsigned long mountain_shadow;
	unsigned long mountain_snow;
	unsigned long snow;
	unsigned long tree;
	unsigned long blossom;
	unsigned long petal;
	unsigned long taskbar;
	unsigned long panel;
	unsigned long panel_border;
	unsigned long text;
	unsigned long muted_text;
	unsigned long terminal_background;
	unsigned long terminal_border;
	unsigned long terminal_text;
	unsigned long white;
	unsigned long black;
	unsigned long accent;
	unsigned long icon_blue;
	unsigned long icon_orange;
	unsigned long icon_yellow;
	HorizonRect taskbar_rect;
	HorizonRect start_button_rect;
	HorizonRect terminal_launcher_rect;
	HorizonRect browser_launcher_rect;
	HorizonRect files_launcher_rect;
	HorizonRect clock_rect;
	HorizonRect terminal_rect;
	HorizonRect terminal_close_rect;
	HorizonRect start_menu_rect;
	HorizonRect start_menu_terminal_rect;
	HorizonRect start_menu_browser_rect;
	HorizonRect start_menu_files_rect;
	HorizonRect start_menu_shutdown_rect;
} HorizonDesktop;

Bool horizon_point_in_rect(int x, int y, HorizonRect rect);
void horizon_fill_rect(HorizonDesktop *desktop, Drawable target, int x, int y, int width, int height, unsigned long color);
void horizon_draw_rect(HorizonDesktop *desktop, Drawable target, int x, int y, int width, int height, unsigned long color);
void horizon_draw_line(HorizonDesktop *desktop, Drawable target, int x1, int y1, int x2, int y2, unsigned long color);
void horizon_draw_text(HorizonDesktop *desktop, Drawable target, int x, int y, const char *text, unsigned long color);
void horizon_draw_text_centered(HorizonDesktop *desktop, Drawable target, HorizonRect rect, const char *text, unsigned long color);
int horizon_text_width(HorizonDesktop *desktop, const char *text);

HorizonDesktop *horizon_desktop_create(Display *display);
void horizon_desktop_destroy(HorizonDesktop *desktop);
void horizon_desktop_resize(HorizonDesktop *desktop, int width, int height);
void horizon_desktop_render(HorizonDesktop *desktop);
void horizon_desktop_handle_click(HorizonDesktop *desktop, int x, int y);
void horizon_desktop_handle_motion(HorizonDesktop *desktop, int x, int y);
void horizon_desktop_handle_key(HorizonDesktop *desktop, XKeyEvent *event);
void horizon_desktop_update_clock(HorizonDesktop *desktop);

#endif
