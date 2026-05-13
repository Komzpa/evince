/*
 * Copyright (C) 2026 Darafei Praliaskouski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "config.h"

#include "ev-job-scheduler.h"
#include "ev-jobs.h"

typedef struct {
	EvJob parent;

	gint *run_started;
	gint *disposed;
	gint *disposed_on_main;
} TestThreadJob;

typedef EvJobClass TestThreadJobClass;

static GType test_thread_job_get_type (void);

G_DEFINE_TYPE (TestThreadJob, test_thread_job, EV_TYPE_JOB)

static gboolean
test_thread_job_run (EvJob *job)
{
	TestThreadJob *thread_job = (TestThreadJob *) job;

	g_atomic_int_set (thread_job->run_started, TRUE);

	while (!g_cancellable_is_cancelled (job->cancellable))
		g_usleep (1000);

	return FALSE;
}

static void
test_thread_job_dispose (GObject *object)
{
	TestThreadJob *job = (TestThreadJob *) object;

	g_atomic_int_set (job->disposed_on_main,
			  g_main_context_is_owner (g_main_context_default ()));
	g_atomic_int_set (job->disposed, TRUE);

	G_OBJECT_CLASS (test_thread_job_parent_class)->dispose (object);
}

static void
test_thread_job_init (TestThreadJob *job)
{
	EV_JOB (job)->run_mode = EV_JOB_RUN_THREAD;
}

static void
test_thread_job_class_init (TestThreadJobClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);
	EvJobClass *job_class = EV_JOB_CLASS (class);

	object_class->dispose = test_thread_job_dispose;
	job_class->run = test_thread_job_run;
}

static void
test_thread_job_final_unref_runs_on_main_context (void)
{
	gint run_started = FALSE;
	gint disposed = FALSE;
	gint disposed_on_main = FALSE;
	gint64 deadline = g_get_monotonic_time () + 5 * G_USEC_PER_SEC;
	TestThreadJob *job = g_object_new (test_thread_job_get_type (), NULL);

	job->run_started = &run_started;
	job->disposed = &disposed;
	job->disposed_on_main = &disposed_on_main;

	ev_job_scheduler_push_job (EV_JOB (job), EV_JOB_PRIORITY_NONE);

	while (!g_atomic_int_get (&run_started) &&
	       g_get_monotonic_time () < deadline)
		g_usleep (1000);

	g_assert_true (g_atomic_int_get (&run_started));

	ev_job_cancel (EV_JOB (job));
	g_object_unref (job);

	ev_job_scheduler_wait ();

	g_assert_true (g_atomic_int_get (&disposed));
	g_assert_true (g_atomic_int_get (&disposed_on_main));
}

int
main (int argc, char **argv)
{
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/ev-job-scheduler/thread-job/final-unref-main-context",
			 test_thread_job_final_unref_runs_on_main_context);

	return g_test_run ();
}
