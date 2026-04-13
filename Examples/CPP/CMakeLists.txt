add_executable(CaraReportCppExamples ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(CaraReportCppExamples PRIVATE CaraReport)

if (WIN32 AND CARAREPORT_BUILD_SHARED)
    add_custom_command(
        TARGET CaraReportCppExamples PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:CaraReportCppExamples> $<TARGET_FILE_DIR:CaraReportCppExamples>
        COMMAND_EXPAND_LISTS
    )
endif ()