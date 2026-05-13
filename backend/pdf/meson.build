backend_deps = backends_common_deps + [
  cairo_pdf_dep,
  cairo_ps_dep,
  poppler_glib_dep,
]

backend_sources = files('ev-poppler.c')

test_ev_poppler_annotations = executable(
  'test-ev-poppler-annotations',
  'test-ev-poppler-annotations.c',
  include_directories: top_inc,
  dependencies: poppler_glib_dep,
  c_args: '-DEVINCE_COMPILATION',
)

test('test-ev-poppler-annotations', test_ev_poppler_annotations)
