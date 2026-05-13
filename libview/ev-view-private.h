/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; c-indent-level: 8 -*- */
/* this file is part of evince, a gnome document viewer
 *
 *  Copyright (C) 2004 Red Hat, Inc
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

#pragma once

#if !defined (EVINCE_COMPILATION)
#error "This is a private header."
#endif

#include "ev-view.h"
#include "ev-document-model.h"
#include "ev-pixbuf-cache.h"
#include "ev-page-cache.h"
#include "ev-jobs.h"
#include "ev-image.h"
#include "ev-form-field.h"
#include "ev-selection.h"
#include "ev-view-cursor.h"

#include <math.h>

#define DRAG_HISTORY 10

struct GdkPoint {
	gint x;
	gint y;
};

typedef struct GdkPoint GdkPoint;

/* Information for middle clicking and moving around the doc */
typedef struct {
        gboolean in_drag;
	GdkPoint start;
	gdouble hadj;
	gdouble vadj;
	guint drag_timeout_id;
	guint release_timeout_id;
	GdkPoint buffer[DRAG_HISTORY];
	GdkPoint momentum;
	gboolean in_notify;
} DragInfo;

/* Autoscrolling */
typedef struct {
	gboolean autoscrolling;
	guint last_y;
	guint start_y;
	guint timeout_id;
} AutoScrollInfo;

/* Information for handling selection */
typedef struct {
	gboolean in_select;
	gboolean in_drag;
	GdkPoint start;
	GList *selections;
	EvSelectionStyle style;
} SelectionInfo;

/* Information for handling images DND */
typedef struct {
	gboolean in_drag;
	GdkPoint start;
	EvImage *image;
} ImageDNDInfo;

typedef enum {
	EV_PAN_ACTION_NONE,
	EV_PAN_ACTION_NEXT,
	EV_PAN_ACTION_PREV
} EvPanAction;

/* Annotation popup windows */
typedef struct {
	GtkWidget *window;
	guint      page;

	gboolean   visible;
} EvViewWindowChild;

typedef enum {
	SCROLL_TO_KEEP_POSITION,
	SCROLL_TO_PAGE_POSITION,
	SCROLL_TO_CENTER,
	SCROLL_TO_FIND_LOCATION,
} PendingScroll;

typedef struct _EvHeightToPageCache {
	gint rotation;
	gboolean dual_even_left;
	gdouble *height_to_page;
	gdouble *dual_height_to_page;
} EvHeightToPageCache;

/* Information for handling annotations */
typedef struct {
	GdkPoint         start;
	GdkPoint         stop;
	gboolean         adding_annot;
	EvAnnotationType type;
	EvAnnotation    *annot;
} AddingAnnotInfo;

typedef struct {
	GdkPoint      start;
	EvPoint       cursor_offset;
	gboolean      annot_clicked;
	gboolean      moving_annot;
	EvAnnotation *annot;
} MovingAnnotInfo;

/* Information for handling link preview thumbnails */
typedef struct {
	EvJob     *job;
	gdouble    left;
	gdouble    top;
	GtkWidget *popover;
	EvLink    *link;
	guint      delay_timeout_id;
} EvLinkPreview;

