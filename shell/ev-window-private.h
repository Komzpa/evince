/* this file is part of evince, a gnome document viewer
 *
 * Copyright (C) 2026 Darafei Praliaskouski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 */

#pragma once

#include <glib.h>

#define EV_WINDOW_FULLSCREEN_TOOLBAR_REVEAL_EDGE 6.0
#define EV_WINDOW_FULLSCREEN_TOOLBAR_HIDE_DELAY_MS 1200

static inline gboolean
ev_window_fullscreen_pointer_reveals_toolbar (gdouble y)
{
	return y <= EV_WINDOW_FULLSCREEN_TOOLBAR_REVEAL_EDGE;
}

static inline gboolean
ev_window_fullscreen_pointer_keeps_toolbar (gdouble y,
					    gint    toolbar_height)
{
	return y <= MAX (toolbar_height, 0);
}

static inline gboolean
ev_window_fullscreen_pointer_hides_toolbar (gdouble y,
					    gint    toolbar_height)
{
	return !ev_window_fullscreen_pointer_reveals_toolbar (y) &&
	       !ev_window_fullscreen_pointer_keeps_toolbar (y, toolbar_height);
}

static inline gboolean
ev_window_fullscreen_toolbar_timeout_hides (gboolean fullscreen)
{
	return fullscreen;
}

static inline gboolean
ev_window_fullscreen_toolbar_tracks_pointer (gboolean fullscreen,
					     gboolean presentation)
{
	return fullscreen && !presentation;
}
