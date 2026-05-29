#include "HorizonWallpapers.h"

static void draw_cloud(HorizonDesktop *desktop, Drawable target, int x, int y, unsigned long color)
{
	XSetForeground(desktop->display, desktop->gc, color);
	XFillArc(desktop->display, target, desktop->gc, x, y, 180, 70, 0, 360 * 64);
	XFillArc(desktop->display, target, desktop->gc, x + 72, y - 12, 140, 64, 0, 360 * 64);
	XFillArc(desktop->display, target, desktop->gc, x + 132, y + 4, 156, 58, 0, 360 * 64);
}

static void draw_petals(HorizonDesktop *desktop, Drawable target)
{
	const int petals[][2] = {
		{102, 162}, {154, 124}, {218, 182}, {304, 88}, {382, 140}, {456, 110}, {510, 160}, {604, 76},
		{714, 164}, {846, 92}, {916, 138}, {1030, 124}, {1090, 172}, {1182, 104}, {1240, 150}
	};

	XSetForeground(desktop->display, desktop->gc, desktop->petal);
	for (unsigned int index = 0; index < sizeof(petals) / sizeof(petals[0]); ++index) {
		XFillArc(desktop->display, target, desktop->gc, petals[index][0], petals[index][1], 8, 5, 0, 360 * 64);
	}
}

static void draw_tree(HorizonDesktop *desktop, Drawable target)
{
	XSetForeground(desktop->display, desktop->gc, desktop->tree);

	XPoint trunk[] = {
		{24, desktop->height - 90}, {52, desktop->height - 170}, {58, desktop->height - 236},
		{84, desktop->height - 350}, {110, desktop->height - 460}, {136, desktop->height - 582},
		{168, desktop->height - 620}, {182, desktop->height - 586}, {162, desktop->height - 476},
		{138, desktop->height - 382}, {120, desktop->height - 286}, {88, desktop->height - 178},
		{62, desktop->height - 100}
	};
	XFillPolygon(desktop->display, target, desktop->gc, trunk, (int)(sizeof(trunk) / sizeof(trunk[0])), Complex, CoordModeOrigin);

	const struct {
		int x1, y1, x2, y2;
	} branches[] = {
		{104, desktop->height - 398, 176, desktop->height - 444},
		{126, desktop->height - 458, 224, desktop->height - 496},
		{148, desktop->height - 512, 242, desktop->height - 560},
		{116, desktop->height - 332, 38, desktop->height - 424},
		{76, desktop->height - 254, 0, desktop->height - 292},
		{62, desktop->height - 194, 12, desktop->height - 154}
	};

	for (unsigned int index = 0; index < sizeof(branches) / sizeof(branches[0]); ++index) {
		XDrawLine(desktop->display, target, desktop->gc, branches[index].x1, branches[index].y1, branches[index].x2, branches[index].y2);
	}

	XSetForeground(desktop->display, desktop->gc, desktop->blossom);
	const int blossoms[][2] = {
		{30, desktop->height - 452}, {52, desktop->height - 410}, {78, desktop->height - 368}, {100, desktop->height - 324},
		{126, desktop->height - 286}, {154, desktop->height - 250}, {178, desktop->height - 218}, {204, desktop->height - 184},
		{234, desktop->height - 158}, {262, desktop->height - 136}, {290, desktop->height - 114}, {318, desktop->height - 128},
		{338, desktop->height - 164}, {360, desktop->height - 196}, {382, desktop->height - 228}, {410, desktop->height - 250},
		{438, desktop->height - 276}, {470, desktop->height - 292}, {496, desktop->height - 312}, {522, desktop->height - 336},
		{552, desktop->height - 350}, {588, desktop->height - 364}, {620, desktop->height - 380}, {660, desktop->height - 394}
	};

	for (unsigned int index = 0; index < sizeof(blossoms) / sizeof(blossoms[0]); ++index) {
		XFillArc(desktop->display, target, desktop->gc, blossoms[index][0], blossoms[index][1], 14, 14, 0, 360 * 64);
	}
}

