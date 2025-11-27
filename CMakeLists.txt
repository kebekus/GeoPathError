cmake_minimum_required(VERSION 3.30)
project(GeoPathError LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Location)
find_package(Boost REQUIRED)

qt_standard_project_setup()

qt_add_executable(GeoPathError
    main.cpp
)
target_include_directories(GeoPathError
    PRIVATE
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(GeoPathError
    PRIVATE
    Qt::Core
    Qt::Location
)

include(GNUInstallDirs)

install(TARGETS GeoPathError
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
