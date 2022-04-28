option(CBENCH_COMPRESSOR_LIBPRESSIO "Enable LIBPRESSIO." OFF)

if (CBENCH_COMPRESSOR_LIBPRESSIO)
	set(MY_LIBS "")

    #Asks for SZ library + set that in EXTRA_LIBS
    find_package(LibPressio REQUIRED)
    find_package(std_compat REQUIRED)
    if (TARGET LibPressio::libpressio)
      set(MY_LIBS ${MY_LIBS} LibPressio::libpressio)

    	#Find other libraries
      set_property(GLOBAL APPEND PROPERTY definitions "-DCBENCH_HAS_LIBPRESSIO")
    else()
      message(FATAL_ERROR "Could NOT find LibPressio")
    endif()
endif()
