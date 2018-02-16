# Minimum CMake version required
cmake_minimum_required(VERSION 3.8)

# Global name of the project
project(STE6245TemplateApp VERSION 0.1 LANGUAGES CXX)


####################################
# Find (extra) required Qt libraries
find_package(Qt5Core 5.9 REQUIRED) # cmake functionality
find_package(Qt53DQuickExtras 5.9 REQUIRED)


####################
# Configure gmlib2qt
find_package(gmlib2qt REQUIRED CONFIG NO_DEFAULT_PATH)


####################################
# Compile and build simulator aspect
add_subdirectory(simulatoraspect)


##########################
# Unittests and benchmarks
add_subdirectory(unittests)
add_subdirectory(benchmarks)


########################
# Compile and build demo
option(BUILD_APPLICATION "Build demo application" TRUE)
if(BUILD_APPLICATION)

  # Source files and friends
  set( HDRS
    guiapplication.h
    )

  set( SRCS
    guiapplication.cpp
    main.cpp
    )

  set( RCCS
    resources/qml/qml.qrc
    )
  qt5_add_resources( RCCS_MOC ${RCCS} )

  add_executable( ${PROJECT_NAME}
    ${HDRS}
    ${SRCS}
    ${RCCS_MOC} )

  # TURN ON C++17 FEATURES
  if(${CMAKE_VERSION} VERSION_LESS "3.8.0")
    message(STATUS "${PROJECT_NAME} requires compiling in mode supporting C++17 features.")
  else()
    target_compile_features(${PROJECT_NAME}
      PUBLIC $<$<CXX_COMPILER_ID:Clang>:cxx_std_17>
      PUBLIC $<$<CXX_COMPILER_ID:GNU>:cxx_std_17>
      )
  endif()

  # Comile definitions
  target_compile_definitions( ${PROJECT_NAME}
    PUBLIC $<$<CXX_COMPILER_ID:MSVC>:
      _USE_MATH_DEFINES
      >
    )

  # Compiler spesific options
  target_compile_options(${PROJECT_NAME}
    PUBLIC $<$<CXX_COMPILER_ID:Clang>:
      -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-documentation -pedantic -Werror
      -Wno-undefined-reinterpret-cast
      -Wno-used-but-marked-unused
      -Wno-exit-time-destructors
      -Wno-global-constructors
      -Wno-missing-prototypes
      -Wno-unreachable-code
      -Wno-weak-vtables
      -Wno-padded # Qt
      >
    PUBLIC $<$<CXX_COMPILER_ID:GNU>:
      -pedantic -Wall -Werror
      >
    PUBLIC $<$<CXX_COMPILER_ID:MSVC>:
      /std:c++latest
      >
      )


  ############################
  # Configure simulator aspect
  target_include_directories(${PROJECT_NAME} PRIVATE simulatoraspect)

  # Dependency: Qt
  target_link_libraries( ${PROJECT_NAME}
    simulatoraspect
    Qt5::3DQuickExtras
    gmlib2::gmlib2qt
    )

endif()
