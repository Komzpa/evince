/*
 * Copyright (C) 2026 Darafei Praliaskouski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "config.h"

#include "ev-sidebar-thumbnails-private.h"

static void
test_thumbnail_target_size_uses_logical_pixels (void)
{
	gint width = 0;
	gint height = 0;

	ev_sidebar_thumbnails_get_target_size (800, 600, 0, &width, &height);
	g_assert_cmpint (width, ==, 160);
	g_assert_cmpint (height, ==, 120);
}

static void
test_thumbnail_target_size_rotates_logical_pixels (void)
{
	gint width = 0;
	gint height = 0;

	ev_sidebar_thumbnails_get_target_size (800, 600, 90, &width, &height);
	g_assert_cmpint (width, ==, 120);
	g_assert_cmpint (height, ==, 160);
}

static void
test_thumbnail_target_size_clamps_extreme_aspect_ratio (void)
{
	gint width = 0;
	gint height = 0;

	ev_sidebar_thumbnails_get_target_size (10000, 1, 0, &width, &height);
	g_assert_cmpint (width, ==, 160);
	g_assert_cmpint (height, ==, 1);
}

int
main (int argc, char **argv)
{
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/sidebar-thumbnails/target-size/logical-pixels",
			 test_thumbnail_target_size_uses_logical_pixels);
	g_test_add_func ("/sidebar-thumbnails/target-size/rotated-logical-pixels",
			 test_thumbnail_target_size_rotates_logical_pixels);
	g_test_add_func ("/sidebar-thumbnails/target-size/clamps-extreme-aspect-ratio",
			 test_thumbnail_target_size_clamps_extreme_aspect_ratio);

	return g_test_run ();
}
