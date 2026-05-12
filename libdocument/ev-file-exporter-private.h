/* this file is part of evince, a gnome document viewer
 *
 *  Copyright (C) 2026 The Evince authors
 *
 * Evince is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#pragma once

#if !defined (EVINCE_COMPILATION)
#error "This is a private header."
#endif

#include "ev-file-exporter.h"

G_BEGIN_DECLS

EV_PRIVATE
void     ev_file_exporter_set_print_settings     (EvFileExporter *exporter,
						  gboolean        scale_to_paper,
						  gboolean        autorotate,
						  gdouble         manual_scale);
EV_PRIVATE
void     ev_file_exporter_clear_print_settings   (EvFileExporter *exporter);
EV_PRIVATE
void     ev_file_exporter_get_print_settings     (EvFileExporter *exporter,
						  gboolean       *scale_to_paper,
						  gboolean       *autorotate,
						  gdouble        *manual_scale);

G_END_DECLS
