#pragma once

#if !defined (EVINCE_COMPILATION)
#error "This is a private header."
#endif

static inline void
ev_poppler_outline_title_make_single_line (char *title)
{
	if (!title)
		return;

	for (char *p = title; *p; p++) {
		if (*p == '\n' || *p == '\r')
			*p = ' ';
	}
}
