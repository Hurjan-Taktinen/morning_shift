CLIENT_SOURCES = files(
  'main.cpp',
  'application.cpp')

subdir('config')
subdir('logs')
subdir('mc')
subdir('network')
subdir('utils')

client = executable(
  meson.project_name(),
  sources : CLIENT_SOURCES,
  include_directories : INCLUDE_DIRS,
  link_with : [utils],
  dependencies : [THREAD, BOOST, SPDLOG, ZLIB],
  link_args : '-lfmt'
)
