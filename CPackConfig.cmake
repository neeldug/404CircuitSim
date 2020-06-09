include(InstallRequiredSystemLibraries)

# Packaging support
set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_PACKAGE_VENDOR "CMake.org")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A blazing fast circuit simulator")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSIN_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENCE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

set(CPACK_SOURCE_IGNORE_FILES
    ./.git
    ./test/*
    ./dist
    ./bin/*
    ./env/*
    ./out/
    ./.*build.*
    ./\\\\.DS_Store
)