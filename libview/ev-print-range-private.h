#pragma once

#include <gtk/gtk.h>

gboolean ev_print_range_clamp (GtkPageRange *ranges,
                               gint         *n_ranges,
                               gint          n_pages,
                               GtkPageSet    page_set,
                               gint         *n_pages_to_print);
