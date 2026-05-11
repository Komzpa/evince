#include <config.h>

#include "ev-print-range-private.h"

static void
test_print_range_empty_page_set (void)
{
	GtkPageRange ranges[] = {
		{ 0, 0 },
		{ 2, 2 },
		{ 4, 4 },
	};
	gint n_ranges = G_N_ELEMENTS (ranges);
	gint n_pages_to_print = -1;

	g_assert_false (ev_print_range_clamp (ranges,
	                                      &n_ranges,
	                                      5,
	                                      GTK_PAGE_SET_EVEN,
	                                      &n_pages_to_print));
	g_assert_cmpint (n_ranges, ==, 3);
	g_assert_cmpint (n_pages_to_print, ==, 0);
}

static void
test_print_range_drops_out_of_document_ranges (void)
{
	GtkPageRange ranges[] = {
		{ 10, 10 },
	};
	gint n_ranges = G_N_ELEMENTS (ranges);
	gint n_pages_to_print = -1;

	g_assert_false (ev_print_range_clamp (ranges,
	                                      &n_ranges,
	                                      5,
	                                      GTK_PAGE_SET_ALL,
	                                      &n_pages_to_print));
	g_assert_cmpint (n_ranges, ==, 0);
	g_assert_cmpint (n_pages_to_print, ==, 0);
}

static void
test_print_range_clamps_partial_ranges (void)
{
	GtkPageRange ranges[] = {
		{ -2, 1 },
		{ 3, 8 },
	};
	gint n_ranges = G_N_ELEMENTS (ranges);
	gint n_pages_to_print = -1;

	g_assert_true (ev_print_range_clamp (ranges,
	                                     &n_ranges,
	                                     5,
	                                     GTK_PAGE_SET_ALL,
	                                     &n_pages_to_print));
	g_assert_cmpint (n_ranges, ==, 2);
	g_assert_cmpint (n_pages_to_print, ==, 4);
	g_assert_cmpint (ranges[0].start, ==, 0);
	g_assert_cmpint (ranges[0].end, ==, 1);
	g_assert_cmpint (ranges[1].start, ==, 3);
	g_assert_cmpint (ranges[1].end, ==, 4);
}

int
main (int   argc,
      char *argv[])
{
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/print/range/empty-page-set", test_print_range_empty_page_set);
	g_test_add_func ("/print/range/drop-out-of-document-ranges", test_print_range_drops_out_of_document_ranges);
	g_test_add_func ("/print/range/clamp-partial-ranges", test_print_range_clamps_partial_ranges);

	return g_test_run ();
}
