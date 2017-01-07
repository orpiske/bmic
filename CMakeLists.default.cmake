set(RUNTIME_DIR "bin")
set(CPP_INCLUDE_DIR "include")
set(SHARED_DIR "share")

if (CMAKE_COMPILER_IS_GNUCXX)
	if (BUILD_WITH_EXTRA_DEBUG)
			set(BMIC_DEBUG_DEFINES "-DBMIC_DEBUG=2")
	endif (BUILD_WITH_EXTRA_DEBUG)

	set(CMAKE_C_FLAGS "-Wall -Wshadow -Wconversion -std=c99 -fstrict-aliasing -fstack-protector-all ${CMAKE_USER_C_FLAGS}" CACHE STRING
		"Flags used by the compiler during all build types." FORCE
	)

	set(CMAKE_C_FLAGS_DEBUG "-DNDEBUG ${BMIC_DEBUG_DEFINES} -fstrict-aliasing -fstack-protector-all -fdiagnostics-color=auto -g ${CMAKE_USER_C_FLAGS}" CACHE STRING
		"Flags used by the compiler during debug build." FORCE
	)

	set(CMAKE_C_FLAGS_RELEASE "-O2 -fomit-frame-pointer -fstrict-aliasing -fstack-protector-all -D_FORTIFY_SOURCE=1 ${CMAKE_USER_C_FLAGS}" CACHE STRING
		"Flags used by the compiler during release build." FORCE
	)
endif (CMAKE_COMPILER_IS_GNUCXX)

macro(SetLinkDirByPlatform PREFIX)
	if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
		if (${APP_BUILD_PLATFORM} EQUAL "64")
			link_directories(${PREFIX}/lib64)
		else(${APP_BUILD_PLATFORM} EQUAL "64")
			link_directories(${PREFIX}/lib)
		endif(${APP_BUILD_PLATFORM} EQUAL "64")
	else (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
		if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
			link_directories(${PREFIX}/lib)
		endif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
	endif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
endmacro(SetLinkDirByPlatform PREFIX)
