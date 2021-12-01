project('morning_shift',
  'cpp',
  default_options : [
    'cpp_std=c++20',
    'buildtype=debug',
    'warning_level=3',
    'werror=false',
    'default_library=shared'],
  version : '0.0.1')

BOOST = dependency(
    'boost',
    modules : [
        'program_options',
    ],
    version : ['>=1.72']
)

SPDLOG_PROJ = subproject('spdlog')
SPDLOG = SPDLOG_PROJ.get_variable('spdlog_dep')

subdir('data')
subdir('include')
subdir('external')
subdir('src')
