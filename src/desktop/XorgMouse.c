#include "XorgMouse.h"

Bool horizon_mouse_point_in_rect(int x, int y, HorizonRect rect)
{
	return x >= rect.x && x < rect.x + rect.width && y >= rect.y && y < rect.y + rect.height;
}
