/*
 * Copyright (C) 2026 Darafei Praliaskouski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "config.h"

#include <cairo-pdf.h>
#include <glib/gstdio.h>

#include "ev-document-factory.h"
#include "ev-init.h"
#include "ev-jobs.h"

static gchar *
create_colored_pdf (void)
{
	cairo_surface_t *surface;
	cairo_t *cr;
	GError *error = NULL;
	gchar *dir;
	gchar *filename;

	dir = g_dir_make_tmp ("evince-thumbnail-render-XXXXXX", &error);
	g_assert_no_error (error);
	filename = g_build_filename (dir, "thumbnail-source.pdf", NULL);
	g_free (dir);

	surface = cairo_pdf_surface_create (filename, 800, 600);
	cr = cairo_create (surface);

	cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
	cairo_paint (cr);

	cairo_set_source_rgb (cr, 0.0, 0.2, 0.7);
	cairo_rectangle (cr, 0, 0, 800, 600);
	cairo_fill (cr);

	cairo_set_source_rgb (cr, 1.0, 0.0, 0.0);
	cairo_rectangle (cr, 520, 100, 180, 360);
	cairo_fill (cr);

	cairo_show_page (cr);
	cairo_destroy (cr);
	cairo_surface_destroy (surface);

	return filename;
}

static guint
count_nonwhite_pixels (cairo_surface_t *surface)
{
	guchar *data;
	gint stride;
	gint width;
	gint height;
	guint count = 0;

	g_assert_cmpint (cairo_image_surface_get_format (surface), ==, CAIRO_FORMAT_ARGB32);

	cairo_surface_flush (surface);
	data = cairo_image_surface_get_data (surface);
	stride = cairo_image_surface_get_stride (surface);
	width = cairo_image_surface_get_width (surface);
	height = cairo_image_surface_get_height (surface);

	for (gint y = 0; y < height; y++) {
		guchar *row = data + y * stride;

		for (gint x = 0; x < width; x++) {
			guchar *pixel = row + x * 4;

			if (pixel[0] < 245 || pixel[1] < 245 || pixel[2] < 245)
				count++;
		}
	}

	return count;
}

static void
test_target_size_thumbnail_renders_page_pixels (void)
{
	EvDocument *document;
	EvJob *job;
	EvJobThumbnailCairo *thumbnail_job;
	GError *error = NULL;
	gchar *filename;
	gchar *dirname;
	gchar *uri;
	guint nonwhite_pixels;

	g_assert_nonnull (g_getenv ("EV_BACKENDS_DIR"));

	filename = create_colored_pdf ();
	uri = g_filename_to_uri (filename, NULL, &error);
	g_assert_no_error (error);

	document = ev_document_factory_get_document (uri, &error);
	g_assert_no_error (error);
	g_assert_nonnull (document);

	G_GNUC_BEGIN_IGNORE_DEPRECATIONS
	job = ev_job_thumbnail_cairo_new_with_target_size (document, 0, 0, 160, 120);
	ev_job_run (job);
	thumbnail_job = EV_JOB_THUMBNAIL_CAIRO (job);
	G_GNUC_END_IGNORE_DEPRECATIONS

	g_assert_false (ev_job_is_failed (job));
	g_assert_nonnull (thumbnail_job->thumbnail_surface);
	g_assert_cmpint (cairo_image_surface_get_width (thumbnail_job->thumbnail_surface), ==, 160);
	g_assert_cmpint (cairo_image_surface_get_height (thumbnail_job->thumbnail_surface), ==, 120);

	nonwhite_pixels = count_nonwhite_pixels (thumbnail_job->thumbnail_surface);
	g_assert_cmpuint (nonwhite_pixels, >, 160 * 120 / 2);

	g_object_unref (job);
	g_object_unref (document);
	dirname = g_path_get_dirname (filename);
	g_unlink (filename);
	g_rmdir (dirname);
	g_free (dirname);
	g_free (uri);
	g_free (filename);
}

int
main (int argc, char **argv)
{
	int retval;

	g_test_init (&argc, &argv, NULL);

	if (!ev_init ())
		return 77;

	g_test_add_func ("/thumbnail-render/target-size/page-pixels",
			 test_target_size_thumbnail_renders_page_pixels);

	retval = g_test_run ();
	ev_shutdown ();

	return retval;
}
