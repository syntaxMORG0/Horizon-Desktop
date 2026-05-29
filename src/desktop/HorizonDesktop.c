#include "HorizonDesktop.h"

#include "HorizonDesktopIcons.h"
#include "HorizonWallpapers.h"
#include "XorgMouse.h"
#include "../shell/HorizonFontHandler.h"
#include "../shell/HorizonStartmenu.h"
#include "../shell/HorizonTaskbar.h"
#include "../shell/HorizonWindow.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <X11/keysym.h>

static unsigned long horizon_color_from_rgb(Display *display, int screen, unsigned char red, unsigned char green, unsigned char blue)
{
	Visual *visual = XDefaultVisual(display, screen);

	if (visual->class == TrueColor || visual->class == DirectColor) {
		unsigned long pixel = 0;
		unsigned long red_mask = visual->red_mask;
		unsigned long green_mask = visual->green_mask;
		unsigned long blue_mask = visual->blue_mask;

		int red_shift = 0;
		int green_shift = 0;
		int blue_shift = 0;

		while (((red_mask >> red_shift) & 1UL) == 0UL && red_shift < 32) {
			++red_shift;
		}
		while (((green_mask >> green_shift) & 1UL) == 0UL && green_shift < 32) {
			++green_shift;
		}
		while (((blue_mask >> blue_shift) & 1UL) == 0UL && blue_shift < 32) {
			++blue_shift;
		}

		int red_bits = 0;
		int green_bits = 0;
		int blue_bits = 0;
		for (unsigned long mask = red_mask >> red_shift; mask != 0UL; mask >>= 1UL) {
			++red_bits;
		}
		for (unsigned long mask = green_mask >> green_shift; mask != 0UL; mask >>= 1UL) {
			++green_bits;
		}
		for (unsigned long mask = blue_mask >> blue_shift; mask != 0UL; mask >>= 1UL) {
			++blue_bits;
		}

		unsigned long red_value = ((unsigned long)red << red_bits) / 256UL;
		unsigned long green_value = ((unsigned long)green << green_bits) / 256UL;
		unsigned long blue_value = ((unsigned long)blue << blue_bits) / 256UL;

		pixel |= (red_value << red_shift) & red_mask;
		pixel |= (green_value << green_shift) & green_mask;
		pixel |= (blue_value << blue_shift) & blue_mask;
		return pixel;
	}

	XColor color;
	color.red = (unsigned short)(red * 257U);
	color.green = (unsigned short)(green * 257U);
	color.blue = (unsigned short)(blue * 257U);
	color.flags = DoRed | DoGreen | DoBlue;
	XAllocColor(display, DefaultColormap(display, screen), &color);
	return color.pixel;
}

Bool horizon_point_in_rect(int x, int y, HorizonRect rect)
{
	return x >= rect.x && x < rect.x + rect.width && y >= rect.y && y < rect.y + rect.height;
}

void horizon_fill_rect(HorizonDesktop *desktop, Drawable target, int x, int y, int width, int height, unsigned long color)
{
	XSetForeground(desktop->display, desktop->gc, color);
	XFillRectangle(desktop->display, target, desktop->gc, x, y, (unsigned int)width, (unsigned int)height);
}

void horizon_draw_rect(HorizonDesktop *desktop, Drawable target, int x, int y, int width, int height, unsigned long color)
{
	XSetForeground(desktop->display, desktop->gc, color);
	XDrawRectangle(desktop->display, target, desktop->gc, x, y, (unsigned int)(width - 1), (unsigned int)(height - 1));
}

void horizon_draw_line(HorizonDesktop *desktop, Drawable target, int x1, int y1, int x2, int y2, unsigned long color)
{
	XSetForeground(desktop->display, desktop->gc, color);
	XDrawLine(desktop->display, target, desktop->gc, x1, y1, x2, y2);
}

int horizon_text_width(HorizonDesktop *desktop, const char *text)
{
	if (text == NULL) {
		return 0;
	}

	return horizon_font_handler_text_width(&desktop->font_handler, text);
}

void horizon_draw_text(HorizonDesktop *desktop, Drawable target, int x, int y, const char *text, unsigned long color)
{
	if (text == NULL) {
		return;
	}

	XSetForeground(desktop->display, desktop->gc, color);
	horizon_font_handler_set_gc_font(&desktop->font_handler, desktop->display, desktop->gc);
	XDrawString(desktop->display, target, desktop->gc, x, y, text, (int)strlen(text));
}

