# Build unittest rules
option(BUILD_UNITTESTS "Build unittests..." FALSE)
if(BUILD_UNITTESTS)

  # Find GTest
  find_package(GTest QUIET)
  if(GTEST_FOUND)

    message("GTest found!")

    # Add GTest include folder and libraries
    include_directories(${GTEST_INCLUDE_DIR})
    link_libraries(${GTEST_BOTH_LIBRARIES})

    # Enable cmake testing framework; ctest
    enable_testing()

    # Tests
    set(TESTS collision_algorithms_tests)

    # Add tests
    foreach(TEST ${TESTS})

      # Add executable unittest target
      add_executable( unittest_${TEST} ${HDRS} ${TEST}.cc  )

      # Adde target properties
      target_include_directories(unittest_${TEST} PRIVATE ../simulatoraspect)
      target_link_libraries(unittest_${TEST}
        simulatoraspect gmlib2::gmlib2qt
        ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})

      # TURN ON C++17 FEATURES
      target_compile_features(unittest_${TEST} PUBLIC cxx_std_17 )

      # Compiler spesific options
      target_compile_options(unittest_${TEST}
        PUBLIC $<$<CXX_COMPILER_ID:Clang>:
          -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-documentation -pedantic -Werror
          -Wno-zero-as-null-pointer-constant
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
          )

      # Google gtest proclamation
      gtest_add_tests( unittest_${TEST} "" AUTO )

    endforeach(TEST)
  else()
    message("GTest not found!")
  endif()
endif()
