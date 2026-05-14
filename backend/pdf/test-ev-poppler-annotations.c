/*
 * Copyright (C) 2026 Darafei Praliaskouski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "config.h"

#include "ev-poppler-private.h"

static void
test_text_icon_flags_keep_existing_flags (void)
{
	PopplerAnnotFlag flags;

	flags = ev_poppler_annot_text_icon_display_flags (POPPLER_ANNOT_FLAG_PRINT |
							  POPPLER_ANNOT_FLAG_READ_ONLY);

	g_assert_true (flags & POPPLER_ANNOT_FLAG_NO_ZOOM);
	g_assert_true (flags & POPPLER_ANNOT_FLAG_PRINT);
	g_assert_true (flags & POPPLER_ANNOT_FLAG_READ_ONLY);
}

static void
test_text_icon_flags_are_idempotent (void)
{
	PopplerAnnotFlag flags;

	flags = ev_poppler_annot_text_icon_display_flags (POPPLER_ANNOT_FLAG_NO_ZOOM |
							  POPPLER_ANNOT_FLAG_LOCKED);

	g_assert_cmpuint (flags, ==, POPPLER_ANNOT_FLAG_NO_ZOOM | POPPLER_ANNOT_FLAG_LOCKED);
}

int
main (int argc, char **argv)
{
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/pdf/annotations/text-icon-flags/keep-existing-flags",
			 test_text_icon_flags_keep_existing_flags);
	g_test_add_func ("/pdf/annotations/text-icon-flags/idempotent",
			 test_text_icon_flags_are_idempotent);

	return g_test_run ();
}
