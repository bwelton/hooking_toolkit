configure_file (
  "${PROJECT_SOURCE_DIR}/private/encrypt_data.sh.in"
  "${PROJECT_SOURCE_DIR}/private/encrypt_data.sh"
  @ONLY
  )
configure_file (
  "${PROJECT_SOURCE_DIR}/private/decrypt_data.sh.in"
  "${PROJECT_SOURCE_DIR}/private/decrypt_data.sh"
  @ONLY
  )
add_custom_target(decrypt
				  COMMAND bash ${PROJECT_SOURCE_DIR}/private/decrypt_data.sh
				 )

add_custom_target(encrypt
				  COMMAND bash ${PROJECT_SOURCE_DIR}/private/encrypt_data.sh
				 )
include("${PROJECT_SOURCE_DIR}/private/src/MakePrivate.cmake")