void horizon_draw_text_centered(HorizonDesktop *desktop, Drawable target, HorizonRect rect, const char *text, unsigned long color)
{
	int text_width = horizon_text_width(desktop, text);
	int x = rect.x + (rect.width - text_width) / 2;
	int y = rect.y + (rect.height + horizon_font_handler_ascent(&desktop->font_handler) - horizon_font_handler_descent(&desktop->font_handler)) / 2;

	horizon_draw_text(desktop, target, x, y, text, color);
}

static void horizon_desktop_set_palette(HorizonDesktop *desktop)
{
	desktop->background_top = horizon_color_from_rgb(desktop->display, desktop->screen, 44, 71, 150);
	desktop->background_high = horizon_color_from_rgb(desktop->display, desktop->screen, 85, 95, 175);
	desktop->background_mid = horizon_color_from_rgb(desktop->display, desktop->screen, 126, 95, 185);
	desktop->background_warm = horizon_color_from_rgb(desktop->display, desktop->screen, 200, 110, 146);
	desktop->background_horizon = horizon_color_from_rgb(desktop->display, desktop->screen, 247, 181, 123);
	desktop->water_deep = horizon_color_from_rgb(desktop->display, desktop->screen, 46, 40, 86);
	desktop->water_mid = horizon_color_from_rgb(desktop->display, desktop->screen, 88, 73, 132);
	desktop->water_light = horizon_color_from_rgb(desktop->display, desktop->screen, 134, 100, 165);
	desktop->water_haze = horizon_color_from_rgb(desktop->display, desktop->screen, 176, 143, 200);
	desktop->mountain = horizon_color_from_rgb(desktop->display, desktop->screen, 56, 43, 96);
	desktop->mountain_shadow = horizon_color_from_rgb(desktop->display, desktop->screen, 36, 29, 69);
	desktop->mountain_snow = horizon_color_from_rgb(desktop->display, desktop->screen, 248, 244, 244);
	desktop->snow = horizon_color_from_rgb(desktop->display, desktop->screen, 255, 250, 246);
	desktop->tree = horizon_color_from_rgb(desktop->display, desktop->screen, 34, 20, 34);
	desktop->blossom = horizon_color_from_rgb(desktop->display, desktop->screen, 255, 137, 176);
	desktop->petal = horizon_color_from_rgb(desktop->display, desktop->screen, 255, 182, 206);
	desktop->taskbar = horizon_color_from_rgb(desktop->display, desktop->screen, 245, 245, 245);
	desktop->panel = horizon_color_from_rgb(desktop->display, desktop->screen, 241, 234, 233);
	desktop->panel_border = horizon_color_from_rgb(desktop->display, desktop->screen, 182, 162, 184);
	desktop->text = horizon_color_from_rgb(desktop->display, desktop->screen, 31, 25, 36);
	desktop->muted_text = horizon_color_from_rgb(desktop->display, desktop->screen, 108, 98, 122);
	desktop->terminal_background = horizon_color_from_rgb(desktop->display, desktop->screen, 4, 4, 4);
	desktop->terminal_border = horizon_color_from_rgb(desktop->display, desktop->screen, 186, 186, 186);
	desktop->terminal_text = horizon_color_from_rgb(desktop->display, desktop->screen, 246, 244, 240);
	desktop->white = horizon_color_from_rgb(desktop->display, desktop->screen, 255, 255, 255);
	desktop->black = horizon_color_from_rgb(desktop->display, desktop->screen, 0, 0, 0);
	desktop->accent = horizon_color_from_rgb(desktop->display, desktop->screen, 246, 122, 159);
	desktop->icon_blue = horizon_color_from_rgb(desktop->display, desktop->screen, 76, 136, 232);
	desktop->icon_orange = horizon_color_from_rgb(desktop->display, desktop->screen, 243, 156, 18);
	desktop->icon_yellow = horizon_color_from_rgb(desktop->display, desktop->screen, 230, 184, 78);
}

