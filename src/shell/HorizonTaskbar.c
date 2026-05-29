#include "HorizonTaskbar.h"

#include "../desktop/XorgMouse.h"

void horizon_taskbar_draw(HorizonDesktop *desktop, Drawable target)
{
	horizon_fill_rect(desktop, target, desktop->taskbar_rect.x, desktop->taskbar_rect.y, desktop->taskbar_rect.width, desktop->taskbar_rect.height, desktop->taskbar);
	horizon_draw_line(desktop, target, 0, desktop->taskbar_rect.y, desktop->width, desktop->taskbar_rect.y, desktop->panel_border);

	bool start_hot = horizon_mouse_point_in_rect(desktop->mouse_x, desktop->mouse_y, desktop->start_button_rect);
	unsigned long start_fill = start_hot || desktop->start_menu_open ? desktop->panel : desktop->white;
	unsigned long start_border = start_hot || desktop->start_menu_open ? desktop->accent : desktop->panel_border;
	horizon_fill_rect(desktop, target, desktop->start_button_rect.x, desktop->start_button_rect.y, desktop->start_button_rect.width, desktop->start_button_rect.height, start_fill);
	horizon_draw_rect(desktop, target, desktop->start_button_rect.x, desktop->start_button_rect.y, desktop->start_button_rect.width, desktop->start_button_rect.height, start_border);
	horizon_draw_text_centered(desktop, target, desktop->start_button_rect, "Start", desktop->text);

	horizon_draw_text_centered(desktop, target, desktop->clock_rect, desktop->clock_text, desktop->text);

	horizon_draw_text(desktop, target, desktop->width - 196, desktop->taskbar_rect.y + 20, "Horizon Desktop", desktop->muted_text);
}