typedef struct _EvViewPrivate {
	EvDocument *document;

	/* Find */
	EvJobFind *find_job;
	GList **find_pages; /* Backwards compatibility. Contains EvFindRectangles's elements per page */
	gint find_page;     /* Page of active find result */
	gint find_result;   /* Index of active find result on find_pages[find_page]. For matches across
	                     * two lines (which comprise two EvFindRectangle's), this will always point
	                     * to the first one, i.e. the one where rect->next_line is TRUE */
	gboolean jump_to_find_result;
	gboolean highlight_find_results;

	EvDocumentModel *model;
	EvPixbufCache *pixbuf_cache;
	gsize pixbuf_cache_size;
	EvPageCache *page_cache;
	EvHeightToPageCache *height_to_page_cache;
	EvViewCursor cursor;

	GtkRequisition requisition;

	/* Scrolling */
	GtkAdjustment *hadjustment;
	GtkAdjustment *vadjustment;
	/* GtkScrollablePolicy needs to be checked when
	 * driving the scrollable adjustment values */
	guint hscroll_policy : 1;
	guint vscroll_policy : 1;

	gint scroll_x;
	gint scroll_y;

	guint update_cursor_idle_id;

	/* Delta sum for emulating normal scrolling */
	gdouble       total_delta;
	PendingScroll pending_scroll;
	gboolean      pending_resize;
	EvPoint       pending_point;

	/* Current geometry */

	gint start_page;
	gint end_page;
	gint current_page;

	gint rotation;
	gdouble scale;
	gint spacing;

	gboolean loading;
	gboolean can_zoom_in;
	gboolean can_zoom_out;
	gboolean continuous;
	gboolean dual_even_left;
	EvSizingMode sizing_mode;
	EvPageLayout page_layout;
	GtkWidget *loading_window;
	guint loading_timeout;
	gboolean allow_links_change_zoom;

	/* Common for button press handling */
	int pressed_button;

	/* Key bindings propagation */
	gboolean key_binding_handled;

	/* Information for middle clicking and dragging around. */
	DragInfo drag_info;

	/* Autoscrolling */
	AutoScrollInfo scroll_info;

	/* Selection */
	GdkPoint motion;
	guint selection_update_id;
	guint selection_scroll_id;

	SelectionInfo selection_info;

	/* Copy link address selection */
	EvLinkAction *link_selected;

	/* Image DND */
	ImageDNDInfo image_dnd_info;

	/* Annotations */
	GList             *window_children;
	AddingAnnotInfo    adding_annot_info;
	MovingAnnotInfo    moving_annot_info;
	GHashTable        *annot_window_map;
	gboolean           enable_spellchecking;

	/* Focus */
	EvMapping *focused_element;
	guint focused_element_page;
	guint child_focus_idle_id;

	/* Synctex */
	EvMapping *synctex_result;

	/* Caret navigation */
	gboolean caret_enabled;
	gint     cursor_offset;
	gint     cursor_page;
	gdouble  cursor_line_offset;
	gboolean cursor_visible;
	guint    cursor_blink_timeout_id;
	guint    cursor_blink_time;

	/* Gestures */
	GtkGesture *pan_gesture;
	GtkGesture *zoom_gesture;
	gdouble prev_zoom_gesture_scale;
	EvPanAction pan_action;

	/* Current zoom center */
	gdouble zoom_center_x;
	gdouble zoom_center_y;
	gboolean zoom_anchor_valid;
	gboolean zoom_anchor_pending_x;
	gboolean zoom_anchor_pending_y;
	gint zoom_anchor_page;
	EvPoint zoom_anchor_doc_point;
	gdouble zoom_anchor_widget_x;
	gdouble zoom_anchor_widget_y;
	guint zoom_anchor_clear_timeout_id;

	/* Link preview */
	EvLinkPreview link_preview;
} EvViewPrivate;

struct _EvViewClass {
	GtkWidgetClass parent_class;

        void     (*scroll)            (EvView         *view,
				       GtkScrollType   scroll,
				       GtkOrientation  orientation);
        void     (*handle_link)       (EvView         *view,
				       gint            old_page,
				       EvLink         *link);
        void     (*external_link)     (EvView         *view,
				       EvLinkAction   *action);
        void     (*popup_menu)        (EvView         *view,
				       GList          *items);
        void     (*selection_changed) (EvView         *view);
        void     (*sync_source)       (EvView         *view,
				       EvSourceLink   *link);
        void     (*annot_added)       (EvView         *view,
				       EvAnnotation   *annot);
        void     (*annot_cancel_add)  (EvView         *view);
        void     (*annot_changed)     (EvView         *view,
				       EvAnnotation   *annot);
        void     (*annot_removed)     (EvView         *view,
				       EvAnnotation   *annot);
        void     (*layers_changed)    (EvView         *view);
        gboolean (*move_cursor)       (EvView         *view,
				       GtkMovementStep step,
				       gint            count,
				       gboolean        extend_selection);
	void     (*activate)          (EvView         *view);
};

void _get_page_size_for_scale_and_rotation (EvDocument *document,
					    gint        page,
					    gdouble     scale,
					    gint        rotation,
					    gint       *page_width,
					    gint       *page_height);
void _ev_view_transform_view_point_to_doc_point (EvView       *view,
						 GdkPoint     *view_point,
						 GdkRectangle *page_area,
						 GtkBorder    *border,
						 double       *doc_point_x,
						 double       *doc_point_y);
void _ev_view_transform_view_rect_to_doc_rect (EvView       *view,
					       GdkRectangle *view_rect,
					       GdkRectangle *page_area,
					       GtkBorder    *border,
					       EvRectangle  *doc_rect);
void _ev_view_transform_doc_point_to_view_point (EvView   *view,
						 int       page,
						 EvPoint  *doc_point,
						 GdkPoint *view_point);
void _ev_view_transform_doc_point_by_rotation_scale (EvView   *view,
						 int       page,
						 EvPoint  *doc_point,
						 GdkPoint *view_point);
