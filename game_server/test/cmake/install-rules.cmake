install(
    TARGETS game_server_test_exe
    RUNTIME COMPONENT game_server_test_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
