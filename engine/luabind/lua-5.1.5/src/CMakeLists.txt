SET(SRC_LIST lapi.c
			 lbaselib.c
			 ldblib.c
			 ldo.c
			 lfunc.c
			 linit.c
			 llex.c
			 lmem.c
			 lobject.c
			 loslib.c
			 lstate.c
			 lstrlib.c
			 ltablib.c
			 lundump.c
			 lzio.c
			 lauxlib.c
			 lcode.c
			 ldebug.c
			 ldump.c
			 lgc.c
			 liolib.c
			 lmathlib.c
			 loadlib.c
			 lopcodes.c
			 lparser.c
			 lstring.c
			 ltable.c
			 ltm.c
			 lvm.c
			 print.c)

if (WIN32)             
  source_group(baselib FILES ${BASELIB_LIST})
  
else()
  SET(CMAKE_C_FLAGS "-Wall -ggdb -pipe -march=i386 -mtune=i686")
  SET(CMAKE_CXX_FLAGS "-Wall -ggdb -pipe -march=i386 -mtune=i686")
endif()


SET(LIBRARY_OUTPUT_PATH ${TF_CURRENT_BINARY_PATH_LIB})

ADD_LIBRARY(lua STATIC ${SRC_LIST})


