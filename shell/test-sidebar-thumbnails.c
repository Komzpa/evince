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
	g_assert_cmpint (width, ==, 100);
	g_assert_cmpint (height, ==, 75);
}

static void
test_thumbnail_target_size_rotates_logical_pixels (void)
{
	gint width = 0;
	gint height = 0;

	ev_sidebar_thumbnails_get_target_size (800, 600, 90, &width, &height);
	g_assert_cmpint (width, ==, 75);
	g_assert_cmpint (height, ==, 100);
}

int
main (int argc, char **argv)
{
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/sidebar-thumbnails/target-size/logical-pixels",
			 test_thumbnail_target_size_uses_logical_pixels);
	g_test_add_func ("/sidebar-thumbnails/target-size/rotated-logical-pixels",
			 test_thumbnail_target_size_rotates_logical_pixels);

	return g_test_run ();
}
