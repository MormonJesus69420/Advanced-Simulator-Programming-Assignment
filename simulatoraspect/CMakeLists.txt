project(simulatoraspect)

set(HDRS
  objectcontrollerbackend.h
  collision_algorithms.h
  simulatoraspecttypes.h
  objectcontroller.h
  simulatoraspect.h
  simulatorjob.h
  simulator.h
  structs.h
  )

set( SRCS
  objectcontrollerbackend.cpp
  collision_algorithms.cpp
  objectcontroller.cpp
  simulatoraspect.cpp
  simulatorjob.cpp
  simulator.cpp
  )

qt5_wrap_cpp( HDRS_MOC
  simulatoraspect.h
  objectcontroller.h
  )

# Add library
add_library(${PROJECT_NAME} ${HDRS} ${SRCS} ${HDRS_MOC})

# TURN ON C++17 FEATURES
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17 )

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
    -Wno-conversion
    -Wno-padded # Qt
    >
  PUBLIC $<$<CXX_COMPILER_ID:GNU>:
    -pedantic -Wall -Werror
    >
    )

# Link target
target_link_libraries (${PROJECT_NAME}
  gmlib2::gmlib2qt
  )
