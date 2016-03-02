include (SquashPlugin)

if ("x86" STREQUAL "${CMAKE_SYSTEM_PROCESSOR}" OR
    "x86_64" STREQUAL "${CMAKE_SYSTEM_PROCESSOR}")
  set_compiler_specific_flags(
    VARIABLE sse_compiler_flags
    GCCISH "-msse4.1"
    MSVC "/arch:SSE4")

  squash_plugin (
    NAME lzsse
    SOURCES squash-lzsse.cpp
    CXX_STANDARD "c++11"
    EMBED_SOURCES
      lzsse/lzsse2/lzsse2.cpp
      lzsse/lzsse4/lzsse4.cpp
      lzsse/lzsse8/lzsse8.cpp
    COMPILER_FLAGS
      ${sse_compiler_flags})
endif()
