/*
 * Copyright (C) 2026 Darafei Praliaskouski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "config.h"

#include "ev-window-private.h"

static void
test_pointer_reveals_toolbar_at_top_edge (void)
{
	g_assert_true (ev_window_fullscreen_pointer_reveals_toolbar (0.0));
	g_assert_true (ev_window_fullscreen_pointer_reveals_toolbar (EV_WINDOW_FULLSCREEN_TOOLBAR_REVEAL_EDGE));
	g_assert_false (ev_window_fullscreen_pointer_reveals_toolbar (EV_WINDOW_FULLSCREEN_TOOLBAR_REVEAL_EDGE + 0.5));
}

static void
test_pointer_keeps_toolbar_inside_toolbar_height (void)
{
	g_assert_true (ev_window_fullscreen_pointer_keeps_toolbar (0.0, 48));
	g_assert_true (ev_window_fullscreen_pointer_keeps_toolbar (48.0, 48));
	g_assert_false (ev_window_fullscreen_pointer_keeps_toolbar (48.5, 48));
}

static void
test_pointer_keeps_toolbar_handles_unallocated_height (void)
{
	g_assert_true (ev_window_fullscreen_pointer_keeps_toolbar (0.0, -1));
	g_assert_false (ev_window_fullscreen_pointer_keeps_toolbar (0.5, -1));
}

static void
test_pointer_hides_toolbar_outside_revealed_toolbar (void)
{
	g_assert_false (ev_window_fullscreen_pointer_hides_toolbar (0.0, 48));
	g_assert_false (ev_window_fullscreen_pointer_hides_toolbar (48.0, 48));
	g_assert_true (ev_window_fullscreen_pointer_hides_toolbar (48.5, 48));
}

static void
test_timeout_only_hides_while_fullscreen (void)
{
	g_assert_true (ev_window_fullscreen_toolbar_timeout_hides (TRUE));
	g_assert_false (ev_window_fullscreen_toolbar_timeout_hides (FALSE));
}

static void
test_pointer_tracking_ignores_presentation (void)
{
	g_assert_true (ev_window_fullscreen_toolbar_tracks_pointer (TRUE, FALSE));
	g_assert_false (ev_window_fullscreen_toolbar_tracks_pointer (TRUE, TRUE));
	g_assert_false (ev_window_fullscreen_toolbar_tracks_pointer (FALSE, FALSE));
}

int
main (int argc, char **argv)
{
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/ev-window/fullscreen-toolbar/reveal-top-edge",
			 test_pointer_reveals_toolbar_at_top_edge);
	g_test_add_func ("/ev-window/fullscreen-toolbar/keep-inside-height",
			 test_pointer_keeps_toolbar_inside_toolbar_height);
	g_test_add_func ("/ev-window/fullscreen-toolbar/unallocated-height",
			 test_pointer_keeps_toolbar_handles_unallocated_height);
	g_test_add_func ("/ev-window/fullscreen-toolbar/hide-outside-toolbar",
			 test_pointer_hides_toolbar_outside_revealed_toolbar);
	g_test_add_func ("/ev-window/fullscreen-toolbar/timeout-state",
			 test_timeout_only_hides_while_fullscreen);
	g_test_add_func ("/ev-window/fullscreen-toolbar/ignore-presentation",
			 test_pointer_tracking_ignores_presentation);

	return g_test_run ();
}
