/*
 * Copyright (C) 2026 Evince contributors
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 */

#include "config.h"

#include "ev-previewer-window-private.h"

#include <glib.h>

static void
test_print_action_enabled_without_settings_file (void)
{
	g_assert_true (ev_previewer_print_action_enabled_for_settings (FALSE, FALSE));
}

static void
test_print_action_disabled_for_failed_settings_file (void)
{
	g_assert_false (ev_previewer_print_action_enabled_for_settings (TRUE, FALSE));
}

static void
test_print_action_enabled_after_settings_load (void)
{
	g_assert_true (ev_previewer_print_action_enabled_for_settings (TRUE, TRUE));
}

int
main (int argc, char **argv)
{
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/previewer/print-settings/default-path-enabled",
			 test_print_action_enabled_without_settings_file);
	g_test_add_func ("/previewer/print-settings/failed-settings-disabled",
			 test_print_action_disabled_for_failed_settings_file);
	g_test_add_func ("/previewer/print-settings/enabled-after-load",
			 test_print_action_enabled_after_settings_load);

	return g_test_run ();
}
