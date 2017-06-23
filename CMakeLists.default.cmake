# uninstall
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/dist/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/dist/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/dist/cmake_uninstall.cmake)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug CACHE STRING
      	"Choose the type of build, options are: Debug, RelWithDebInfo or Release."
      	FORCE
	)
endif(NOT CMAKE_BUILD_TYPE)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-fdiagnostics-color=auto" HAS_COMPILER_COLORS)

if (CMAKE_COMPILER_IS_GNUCXX)
	if (BUILD_WITH_EXTRA_DEBUG)
			set(BMIC_DEBUG_DEFINES "-DBMIC_DEBUG=2")
	endif (BUILD_WITH_EXTRA_DEBUG)

	if (HAS_COMPILER_COLORS)
		set(COMPILER_COLOR_FLAGS_OPTS "-fdiagnostics-color=auto")
	else (HAS_COMPILER_COLORS)
		set(COMPILER_COLOR_FLAGS_OPTS "")
	endif (HAS_COMPILER_COLORS)

	set(CMAKE_C_FLAGS "-Wall -Wshadow -Wconversion -std=c99 -pedantic-errors -fstrict-aliasing -fstack-protector-all ${COMPILER_COLOR_FLAGS_OPTS} ${CMAKE_USER_C_FLAGS}" CACHE STRING
		"Flags used by the compiler during all build types." FORCE
	)

	set(CMAKE_C_FLAGS_DEBUG "-DNDEBUG ${BMIC_DEBUG_DEFINES} -fstrict-aliasing -fstack-protector-all -g ${CMAKE_USER_C_FLAGS}" CACHE STRING
		"Flags used by the compiler during debug build." FORCE
	)

	set(CMAKE_C_FLAGS_RELEASE "-O2 -fomit-frame-pointer -fstrict-aliasing -fstack-protector-all -D_FORTIFY_SOURCE=1 ${CMAKE_USER_C_FLAGS}" CACHE STRING
		"Flags used by the compiler during release build." FORCE
	)

	set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g -O2 -fstrict-aliasing -fstack-protector-all -D_FORTIFY_SOURCE=1 ${CMAKE_USER_C_FLAGS}" CACHE STRING
		"Flags used by the compiler during release build." FORCE
	)
endif (CMAKE_COMPILER_IS_GNUCXX)