static void horizon_desktop_set_layout(HorizonDesktop *desktop)
{
	desktop->taskbar_height = 56;
	desktop->taskbar_rect = (HorizonRect){0, desktop->height - desktop->taskbar_height, desktop->width, desktop->taskbar_height};
	desktop->start_button_rect = (HorizonRect){14, desktop->taskbar_rect.y + 10, 88, 36};
	desktop->terminal_launcher_rect = (HorizonRect){114, desktop->taskbar_rect.y + 8, 40, 40};
	desktop->browser_launcher_rect = (HorizonRect){166, desktop->taskbar_rect.y + 8, 40, 40};
	desktop->files_launcher_rect = (HorizonRect){218, desktop->taskbar_rect.y + 8, 40, 40};
	desktop->clock_rect = (HorizonRect){desktop->width - 102, desktop->taskbar_rect.y + 8, 88, 40};

	int terminal_width = desktop->width > 860 ? 700 : desktop->width - 160;
	int terminal_height = desktop->height > 620 ? 420 : desktop->height - desktop->taskbar_height - 110;
	if (terminal_width < 420) {
		terminal_width = 420;
	}
	if (terminal_height < 260) {
		terminal_height = 260;
	}

	int terminal_x = (desktop->width - terminal_width) / 2;
	if (terminal_x < 24) {
		terminal_x = 24;
	}
	int terminal_y = 104;
	if (terminal_y + terminal_height > desktop->height - desktop->taskbar_height - 20) {
		terminal_y = desktop->height - desktop->taskbar_height - terminal_height - 20;
	}
	if (terminal_y < 24) {
		terminal_y = 24;
	}

	desktop->terminal_rect = (HorizonRect){terminal_x, terminal_y, terminal_width, terminal_height};
	desktop->terminal_close_rect = (HorizonRect){terminal_x + terminal_width - 30, terminal_y + 2, 28, 22};

	int menu_width = 260;
	int menu_height = 268;
	int menu_x = desktop->start_button_rect.x;
	int menu_y = desktop->taskbar_rect.y - menu_height - 12;
	if (menu_y < 20) {
		menu_y = 20;
	}
	desktop->start_menu_rect = (HorizonRect){menu_x, menu_y, menu_width, menu_height};
	desktop->start_menu_terminal_rect = (HorizonRect){menu_x + 14, menu_y + 78, menu_width - 28, 38};
	desktop->start_menu_browser_rect = (HorizonRect){menu_x + 14, menu_y + 120, menu_width - 28, 38};
	desktop->start_menu_files_rect = (HorizonRect){menu_x + 14, menu_y + 162, menu_width - 28, 38};
	desktop->start_menu_shutdown_rect = (HorizonRect){menu_x + 14, menu_y + 204, menu_width - 28, 38};
}

void horizon_desktop_update_clock(HorizonDesktop *desktop)
{
	time_t now = time(NULL);
	struct tm *local_time = localtime(&now);

	if (local_time != NULL) {
		strftime(desktop->clock_text, sizeof(desktop->clock_text), "%H:%M", local_time);
	} else {
		strncpy(desktop->clock_text, "16:23", sizeof(desktop->clock_text));
		desktop->clock_text[sizeof(desktop->clock_text) - 1] = '\0';
	}
}

HorizonDesktop *horizon_desktop_create(Display *display)
{
	HorizonDesktop *desktop = calloc(1, sizeof(*desktop));
	if (desktop == NULL) {
		return NULL;
	}

	desktop->display = display;
	desktop->screen = DefaultScreen(display);
	desktop->root = RootWindow(display, desktop->screen);
	desktop->colormap = DefaultColormap(display, desktop->screen);
	desktop->width = DisplayWidth(display, desktop->screen);
	desktop->height = DisplayHeight(display, desktop->screen);
	desktop->running = true;
	desktop->start_menu_open = false;
	desktop->terminal_visible = true;
	desktop->mouse_x = 0;
	desktop->mouse_y = 0;

	horizon_desktop_set_palette(desktop);

	horizon_font_handler_init(&desktop->font_handler, display);

	XSetWindowAttributes attributes;
	attributes.override_redirect = True;
	attributes.background_pixel = desktop->black;
	attributes.event_mask = ExposureMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | KeyPressMask | StructureNotifyMask;

	desktop->window = XCreateWindow(display, desktop->root, 0, 0, (unsigned int)desktop->width, (unsigned int)desktop->height, 0,
		CopyFromParent, InputOutput, CopyFromParent, CWOverrideRedirect | CWBackPixel | CWEventMask, &attributes);

	if (desktop->window == 0) {
		horizon_desktop_destroy(desktop);
		return NULL;
	}

	desktop->gc = XCreateGC(display, desktop->window, 0, NULL);
	if (desktop->gc == 0) {
		horizon_desktop_destroy(desktop);
		return NULL;
	}

	horizon_font_handler_set_gc_font(&desktop->font_handler, display, desktop->gc);

	desktop->backbuffer = XCreatePixmap(display, desktop->window, (unsigned int)desktop->width, (unsigned int)desktop->height, (unsigned int)DefaultDepth(display, desktop->screen));

	desktop->wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(display, desktop->window, &desktop->wm_delete_window, 1);
	XStoreName(display, desktop->window, "Horizon Desktop");

	horizon_desktop_set_layout(desktop);
	horizon_desktop_update_clock(desktop);

	XMapRaised(display, desktop->window);
	XSetInputFocus(display, desktop->window, RevertToParent, CurrentTime);

	horizon_desktop_render(desktop);
	return desktop;
}

