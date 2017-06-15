configure_file (
  "${PROJECT_SOURCE_DIR}/src/examples/example_defs.def.in"
  "${PROJECT_BINARY_DIR}/examples/example_defs.def"
  @ONLY
  )

install(FILES ${PROJECT_BINARY_DIR}/examples/example_defs.def
		DESTINATION bin)

