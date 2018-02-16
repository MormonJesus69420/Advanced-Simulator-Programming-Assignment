# Build benchmark rules
option(BUILD_BENCHMARKS "Build benchmarks..." FALSE)
if(BUILD_BENCHMARKS)

  # Find GTest
  find_package(benchmark QUIET)
  if(benchmark_FOUND)

    message("Google Benchmark found!")

    # Enable cmake testing framework; ctest
    enable_testing()

    # Benchmarks
    set(BENCHS template_benchmark)

    # Add benchmarks
    foreach(BENCH ${BENCHS})

      set(BENCH_NAME "bench_${BENCH}")

      # Add executable benchmark target
      add_executable(${BENCH_NAME} ${BENCH}.cc)

      target_include_directories(${BENCH_NAME} PRIVATE ../simulatoraspect)

      # Add target properties
      target_link_libraries(${BENCH_NAME}
        simulatoraspect gmlib2::gmlib2qt
        benchmark::benchmark)
      # pthread)

      # TURN ON C++17 FEATURES
      target_compile_features(${BENCH_NAME} PUBLIC cxx_std_17 )

      # Compiler spesific options
      target_compile_options(${BENCH_NAME}
        PUBLIC $<$<CXX_COMPILER_ID:Clang>:
          -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-documentation -pedantic -Werror
          -Wno-padded # Qt
          -Wno-global-constructors
          -Wno-exit-time-destructors
          -Wno-weak-vtables
          -Wno-used-but-marked-unused
          -Wno-missing-prototypes
          -Wno-undefined-reinterpret-cast
          -Wno-unreachable-code
          -Wno-zero-as-null-pointer-constant
          >
        PUBLIC $<$<CXX_COMPILER_ID:GNU>:
          -pedantic -Wall -Werror
          >
          )

      # CMake add test
      add_test(NAME ${BENCH_NAME} COMMAND ${BENCH_NAME})

    endforeach(BENCH)
  else()
    message("Google Benchmark not found!")
  endif()

endif()
