/*
 * Copyright (C) 2026 Evince contributors
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 */

#include "config.h"

#include "ev-poppler-links-private.h"

#include <glib.h>

static void
test_outline_title_make_single_line (void)
{
	char title[] = "Chapter\nOne\rTwo";

	ev_poppler_outline_title_make_single_line (title);

	g_assert_cmpstr (title, ==, "Chapter One Two");
}

int
main (int argc, char *argv[])
{
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/pdf/links/outline-title-single-line",
			 test_outline_title_make_single_line);

	return g_test_run ();
}
