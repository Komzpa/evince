/*
 * Copyright (C) 2026 Darafei Praliaskouski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "config.h"

#include <glib.h>
#include <stdlib.h>

int evince_thumbnailer_program_main (int argc, char *argv[]);

#define main evince_thumbnailer_program_main
#include "evince-thumbnailer.c"
#undef main

static void
test_watchdog_timeout_reports_failure (void)
{
	g_assert_cmpint (thumbnailer_timeout_exit_status (), ==, EXIT_FAILURE);
}

int
main (int argc, char **argv)
{
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/thumbnailer/watchdog-timeout-reports-failure",
			 test_watchdog_timeout_reports_failure);

	return g_test_run ();
}
