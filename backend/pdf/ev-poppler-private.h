#pragma once

#include <poppler.h>

static inline PopplerAnnotFlag
ev_poppler_annot_text_icon_display_flags (PopplerAnnotFlag flags)
{
	return flags | POPPLER_ANNOT_FLAG_NO_ZOOM;
}
