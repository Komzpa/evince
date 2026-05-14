#pragma once

#include <glib.h>

static inline gboolean
ev_previewer_print_action_enabled_for_settings (gboolean settings_provided,
						gboolean settings_loaded)
{
	return !settings_provided || settings_loaded;
}
