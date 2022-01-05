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

SPDLOG = subproject('spdlog').get_variable('spdlog_dep')
CATCH2 = subproject('catch2').get_variable('catch2_dep')
ZLIB = subproject('zlib').get_variable('zlib_dep')
THREAD = dependency('threads')
FMT = dependency('fmt')

add_project_arguments(
    [
        # '-DASIO_ENABLE_HANDLER_TRACKING',
    ],
    language : ['cpp']
)

subdir('data')
subdir('include')
subdir('external')
subdir('src')
subdir('unittests')
