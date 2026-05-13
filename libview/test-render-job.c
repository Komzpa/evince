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
#include "ev-job-scheduler.h"
#include "ev-jobs.h"

static gchar *
create_colored_pdf (void)
{
	cairo_surface_t *surface;
	cairo_t *cr;
	GError *error = NULL;
	gchar *dir;
	gchar *filename;

	dir = g_dir_make_tmp ("evince-render-job-XXXXXX", &error);
	g_assert_no_error (error);
	filename = g_build_filename (dir, "render-source.pdf", NULL);
	g_free (dir);

	surface = cairo_pdf_surface_create (filename, 320, 240);
	cr = cairo_create (surface);

	cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
	cairo_paint (cr);

	cairo_set_source_rgb (cr, 0.0, 0.2, 0.7);
	cairo_rectangle (cr, 0, 0, 320, 240);
	cairo_fill (cr);

	cairo_set_source_rgb (cr, 1.0, 0.0, 0.0);
	cairo_rectangle (cr, 210, 40, 70, 150);
	cairo_fill (cr);

	cairo_show_page (cr);
	cairo_destroy (cr);
	cairo_surface_destroy (surface);

	return filename;
}

static guint
count_nonwhite_texture_pixels (GdkTexture *texture)
{
	gint width = gdk_texture_get_width (texture);
	gint height = gdk_texture_get_height (texture);
	gsize stride = width * 4;
	g_autofree guchar *data = g_malloc0 (stride * height);
	guint count = 0;

	gdk_texture_download (texture, data, stride);

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
render_job_finished_cb (EvJob   *job,
			gboolean *finished)
{
	*finished = TRUE;
}

static void
test_render_texture_job_produces_page_pixels (void)
{
	EvDocument *document;
	EvJob *job;
	EvJobRenderTexture *render_job;
	GError *error = NULL;
	g_autofree gchar *filename = NULL;
	g_autofree gchar *dirname = NULL;
	g_autofree gchar *uri = NULL;
	gboolean finished_signal = FALSE;
	gint64 deadline;

	g_assert_nonnull (g_getenv ("EV_BACKENDS_DIR"));

	filename = create_colored_pdf ();
	uri = g_filename_to_uri (filename, NULL, &error);
	g_assert_no_error (error);

	document = ev_document_factory_get_document (uri, &error);
	g_assert_no_error (error);
	g_assert_nonnull (document);

	job = ev_job_render_texture_new (document, 0, 0, 1.0, 160, 120);
	render_job = EV_JOB_RENDER_TEXTURE (job);
	g_signal_connect (job, "finished",
			  G_CALLBACK (render_job_finished_cb),
			  &finished_signal);

	ev_job_scheduler_push_job (job, EV_JOB_PRIORITY_URGENT);

	deadline = g_get_monotonic_time () + 5 * G_USEC_PER_SEC;
	while (!finished_signal &&
	       g_get_monotonic_time () < deadline)
		g_main_context_iteration (NULL, TRUE);

	g_assert_true (finished_signal);
	g_assert_false (ev_job_is_failed (job));
	g_assert_true (render_job->page_ready);
	g_assert_nonnull (render_job->texture);
	g_assert_cmpuint (count_nonwhite_texture_pixels (render_job->texture), >, 160 * 120 / 2);

	g_object_unref (job);
	g_object_unref (document);
	dirname = g_path_get_dirname (filename);
	g_unlink (filename);
	g_rmdir (dirname);
}

int
main (int argc, char **argv)
{
	int retval;

	g_test_init (&argc, &argv, NULL);

	if (!ev_init ())
		return 77;

	g_test_add_func ("/render-job/render-texture-job-produces-page-pixels",
			 test_render_texture_job_produces_page_pixels);

	retval = g_test_run ();
	ev_shutdown ();

	return retval;
}
