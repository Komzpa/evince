/*
 * Copyright (C) 2026 Darafei Praliaskouski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "config.h"

#include "ev-window-metadata-private.h"

static void
test_document_mode_uses_fullscreen_metadata (void)
{
	g_assert_cmpint (ev_window_metadata_document_mode (TRUE, TRUE, FALSE, FALSE),
			 ==,
			 EV_WINDOW_METADATA_DOCUMENT_MODE_FULLSCREEN);
}

static void
test_document_mode_ignores_false_fullscreen_metadata (void)
{
	g_assert_cmpint (ev_window_metadata_document_mode (TRUE, FALSE, FALSE, FALSE),
			 ==,
			 EV_WINDOW_METADATA_DOCUMENT_MODE_NONE);
}

static void
test_document_mode_prefers_presentation_metadata (void)
{
	g_assert_cmpint (ev_window_metadata_document_mode (TRUE, TRUE, TRUE, TRUE),
			 ==,
			 EV_WINDOW_METADATA_DOCUMENT_MODE_PRESENTATION);
}

static void
test_document_mode_ignores_missing_presentation_metadata (void)
{
	g_assert_cmpint (ev_window_metadata_document_mode (TRUE, TRUE, FALSE, FALSE),
			 ==,
			 EV_WINDOW_METADATA_DOCUMENT_MODE_FULLSCREEN);
}

int
main (int argc, char **argv)
{
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/ev-window/metadata/document-mode/fullscreen",
			 test_document_mode_uses_fullscreen_metadata);
	g_test_add_func ("/ev-window/metadata/document-mode/false-fullscreen",
			 test_document_mode_ignores_false_fullscreen_metadata);
	g_test_add_func ("/ev-window/metadata/document-mode/presentation-preferred",
			 test_document_mode_prefers_presentation_metadata);
	g_test_add_func ("/ev-window/metadata/document-mode/missing-presentation",
			 test_document_mode_ignores_missing_presentation_metadata);

	return g_test_run ();
}
