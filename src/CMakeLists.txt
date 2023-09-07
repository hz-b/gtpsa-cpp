# already set here as it is required in c and c++
# part
set(gtpsa_CMAKE_LIB_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/gtpsa")


# -- describing package already here ... c / c++
# are subcomponents
message(STATUS "Building gtpsa.cmake")

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/gtpsaConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/gtpsaConfig.cmake"
    INSTALL_DESTINATION ${gtpsa_CMAKE_LIB_DIR}
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/gtpsaConfigVersion.cmake
    VERSION "${GTPSA_VERSION}"
    COMPATIBILITY SameMajorVersion
    # check the destination file
    # INSTALL_DESTINATION ${gtpsa_CMAKE_LIB_DIR}
)

# install(TARGETS gtpsa
#         EXPORT gtpsaTargets
#         LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#         ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#         INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )
#
# install(
#     EXPORT gtpsaTargets
#     FILE gtpsaTargets.cmake
#     DESTINATION ${gtpsa_CMAKE_LIB_DIR}
#     NAMESPACE gtpsa::
# )

#install(
#    EXPORT gtpsaTargets
#    FILE gtpsaTargets.cmake
#    DESTINATION ${gtpsa_CMAKE_LIB_DIR}
#    NAMESPACE gtpsa::
#)

install(FILES
         "${CMAKE_CURRENT_BINARY_DIR}/gtpsaConfig.cmake"
         "${CMAKE_CURRENT_BINARY_DIR}/gtpsaConfigVersion.cmake"
	 DESTINATION ${gtpsa_CMAKE_LIB_DIR}
)



add_subdirectory(c)

# Required info in the directories below
# need to build proper package support
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/c++)
add_subdirectory(c++)
