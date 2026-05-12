/*
 * Copyright (C) 2026 Evince contributors
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 */

#include "config.h"

#include "ev-document-info.h"

#include <exempi/xmp.h>
#include <glib.h>

static void
test_pdf_keywords (void)
{
	const char xmp[] =
		"<x:xmpmeta xmlns:x=\"adobe:ns:meta/\">"
		"<rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\">"
		"<rdf:Description xmlns:pdf=\"http://ns.adobe.com/pdf/1.3/\" "
		"pdf:Keywords=\"GNOME\"/>"
		"</rdf:RDF>"
		"</x:xmpmeta>";
	EvDocumentInfo *info;

	info = ev_document_info_new ();
	ev_document_info_take_modified_datetime (info, g_date_time_new_now_utc ());

	g_assert_true (ev_document_info_set_from_xmp (info, xmp, -1));
	g_assert_true (info->fields_mask & EV_DOCUMENT_INFO_KEYWORDS);
	g_assert_cmpstr (info->keywords, ==, "GNOME");

	ev_document_info_free (info);
}

int
main (int argc, char *argv[])
{
	int retval;

	g_test_init (&argc, &argv, NULL);

	g_assert_true (xmp_init ());

	g_test_add_func ("/xmp/pdf-keywords", test_pdf_keywords);

	retval = g_test_run ();

	xmp_terminate ();

	return retval;
}
