CLIENT_SOURCES = files('main.cpp')

subdir('config')
subdir('utils')
subdir('logs')

client = executable(
  meson.project_name(),
  sources : CLIENT_SOURCES,
  include_directories : INCLUDE_DIRS,
  link_with : [utils],
  dependencies : [BOOST, SPDLOG]
)
