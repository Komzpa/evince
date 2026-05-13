/*
 * Copyright (C) 2026 Darafei Praliaskouski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "config.h"

#include "ev-view-private.h"

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

static void
test_zoom_anchor_scroll_value (void)
{
	g_assert_cmpint (ev_view_zoom_anchor_scroll_value (520, 0, 1000, 300), ==, 520);
	g_assert_cmpint (ev_view_zoom_anchor_scroll_value (-70, -70, 1000, 300), ==, -70);
	g_assert_cmpint (ev_view_zoom_anchor_scroll_value (860, 0, 1160, 300), ==, 860);
}

static void
test_zoom_anchor_scroll_bounds_allow_edge_overscroll (void)
{
	gdouble lower = 0.0;
	gdouble upper = 1000.0;

	ev_view_zoom_anchor_scroll_bounds (-70, lower, upper, 300, &lower, &upper);

	g_assert_cmpfloat (lower, ==, -70.0);
	g_assert_cmpfloat (upper, ==, 1000.0);

	lower = 0.0;
	upper = 1000.0;
	ev_view_zoom_anchor_scroll_bounds (860, lower, upper, 300, &lower, &upper);

	g_assert_cmpfloat (lower, ==, 0.0);
	g_assert_cmpfloat (upper, ==, 1160.0);
}

static void
test_zoom_anchor_matches_same_widget_position (void)
{
	g_assert_true (ev_view_zoom_anchor_matches_widget_position (TRUE, 120.0, 80.0, 120.0, 80.0));
	g_assert_true (ev_view_zoom_anchor_matches_widget_position (TRUE, 120.0, 80.0, 120.4, 79.6));
	g_assert_false (ev_view_zoom_anchor_matches_widget_position (FALSE, 120.0, 80.0, 120.0, 80.0));
	g_assert_false (ev_view_zoom_anchor_matches_widget_position (TRUE, 120.0, 80.0, 121.0, 80.0));
	g_assert_false (ev_view_zoom_anchor_matches_widget_position (TRUE, 120.0, 80.0, 120.0, 81.0));
}

static void
test_zoom_scale_limit_allows_400_percent (void)
{
	gdouble dpi = 96.0 / 72.0;
	gdouble scale;

	scale = ev_view_max_scale_for_page (52428800, 1024, 768, dpi);

	g_assert_cmpfloat_with_epsilon (scale / dpi, 4.0, 0.000001);
}

static void
test_zoom_scale_limit_keeps_larger_cache_limit (void)
{
	gdouble dpi = 96.0 / 72.0;
	gdouble scale;

	scale = ev_view_max_scale_for_page (1024 * 1024 * 1024, 512, 512, dpi);

	g_assert_cmpfloat (scale / dpi, >, 4.0);
}

static void
test_zoom_center_for_scroll_uses_event_position (void)
{
	gdouble x = 0.0;
	gdouble y = 0.0;

	ev_view_zoom_center_for_scroll (TRUE, 42.0, 84.0,
					TRUE, 10, 20,
					800, 600,
					&x, &y);

	g_assert_cmpfloat (x, ==, 42.0);
	g_assert_cmpfloat (y, ==, 84.0);
}

static void
test_zoom_center_for_scroll_uses_pointer_fallback (void)
{
	gdouble x = 0.0;
	gdouble y = 0.0;

	ev_view_zoom_center_for_scroll (FALSE, 0.0, 0.0,
					TRUE, 123, 234,
					800, 600,
					&x, &y);

	g_assert_cmpfloat (x, ==, 123.0);
	g_assert_cmpfloat (y, ==, 234.0);
}

static void
test_zoom_center_for_scroll_uses_widget_center_last (void)
{
	gdouble x = 0.0;
	gdouble y = 0.0;

	ev_view_zoom_center_for_scroll (FALSE, 0.0, 0.0,
					FALSE, 0, 0,
					800, 600,
					&x, &y);

	g_assert_cmpfloat (x, ==, 400.0);
	g_assert_cmpfloat (y, ==, 300.0);
}

static void
test_range_update_queues_draw_for_pending_resize (void)
{
	g_assert_true (ev_view_should_queue_draw_after_range_update (TRUE, FALSE));
	g_assert_true (ev_view_should_queue_draw_after_range_update (FALSE, TRUE));
	g_assert_false (ev_view_should_queue_draw_after_range_update (FALSE, FALSE));
}

static void
test_scale_change_queues_draw (void)
{
	g_assert_true (ev_view_should_queue_draw_after_scale_change (TRUE));
	g_assert_false (ev_view_should_queue_draw_after_scale_change (FALSE));
}

static void
test_size_allocate_runs_for_zoom_resize_while_loading (void)
{
	g_assert_true (ev_view_should_handle_size_allocate (TRUE, FALSE, FALSE));
	g_assert_true (ev_view_should_handle_size_allocate (TRUE, TRUE, TRUE));
	g_assert_false (ev_view_should_handle_size_allocate (TRUE, TRUE, FALSE));
	g_assert_false (ev_view_should_handle_size_allocate (FALSE, FALSE, TRUE));
}

int
main (int argc, char **argv)
{
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/ev-view/zoom-anchor-scroll-value",
			 test_zoom_anchor_scroll_value);
	g_test_add_func ("/ev-view/zoom-anchor-scroll-bounds-allow-edge-overscroll",
			 test_zoom_anchor_scroll_bounds_allow_edge_overscroll);
	g_test_add_func ("/ev-view/zoom-anchor-matches-same-widget-position",
			 test_zoom_anchor_matches_same_widget_position);
	g_test_add_func ("/ev-view/zoom-scale-limit/allows-400-percent",
			 test_zoom_scale_limit_allows_400_percent);
	g_test_add_func ("/ev-view/zoom-scale-limit/keeps-larger-cache-limit",
			 test_zoom_scale_limit_keeps_larger_cache_limit);
	g_test_add_func ("/ev-view/zoom-center-for-scroll/event-position",
			 test_zoom_center_for_scroll_uses_event_position);
	g_test_add_func ("/ev-view/zoom-center-for-scroll/pointer-fallback",
			 test_zoom_center_for_scroll_uses_pointer_fallback);
	g_test_add_func ("/ev-view/zoom-center-for-scroll/widget-center-last",
			 test_zoom_center_for_scroll_uses_widget_center_last);
	g_test_add_func ("/ev-view/range-update/queues-draw-for-pending-resize",
			 test_range_update_queues_draw_for_pending_resize);
	g_test_add_func ("/ev-view/scale-change/queues-draw",
			 test_scale_change_queues_draw);
	g_test_add_func ("/ev-view/size-allocate/runs-for-zoom-resize-while-loading",
			 test_size_allocate_runs_for_zoom_resize_while_loading);

	if (!gtk_init_check ()) {
		g_test_add_func ("/ev-view/scroll-controller-installed",
				 test_scroll_controller_no_display);
		return g_test_run ();
	}

	g_test_add_func ("/ev-view/scroll-controller-installed",
			 test_scroll_controller_is_installed);

	return g_test_run ();
}
