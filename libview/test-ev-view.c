/*
 * Copyright (C) 2026 Darafei Praliaskouski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "config.h"

#include <evince-view.h>

static void
test_scroll_controller_no_display (void)
{
	g_test_skip ("GTK display is not available");
}

static void
test_scroll_controller_is_installed (void)
{
	GtkWidget *view = GTK_WIDGET (g_object_ref_sink (ev_view_new ()));
	GListModel *controllers = gtk_widget_observe_controllers (view);
	guint n_controllers = g_list_model_get_n_items (controllers);
	gboolean found_scroll_controller = FALSE;

	for (guint i = 0; i < n_controllers; i++) {
		g_autoptr (GtkEventController) controller = g_list_model_get_item (controllers, i);

		if (GTK_IS_EVENT_CONTROLLER_SCROLL (controller)) {
			found_scroll_controller = TRUE;
			break;
		}
	}

	g_assert_true (found_scroll_controller);

	g_object_unref (controllers);
	g_object_unref (view);
}

int
main (int argc, char **argv)
{
	g_test_init (&argc, &argv, NULL);

	if (!gtk_init_check ()) {
		g_test_add_func ("/ev-view/scroll-controller-installed",
				 test_scroll_controller_no_display);
		return g_test_run ();
	}

	g_test_add_func ("/ev-view/scroll-controller-installed",
			 test_scroll_controller_is_installed);

	return g_test_run ();
}
