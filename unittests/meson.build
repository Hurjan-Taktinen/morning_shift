test('example',
  executable(
    'exampletests',
    ['example.cpp'],
    include_directories : INCLUDE_DIRS,
    dependencies : [CATCH2]
  )
)

test('varint',
  executable(
    'varinttests',
    ['varint.cpp'],
    include_directories : INCLUDE_DIRS,
    dependencies : [CATCH2],
    link_with: [utils],
  )
)