void _ev_view_transform_doc_rect_to_view_rect (EvView       *view,
					       int           page,
					       EvRectangle  *doc_rect,
					       GdkRectangle *view_rect);
void _ev_view_get_selection_colors (EvView  *view,
				    GdkRGBA *bg_color,
				    GdkRGBA *fg_color);
gint _ev_view_get_caret_cursor_offset_at_doc_point (EvView *view,
						    gint    page,
						    gdouble doc_x,
						    gdouble doc_y);

#define EV_VIEW_MIN_FREE_ZOOM 4.0

static inline gdouble
ev_view_min_free_scale (gdouble dpi)
{
	return EV_VIEW_MIN_FREE_ZOOM * dpi;
}

static inline gdouble
ev_view_max_scale_for_page (gsize   pixbuf_cache_size,
			    gdouble width,
			    gdouble height,
			    gdouble dpi)
{
	gdouble cache_scale;

	if (dpi <= 0)
		dpi = 1.0;

	if (width <= 0 || height <= 0)
		return ev_view_min_free_scale (dpi);

	cache_scale = sqrt (pixbuf_cache_size / (width * dpi * 4 * height * dpi)) * dpi;

	return MAX (cache_scale, ev_view_min_free_scale (dpi));
}

static inline gint
ev_view_zoom_anchor_scroll_position (gdouble anchor_view_pos,
				     gdouble anchor_widget_pos)
{
	return anchor_view_pos - anchor_widget_pos;
}

static inline void
ev_view_zoom_anchor_scroll_bounds (gdouble desired_scroll_value,
				   gdouble base_lower,
				   gdouble base_upper,
				   gdouble page_size,
				   gdouble *lower,
				   gdouble *upper)
{
	*lower = MIN (base_lower, desired_scroll_value);
	*upper = MAX (base_upper, desired_scroll_value + page_size);
}

static inline gint
ev_view_zoom_anchor_scroll_value (gdouble desired_scroll_value,
				  gdouble lower,
				  gdouble upper,
				  gdouble page_size)
{
	return CLAMP ((gint) round (desired_scroll_value),
		      (gint) lower,
		      (gint) MAX (lower, upper - page_size));
}

static inline gboolean
ev_view_zoom_anchor_matches_widget_position (gboolean anchor_valid,
					     gdouble  anchor_widget_x,
					     gdouble  anchor_widget_y,
					     gdouble  widget_x,
					     gdouble  widget_y)
{
	return anchor_valid &&
	       fabs (anchor_widget_x - widget_x) <= 0.5 &&
	       fabs (anchor_widget_y - widget_y) <= 0.5;
}

static inline void
ev_view_zoom_center_for_scroll (gboolean event_has_position,
				gdouble  event_x,
				gdouble  event_y,
				gboolean pointer_has_position,
				gint     pointer_x,
				gint     pointer_y,
				gint     widget_width,
				gint     widget_height,
				gdouble *x,
				gdouble *y)
{
	if (event_has_position) {
		*x = event_x;
		*y = event_y;
	} else if (pointer_has_position) {
		*x = pointer_x;
		*y = pointer_y;
	} else {
		*x = widget_width / 2.0;
		*y = widget_height / 2.0;
	}
}

static inline gboolean
ev_view_should_queue_draw_after_range_update (gboolean current_page_has_texture,
					      gboolean pending_resize)
{
	return current_page_has_texture || pending_resize;
}

static inline gboolean
ev_view_should_queue_draw_after_scale_change (gboolean scale_changed)
{
	return scale_changed;
}

static inline gboolean
ev_view_should_update_range_after_scale_change (gboolean has_document,
						gboolean has_pixbuf_cache,
						gboolean has_page_cache,
						gint     start_page,
						gint     end_page)
{
	return has_document &&
	       has_pixbuf_cache &&
	       has_page_cache &&
	       start_page >= 0 &&
	       end_page >= start_page;
}

static inline gboolean
ev_view_should_handle_size_allocate (gboolean has_document,
				     gboolean loading,
				     gboolean pending_resize)
{
	return has_document && (!loading || pending_resize);
}

void _ev_view_clear_selection (EvView   *view);
void _ev_view_set_selection   (EvView   *view,
			       GdkPoint *start_point,
			       GdkPoint *end_point);

void _ev_view_set_focused_element (EvView *view,
				   EvMapping *element_mapping,
				   gint page);
void _ev_view_focus_form_field    (EvView      *view,
				   EvFormField *field);

void _ev_view_ensure_rectangle_is_visible (EvView       *view,
					   GdkRectangle *rect);
