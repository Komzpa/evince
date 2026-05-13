/* this file is part of evince, a gnome document viewer */

#pragma once

#if !defined (EVINCE_COMPILATION)
#error "This is a private header."
#endif

#include <glib.h>
#include <gdk/gdk.h>
#include <cairo.h>

#define EV_SIDEBAR_THUMBNAIL_WIDTH 160

static inline void
ev_sidebar_thumbnails_get_target_size (gdouble page_width,
					gdouble page_height,
					gint    rotation,
					gint   *width,
					gint   *height)
{
	gint thumbnail_height;

	g_return_if_fail (page_width > 0);
	g_return_if_fail (page_height > 0);
	g_return_if_fail (width != NULL);
	g_return_if_fail (height != NULL);

	thumbnail_height = MAX ((gint) (EV_SIDEBAR_THUMBNAIL_WIDTH * page_height / page_width + 0.5), 1);

	if (rotation == 90 || rotation == 270) {
		*width = thumbnail_height;
		*height = EV_SIDEBAR_THUMBNAIL_WIDTH;
	} else {
		*width = EV_SIDEBAR_THUMBNAIL_WIDTH;
		*height = thumbnail_height;
	}
}

static inline GdkTexture *
ev_sidebar_thumbnails_texture_new_for_surface (cairo_surface_t *surface)
{
	GdkTexture *texture;
	GBytes *bytes;

	g_return_val_if_fail (cairo_surface_get_type (surface) == CAIRO_SURFACE_TYPE_IMAGE, NULL);
	g_return_val_if_fail (cairo_image_surface_get_width (surface) > 0, NULL);
	g_return_val_if_fail (cairo_image_surface_get_height (surface) > 0, NULL);

	bytes = g_bytes_new_with_free_func (cairo_image_surface_get_data (surface),
					    cairo_image_surface_get_height (surface) *
					    cairo_image_surface_get_stride (surface),
					    (GDestroyNotify) cairo_surface_destroy,
					    cairo_surface_reference (surface));

	texture = gdk_memory_texture_new (cairo_image_surface_get_width (surface),
					  cairo_image_surface_get_height (surface),
					  GDK_MEMORY_DEFAULT,
					  bytes,
					  cairo_image_surface_get_stride (surface));

	g_bytes_unref (bytes);

	return texture;
}
