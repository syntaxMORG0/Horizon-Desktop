#include "HorizonDesktopIcons.h"

#include "XorgMouse.h"

static void draw_terminal_icon(HorizonDesktop *desktop, Drawable target, HorizonRect rect, bool highlighted)
{
	unsigned long shell = highlighted ? desktop->accent : desktop->black;
	unsigned long border = highlighted ? desktop->white : desktop->panel_border;
	horizon_fill_rect(desktop, target, rect.x, rect.y, rect.width, rect.height, shell);
	horizon_draw_rect(desktop, target, rect.x, rect.y, rect.width, rect.height, border);
	horizon_draw_line(desktop, target, rect.x + 8, rect.y + 10, rect.x + 18, rect.y + 16, desktop->white);
	horizon_draw_line(desktop, target, rect.x + 8, rect.y + 16, rect.x + 12, rect.y + 12, desktop->white);
	horizon_draw_line(desktop, target, rect.x + 8, rect.y + 16, rect.x + 14, rect.y + 16, desktop->white);
}

static void draw_browser_icon(HorizonDesktop *desktop, Drawable target, HorizonRect rect, bool highlighted)
{
	unsigned long body = highlighted ? desktop->icon_orange : desktop->icon_orange;
	unsigned long ring = highlighted ? desktop->white : desktop->icon_blue;
	XSetForeground(desktop->display, desktop->gc, body);
	XFillArc(desktop->display, target, desktop->gc, rect.x + 4, rect.y + 4, rect.width - 8, rect.height - 8, 0, 360 * 64);
	XSetForeground(desktop->display, desktop->gc, ring);
	XFillArc(desktop->display, target, desktop->gc, rect.x + 9, rect.y + 9, rect.width - 18, rect.height - 18, 0, 360 * 64);
}

static void draw_files_icon(HorizonDesktop *desktop, Drawable target, HorizonRect rect, bool highlighted)
{
	unsigned long folder = highlighted ? desktop->icon_yellow : desktop->icon_yellow;
	horizon_fill_rect(desktop, target, rect.x, rect.y + 8, rect.width, rect.height - 8, folder);
	horizon_fill_rect(desktop, target, rect.x + 7, rect.y + 3, rect.width / 2, 10, folder);
	horizon_draw_rect(desktop, target, rect.x, rect.y + 8, rect.width, rect.height - 8, desktop->panel_border);
	horizon_draw_rect(desktop, target, rect.x + 7, rect.y + 3, rect.width / 2, 10, desktop->panel_border);
}

void horizon_desktop_icons_draw(HorizonDesktop *desktop, Drawable target)
{
	bool terminal_hot = horizon_mouse_point_in_rect(desktop->mouse_x, desktop->mouse_y, desktop->terminal_launcher_rect);
	bool browser_hot = horizon_mouse_point_in_rect(desktop->mouse_x, desktop->mouse_y, desktop->browser_launcher_rect);
	bool files_hot = horizon_mouse_point_in_rect(desktop->mouse_x, desktop->mouse_y, desktop->files_launcher_rect);

	draw_terminal_icon(desktop, target, desktop->terminal_launcher_rect, terminal_hot);
	draw_browser_icon(desktop, target, desktop->browser_launcher_rect, browser_hot);
	draw_files_icon(desktop, target, desktop->files_launcher_rect, files_hot);
}
