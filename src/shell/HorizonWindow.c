#include "HorizonWindow.h"

#include "../desktop/XorgMouse.h"

static void draw_terminal_line(HorizonDesktop *desktop, Drawable target, int x, int y, const char *text)
{
	horizon_draw_text(desktop, target, x, y, text, desktop->terminal_text);
}

void horizon_window_draw(HorizonDesktop *desktop, Drawable target)
{
	if (!desktop->terminal_visible) {
		return;
	}

	horizon_fill_rect(desktop, target, desktop->terminal_rect.x, desktop->terminal_rect.y, desktop->terminal_rect.width, desktop->terminal_rect.height, desktop->terminal_background);
	horizon_draw_rect(desktop, target, desktop->terminal_rect.x, desktop->terminal_rect.y, desktop->terminal_rect.width, desktop->terminal_rect.height, desktop->terminal_border);

	horizon_fill_rect(desktop, target, desktop->terminal_rect.x, desktop->terminal_rect.y, desktop->terminal_rect.width, 28, desktop->white);
	horizon_draw_text(desktop, target, desktop->terminal_rect.x + 18, desktop->terminal_rect.y + 20, "Xterm - ~/", desktop->text);

	bool close_hot = horizon_mouse_point_in_rect(desktop->mouse_x, desktop->mouse_y, desktop->terminal_close_rect);
	unsigned long close_fill = close_hot ? desktop->accent : desktop->white;
	horizon_fill_rect(desktop, target, desktop->terminal_close_rect.x, desktop->terminal_close_rect.y, desktop->terminal_close_rect.width, desktop->terminal_close_rect.height, close_fill);
	horizon_draw_rect(desktop, target, desktop->terminal_close_rect.x, desktop->terminal_close_rect.y, desktop->terminal_close_rect.width, desktop->terminal_close_rect.height, desktop->panel_border);
	horizon_draw_text_centered(desktop, target, desktop->terminal_close_rect, "X", close_hot ? desktop->white : desktop->text);

	draw_terminal_line(desktop, target, desktop->terminal_rect.x + 18, desktop->terminal_rect.y + 64, "~ > which $SHELL");
	draw_terminal_line(desktop, target, desktop->terminal_rect.x + 18, desktop->terminal_rect.y + 96, "/bin/bash");
	draw_terminal_line(desktop, target, desktop->terminal_rect.x + 18, desktop->terminal_rect.y + 130, "~ > echo hello from horizon");
	draw_terminal_line(desktop, target, desktop->terminal_rect.x + 18, desktop->terminal_rect.y + 162, "hello from horizon");
	draw_terminal_line(desktop, target, desktop->terminal_rect.x + 18, desktop->terminal_rect.y + 196, "~ > sudo systemctl enable --now tailscaled");
	draw_terminal_line(desktop, target, desktop->terminal_rect.x + 18, desktop->terminal_rect.y + 228, "created symlink /usr/lib/tailscaled");
	draw_terminal_line(desktop, target, desktop->terminal_rect.x + 18, desktop->terminal_rect.y + 262, "~ > tailscale up");
	draw_terminal_line(desktop, target, desktop->terminal_rect.x + 18, desktop->terminal_rect.y + 294, "ssh token accepted logging into noahnet.tailnet");
}
