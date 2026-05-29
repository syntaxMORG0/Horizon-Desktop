#include "HorizonCompositor.h"

#include "../desktop/HorizonDesktop.h"

#include <stdio.h>

int horizon_compositor_run(void)
{
	Display *display = XOpenDisplay(NULL);
	if (display == NULL) {
		fprintf(stderr, "Horizon Desktop: unable to open X display\n");
		return 1;
	}

	HorizonDesktop *desktop = horizon_desktop_create(display);
	if (desktop == NULL) {
		XCloseDisplay(display);
		return 1;
	}

	while (desktop->running) {
		XEvent event;
		XNextEvent(display, &event);

		switch (event.type) {
			case Expose:
				if (event.xexpose.count == 0) {
					horizon_desktop_render(desktop);
				}
				break;
			case ConfigureNotify:
				horizon_desktop_resize(desktop, event.xconfigure.width, event.xconfigure.height);
				horizon_desktop_render(desktop);
				break;
			case ButtonPress:
				horizon_desktop_handle_click(desktop, event.xbutton.x, event.xbutton.y);
				horizon_desktop_render(desktop);
				break;
			case MotionNotify:
				horizon_desktop_handle_motion(desktop, event.xmotion.x, event.xmotion.y);
				horizon_desktop_render(desktop);
				break;
			case KeyPress:
				horizon_desktop_handle_key(desktop, &event.xkey);
				horizon_desktop_render(desktop);
				break;
			case ClientMessage:
				if ((Atom)event.xclient.data.l[0] == desktop->wm_delete_window) {
					desktop->running = false;
				}
				break;
			default:
				break;
		}
	}

	horizon_desktop_destroy(desktop);
	XCloseDisplay(display);
	return 0;
}
