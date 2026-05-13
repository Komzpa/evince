/* this file is part of evince, a gnome document viewer
 *
 * Copyright (C) 2026 Darafei Praliaskouski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 */

#pragma once

#include <glib.h>

typedef enum {
	EV_WINDOW_METADATA_DOCUMENT_MODE_NONE,
	EV_WINDOW_METADATA_DOCUMENT_MODE_FULLSCREEN,
	EV_WINDOW_METADATA_DOCUMENT_MODE_PRESENTATION
} EvWindowMetadataDocumentMode;

static inline EvWindowMetadataDocumentMode
ev_window_metadata_document_mode (gboolean has_fullscreen,
				  gboolean fullscreen,
				  gboolean has_presentation,
				  gboolean presentation)
{
	if (has_presentation && presentation)
		return EV_WINDOW_METADATA_DOCUMENT_MODE_PRESENTATION;

	if (has_fullscreen && fullscreen)
		return EV_WINDOW_METADATA_DOCUMENT_MODE_FULLSCREEN;

	return EV_WINDOW_METADATA_DOCUMENT_MODE_NONE;
}
