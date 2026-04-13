set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/main.c PROPERTIES LANGUAGE C)
add_executable(CaraReportCExamples ${CMAKE_CURRENT_SOURCE_DIR}/main.c)
target_link_libraries(CaraReportCExamples PRIVATE CaraReport)

if (WIN32 AND CARAREPORT_BUILD_SHARED)
    add_custom_command(
        TARGET CaraReportCExamples PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:CaraReportCExamples> $<TARGET_FILE_DIR:CaraReportCExamples>
        COMMAND_EXPAND_LISTS
    )
endif ()