project(CaraReport)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/*.h")

if(CARAREPORT_BUILD_SHARED)
    set(_CaraReport_LIB_TYPE SHARED)
else()
    set(_CaraReport_LIB_TYPE STATIC)
endif()

add_library(${PROJECT_NAME} ${_CaraReport_LIB_TYPE} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PUBLIC _USE_DETAILED_FUNCTION_NAME_IN_SOURCE_LOCATION=0)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8 /WX /diagnostics:caret)
endif()

if(CARAREPORT_BUILD_SHARED)
    include(GenerateExportHeader)
    generate_export_header(${PROJECT_NAME}
                 BASE_NAME ${PROJECT_NAME}
                 EXPORT_MACRO_NAME CARAREPORT_API
                 EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/API.h
    )
else()
    file(WRITE ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/API.h "/* Auto-generated for static build */\n#pragma once\n#define CARAREPORT_API\n")
endif()


if(CARAREPORT_BUILD_SHARED)
    install(TARGETS ${PROJECT_NAME}
        EXPORT CaraReportTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
else()
    install(TARGETS ${PROJECT_NAME}
        EXPORT CaraReportTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()

install(EXPORT CaraReportTargets
    FILE CaraReportTargets.cmake
    NAMESPACE CaraReport::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CaraReport
)

install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/CaraReport.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/CaraReport
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