void horizon_desktop_destroy(HorizonDesktop *desktop)
{
	if (desktop == NULL) {
		return;
	}

	if (desktop->display != NULL && desktop->backbuffer != 0) {
		XFreePixmap(desktop->display, desktop->backbuffer);
	}
	if (desktop->display != NULL && desktop->gc != 0) {
		XFreeGC(desktop->display, desktop->gc);
	}
	if (desktop->display != NULL && desktop->window != 0) {
		XDestroyWindow(desktop->display, desktop->window);
	}
	horizon_font_handler_destroy(&desktop->font_handler, desktop->display);

	free(desktop);
}

void horizon_desktop_resize(HorizonDesktop *desktop, int width, int height)
{
	if (desktop == NULL || width <= 0 || height <= 0) {
		return;
	}

	desktop->width = width;
	desktop->height = height;
	horizon_desktop_set_layout(desktop);
	horizon_desktop_update_clock(desktop);

	if (desktop->backbuffer != 0) {
		XFreePixmap(desktop->display, desktop->backbuffer);
	}
	desktop->backbuffer = XCreatePixmap(desktop->display, desktop->window, (unsigned int)desktop->width, (unsigned int)desktop->height, (unsigned int)DefaultDepth(desktop->display, desktop->screen));
}

void horizon_desktop_render(HorizonDesktop *desktop)
{
	if (desktop == NULL || desktop->display == NULL || desktop->window == 0 || desktop->backbuffer == 0) {
		return;
	}

	horizon_desktop_update_clock(desktop);
	horizon_fill_rect(desktop, desktop->backbuffer, 0, 0, desktop->width, desktop->height, desktop->black);
	horizon_wallpaper_draw(desktop, desktop->backbuffer);
	horizon_taskbar_draw(desktop, desktop->backbuffer);
	horizon_desktop_icons_draw(desktop, desktop->backbuffer);
	horizon_window_draw(desktop, desktop->backbuffer);
	horizon_startmenu_draw(desktop, desktop->backbuffer);

	XCopyArea(desktop->display, desktop->backbuffer, desktop->window, desktop->gc, 0, 0, (unsigned int)desktop->width, (unsigned int)desktop->height, 0, 0);
	XFlush(desktop->display);
}

void horizon_desktop_handle_motion(HorizonDesktop *desktop, int x, int y)
{
	if (desktop == NULL) {
		return;
	}

	desktop->mouse_x = x;
	desktop->mouse_y = y;
}

static void horizon_desktop_activate_menu_item(HorizonDesktop *desktop, HorizonRect item)
{
	if (horizon_mouse_point_in_rect(desktop->mouse_x, desktop->mouse_y, item)) {
		desktop->start_menu_open = false;
	}
}

void horizon_desktop_handle_click(HorizonDesktop *desktop, int x, int y)
{
	if (desktop == NULL) {
		return;
	}

	desktop->mouse_x = x;
	desktop->mouse_y = y;

	if (horizon_mouse_point_in_rect(x, y, desktop->terminal_visible ? desktop->terminal_close_rect : (HorizonRect){-1, -1, 0, 0})) {
		desktop->terminal_visible = false;
		return;
	}

	if (horizon_mouse_point_in_rect(x, y, desktop->start_button_rect)) {
		desktop->start_menu_open = !desktop->start_menu_open;
		return;
	}

	if (desktop->start_menu_open) {
		if (horizon_mouse_point_in_rect(x, y, desktop->start_menu_terminal_rect)) {
			desktop->terminal_visible = true;
			desktop->start_menu_open = false;
			return;
		}
		if (horizon_mouse_point_in_rect(x, y, desktop->start_menu_shutdown_rect)) {
			desktop->running = false;
			return;
		}

		horizon_desktop_activate_menu_item(desktop, desktop->start_menu_browser_rect);
		horizon_desktop_activate_menu_item(desktop, desktop->start_menu_files_rect);

		if (!horizon_mouse_point_in_rect(x, y, desktop->start_menu_rect)) {
			desktop->start_menu_open = false;
		}
		return;
	}

	if (horizon_mouse_point_in_rect(x, y, desktop->terminal_launcher_rect)) {
		desktop->terminal_visible = !desktop->terminal_visible;
		return;
	}
}

void horizon_desktop_handle_key(HorizonDesktop *desktop, XKeyEvent *event)
{
	if (desktop == NULL || event == NULL) {
		return;
	}

	KeySym key = XLookupKeysym(event, 0);
	if (key == XK_Escape || key == XK_q) {
		desktop->running = false;
		return;
	}
	if (key == XK_s) {
		desktop->start_menu_open = !desktop->start_menu_open;
		return;
	}
	if (key == XK_t) {
		desktop->terminal_visible = !desktop->terminal_visible;
	}
}
