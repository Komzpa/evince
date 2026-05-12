/* this file is part of evince, a gnome document viewer
 *
 *  Copyright (C) 2004 Martin Kretzschmar
 *
 *  Author:
 *    Martin Kretzschmar <martink@gnome.org>
 *
 * Evince is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Evince is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <config.h>
#include "ev-file-exporter.h"
#include "ev-file-exporter-private.h"
#include "ev-document.h"

G_DEFINE_INTERFACE (EvFileExporter, ev_file_exporter, 0)

typedef struct {
	gboolean scale_to_paper;
	gboolean autorotate;
	gdouble manual_scale;
} EvFileExporterPrintSettings;

static GQuark
ev_file_exporter_print_settings_quark (void)
{
	static GQuark quark = 0;

	if (quark == 0)
		quark = g_quark_from_static_string ("ev-file-exporter-print-settings");

	return quark;
}

static void
ev_file_exporter_default_init (EvFileExporterInterface *klass)
{
}

void
ev_file_exporter_set_print_settings (EvFileExporter *exporter,
				     gboolean        scale_to_paper,
				     gboolean        autorotate,
				     gdouble         manual_scale)
{
	EvFileExporterPrintSettings *settings;

	g_return_if_fail (EV_IS_FILE_EXPORTER (exporter));

	settings = g_new0 (EvFileExporterPrintSettings, 1);
	settings->scale_to_paper = scale_to_paper;
	settings->autorotate = autorotate;
	settings->manual_scale = manual_scale;

	g_object_set_qdata_full (G_OBJECT (exporter),
				 ev_file_exporter_print_settings_quark (),
				 settings,
				 g_free);
}

void
ev_file_exporter_clear_print_settings (EvFileExporter *exporter)
{
	g_return_if_fail (EV_IS_FILE_EXPORTER (exporter));

	g_object_set_qdata (G_OBJECT (exporter),
			    ev_file_exporter_print_settings_quark (),
			    NULL);
}

void
ev_file_exporter_get_print_settings (EvFileExporter *exporter,
				     gboolean       *scale_to_paper,
				     gboolean       *autorotate,
				     gdouble        *manual_scale)
{
	EvFileExporterPrintSettings *settings;

	g_return_if_fail (EV_IS_FILE_EXPORTER (exporter));

	settings = g_object_get_qdata (G_OBJECT (exporter),
				      ev_file_exporter_print_settings_quark ());

	if (scale_to_paper)
		*scale_to_paper = settings ? settings->scale_to_paper : TRUE;
	if (autorotate)
		*autorotate = settings ? settings->autorotate : TRUE;
	if (manual_scale)
		*manual_scale = settings ? settings->manual_scale : 1.0;
}

void
ev_file_exporter_begin (EvFileExporter        *exporter,
                        EvFileExporterContext *fc)
{
        EvFileExporterInterface *iface = EV_FILE_EXPORTER_GET_IFACE (exporter);

        iface->begin (exporter, fc);
}

void
ev_file_exporter_begin_page (EvFileExporter *exporter)
{
	EvFileExporterInterface *iface = EV_FILE_EXPORTER_GET_IFACE (exporter);

	if (iface->begin_page)
		iface->begin_page (exporter);
}

void
ev_file_exporter_do_page (EvFileExporter  *exporter,
			  EvRenderContext *rc)
{
        EvFileExporterInterface *iface = EV_FILE_EXPORTER_GET_IFACE (exporter);

        iface->do_page (exporter, rc);
}

void
ev_file_exporter_end_page (EvFileExporter *exporter)
{
	EvFileExporterInterface *iface = EV_FILE_EXPORTER_GET_IFACE (exporter);

	if (iface->end_page)
		iface->end_page (exporter);
}

void
ev_file_exporter_end (EvFileExporter *exporter)
{
        EvFileExporterInterface *iface = EV_FILE_EXPORTER_GET_IFACE (exporter);

        iface->end (exporter);
	ev_file_exporter_clear_print_settings (exporter);
}

EvFileExporterCapabilities
ev_file_exporter_get_capabilities (EvFileExporter *exporter)
{
	EvFileExporterInterface *iface = EV_FILE_EXPORTER_GET_IFACE (exporter);

	return iface->get_capabilities (exporter);
}