static void draw_mountain(HorizonDesktop *desktop, Drawable target)
{
	XPoint shadow[] = {
		{702, desktop->height - 255}, {828, desktop->height - 446}, {946, desktop->height - 470},
		{1128, desktop->height - 265}, {1044, desktop->height - 220}, {892, desktop->height - 232}
	};

	XPoint main[] = {
		{690, desktop->height - 246}, {836, desktop->height - 458}, {958, desktop->height - 474},
		{1118, desktop->height - 286}, {1032, desktop->height - 206}, {888, desktop->height - 220}, {766, desktop->height - 220}
	};

	XPoint snow[] = {
		{826, desktop->height - 458}, {878, desktop->height - 522}, {948, desktop->height - 478}, {898, desktop->height - 448}
	};

	XSetForeground(desktop->display, desktop->gc, desktop->mountain_shadow);
	XFillPolygon(desktop->display, target, desktop->gc, shadow, (int)(sizeof(shadow) / sizeof(shadow[0])), Convex, CoordModeOrigin);

	XSetForeground(desktop->display, desktop->gc, desktop->mountain);
	XFillPolygon(desktop->display, target, desktop->gc, main, (int)(sizeof(main) / sizeof(main[0])), Convex, CoordModeOrigin);

	XSetForeground(desktop->display, desktop->gc, desktop->mountain_snow);
	XFillPolygon(desktop->display, target, desktop->gc, snow, (int)(sizeof(snow) / sizeof(snow[0])), Convex, CoordModeOrigin);

	XSetForeground(desktop->display, desktop->gc, desktop->snow);
	XFillArc(desktop->display, target, desktop->gc, 834, desktop->height - 468, 46, 46, 0, 360 * 64);
}

static void draw_reflection(HorizonDesktop *desktop, Drawable target)
{
	const int horizon = desktop->height - desktop->taskbar_height - 240;

	XSetForeground(desktop->display, desktop->gc, desktop->water_deep);
	XFillRectangle(desktop->display, target, desktop->gc, 0, horizon, desktop->width, desktop->height - desktop->taskbar_height - horizon);

	XSetForeground(desktop->display, desktop->gc, desktop->water_mid);
	for (int band = 0; band < 6; ++band) {
		int band_y = horizon + band * 28;
		XFillRectangle(desktop->display, target, desktop->gc, 0, band_y, desktop->width, 16);
	}

	XSetForeground(desktop->display, desktop->gc, desktop->water_light);
	XFillArc(desktop->display, target, desktop->gc, 640, horizon + 64, 420, 86, 0, 360 * 64);
	XFillArc(desktop->display, target, desktop->gc, 220, horizon + 92, 240, 54, 0, 360 * 64);
	XFillArc(desktop->display, target, desktop->gc, 108, horizon + 130, 180, 40, 0, 360 * 64);

	XSetForeground(desktop->display, desktop->gc, desktop->water_haze);
	XDrawLine(desktop->display, target, desktop->gc, 0, horizon + 66, desktop->width, horizon + 66);
	XDrawLine(desktop->display, target, desktop->gc, 0, horizon + 98, desktop->width, horizon + 98);
	XDrawLine(desktop->display, target, desktop->gc, 0, horizon + 130, desktop->width, horizon + 130);
}

void horizon_wallpaper_draw(HorizonDesktop *desktop, Drawable target)
{
	const int horizon = desktop->height - desktop->taskbar_height - 242;

	horizon_fill_rect(desktop, target, 0, 0, desktop->width, desktop->height, desktop->background_top);
	horizon_fill_rect(desktop, target, 0, 0, desktop->width, horizon / 3, desktop->background_top);
	horizon_fill_rect(desktop, target, 0, horizon / 3, desktop->width, horizon / 4, desktop->background_high);
	horizon_fill_rect(desktop, target, 0, horizon / 3 + horizon / 4, desktop->width, horizon / 4, desktop->background_mid);
	horizon_fill_rect(desktop, target, 0, horizon - 36, desktop->width, 36, desktop->background_warm);
	horizon_fill_rect(desktop, target, 0, horizon, desktop->width, desktop->height - desktop->taskbar_height - horizon, desktop->background_horizon);

	draw_cloud(desktop, target, 54, 42, desktop->background_warm);
	draw_cloud(desktop, target, 298, 84, desktop->background_warm);
	draw_cloud(desktop, target, 686, 62, desktop->background_warm);
	draw_cloud(desktop, target, 998, 84, desktop->background_warm);
	draw_cloud(desktop, target, 1104, 124, desktop->background_warm);

	XSetForeground(desktop->display, desktop->gc, desktop->snow);
	XFillArc(desktop->display, target, desktop->gc, 332, horizon - 34, 28, 28, 0, 360 * 64);
	XSetForeground(desktop->display, desktop->gc, desktop->background_warm);
	XFillArc(desktop->display, target, desktop->gc, 316, horizon - 50, 60, 60, 0, 360 * 64);

	draw_mountain(desktop, target);
	draw_reflection(desktop, target);
	draw_tree(desktop, target);
	draw_petals(desktop, target);

	XSetForeground(desktop->display, desktop->gc, desktop->water_haze);
	XDrawLine(desktop->display, target, desktop->gc, 0, horizon + 32, desktop->width, horizon + 32);
	XDrawLine(desktop->display, target, desktop->gc, 0, horizon + 170, desktop->width, horizon + 170);
}
