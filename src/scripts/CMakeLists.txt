file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/scripts)
configure_file (
  "${PROJECT_SOURCE_DIR}/src/scripts/inst_app.sh.in"
  "${PROJECT_BINARY_DIR}/scripts/inst_app.sh"
  @ONLY
  )

configure_file (
  "${PROJECT_SOURCE_DIR}/src/scripts/dump_mods.sh.in"
  "${PROJECT_BINARY_DIR}/scripts/dump_mods.sh"
  @ONLY
  )

configure_file (
  "${PROJECT_SOURCE_DIR}/src/scripts/dump_symbols.sh.in"
  "${PROJECT_BINARY_DIR}/scripts/dump_symbols.sh"
  @ONLY
  )

configure_file (
  "${PROJECT_SOURCE_DIR}/src/scripts/find_symbol_prefix.sh.in"
  "${PROJECT_BINARY_DIR}/scripts/find_symbol_prefix.sh"
  @ONLY
  )

install(FILES ${PROJECT_BINARY_DIR}/scripts/inst_app.sh 
		${PROJECT_BINARY_DIR}/scripts/dump_mods.sh 
		${PROJECT_BINARY_DIR}/scripts/dump_symbols.sh 
		${PROJECT_BINARY_DIR}/scripts/find_symbol_prefix.sh 
		DESTINATION bin)

