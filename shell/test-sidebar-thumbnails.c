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

static guint
count_nonwhite_texture_pixels (GdkTexture *texture)
{
	gsize stride;
	guchar *data;
	guint count = 0;

	stride = gdk_texture_get_width (texture) * 4;
	data = g_malloc0 (stride * gdk_texture_get_height (texture));
	gdk_texture_download (texture, data, stride);

	for (gint y = 0; y < gdk_texture_get_height (texture); y++) {
		guchar *row = data + y * stride;

		for (gint x = 0; x < gdk_texture_get_width (texture); x++) {
			guchar *pixel = row + x * 4;

			if (pixel[0] < 245 || pixel[1] < 245 || pixel[2] < 245) {
				count++;
			}
		}
	}

	g_free (data);

	return count;
}

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

static void
test_thumbnail_texture_keeps_page_pixels_after_surface_destroy (void)
{
	cairo_surface_t *surface;
	cairo_t *cr;
	GdkTexture *texture;
	guint nonwhite_pixels;

	surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 160, 120);
	cr = cairo_create (surface);

	cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
	cairo_paint (cr);
	cairo_set_source_rgb (cr, 0.0, 0.2, 0.7);
	cairo_rectangle (cr, 0, 0, 160, 120);
	cairo_fill (cr);
	cairo_destroy (cr);

	texture = ev_sidebar_thumbnails_texture_new_for_surface (surface);
	cairo_surface_destroy (surface);

	g_assert_cmpint (gdk_texture_get_width (texture), ==, 160);
	g_assert_cmpint (gdk_texture_get_height (texture), ==, 120);

	nonwhite_pixels = count_nonwhite_texture_pixels (texture);
	g_assert_cmpuint (nonwhite_pixels, >, 160 * 120 / 2);

	g_object_unref (texture);
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
	g_test_add_func ("/sidebar-thumbnails/texture/keeps-page-pixels-after-surface-destroy",
			 test_thumbnail_texture_keeps_page_pixels_after_surface_destroy);

	return g_test_run ();
}
