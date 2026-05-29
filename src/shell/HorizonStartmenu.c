#include "HorizonStartmenu.h"

#include "../desktop/XorgMouse.h"

static void draw_item(HorizonDesktop *desktop, Drawable target, HorizonRect rect, const char *label, bool active)
{
	unsigned long fill = active ? desktop->panel_border : desktop->panel;
	unsigned long text = active ? desktop->white : desktop->text;

	horizon_fill_rect(desktop, target, rect.x, rect.y, rect.width, rect.height, fill);
	horizon_draw_rect(desktop, target, rect.x, rect.y, rect.width, rect.height, desktop->panel_border);
	horizon_draw_text(desktop, target, rect.x + 18, rect.y + 26, label, text);
}

void horizon_startmenu_draw(HorizonDesktop *desktop, Drawable target)
{
	if (!desktop->start_menu_open) {
		return;
	}

	horizon_fill_rect(desktop, target, desktop->start_menu_rect.x, desktop->start_menu_rect.y, desktop->start_menu_rect.width, desktop->start_menu_rect.height, desktop->panel);
	horizon_draw_rect(desktop, target, desktop->start_menu_rect.x, desktop->start_menu_rect.y, desktop->start_menu_rect.width, desktop->start_menu_rect.height, desktop->panel_border);
	horizon_draw_text(desktop, target, desktop->start_menu_rect.x + 18, desktop->start_menu_rect.y + 28, "Horizon", desktop->text);
	horizon_draw_text(desktop, target, desktop->start_menu_rect.x + 18, desktop->start_menu_rect.y + 46, "Select a launcher", desktop->muted_text);

	draw_item(desktop, target, desktop->start_menu_terminal_rect, "Terminal", horizon_mouse_point_in_rect(desktop->mouse_x, desktop->mouse_y, desktop->start_menu_terminal_rect));
	draw_item(desktop, target, desktop->start_menu_browser_rect, "Browser", horizon_mouse_point_in_rect(desktop->mouse_x, desktop->mouse_y, desktop->start_menu_browser_rect));
	draw_item(desktop, target, desktop->start_menu_files_rect, "Files", horizon_mouse_point_in_rect(desktop->mouse_x, desktop->mouse_y, desktop->start_menu_files_rect));
	draw_item(desktop, target, desktop->start_menu_shutdown_rect, "Shutdown", horizon_mouse_point_in_rect(desktop->mouse_x, desktop->mouse_y, desktop->start_menu_shutdown_rect));
}
