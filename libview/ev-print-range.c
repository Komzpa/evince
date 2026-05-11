#include <config.h>

#include "ev-print-range-private.h"

gboolean
ev_print_range_clamp (GtkPageRange *ranges,
                      gint         *n_ranges,
                      gint          n_pages,
                      GtkPageSet    page_set,
                      gint         *n_pages_to_print)
{
	gint num_of_correct_ranges = 0;
	gint num_of_printable_pages = 0;

	for (gint i = 0; i < *n_ranges; i++) {
		GtkPageRange range = ranges[i];
		gint range_pages;

		if (range.start < 0)
			range.start = 0;
		if (range.end >= n_pages)
			range.end = n_pages - 1;

		if (range.start >= n_pages ||
		    range.end < 0 ||
		    range.start > range.end)
			continue;

		ranges[num_of_correct_ranges] = range;
		num_of_correct_ranges++;

		range_pages = range.end - range.start + 1;
		if (page_set == GTK_PAGE_SET_ALL) {
			num_of_printable_pages += range_pages;
		} else if (range_pages % 2 == 0) {
			num_of_printable_pages += range_pages / 2;
		} else if (page_set == GTK_PAGE_SET_EVEN) {
			num_of_printable_pages += range.start % 2 == 0 ?
				range_pages / 2 : (range_pages / 2) + 1;
		} else if (page_set == GTK_PAGE_SET_ODD) {
			num_of_printable_pages += range.start % 2 == 0 ?
				(range_pages / 2) + 1 : range_pages / 2;
		}
	}

	*n_ranges = num_of_correct_ranges;
	*n_pages_to_print = num_of_printable_pages;

	return num_of_printable_pages > 0;
}
