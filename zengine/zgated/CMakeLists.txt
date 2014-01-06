CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT (server)

IF(CMAKE_SYSTEM MATCHES Linux)
  ADD_DEFINITIONS(-D__LINUX__)
  ADD_DEFINITIONS(-D__linux)
  ADD_DEFINITIONS(-D_REENTRANT)
ELSE(CMAKE_SYSTEM MATCHES Linux)
  ADD_DEFINITIONS(-D__WINDOWS__)
ENDIF(CMAKE_SYSTEM MATCHES Linux)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

SET(EXECUTABLE_OUTPUT_PATH ${TF_CURRENT_BINARY_PATH_BIN})

#INCLUDE_DIRECTORIES(..)
INCLUDE_DIRECTORIES(../proto/cc/common ../proto/cc/editor ../proto/cc/server)
LINK_DIRECTORIES(${TF_CURRENT_BINARY_PATH_LIB})

SET (AGENTD_LIST 
	./agent_server.h
	./agent_server.cc 
	./config_info.h
	./config_info.cc
	./client_comm_handler.cc
	./client_comm_handler.h
	./client_comm_manager.h
	./client_comm_manager.cc
	./server_comm_manager.cc
	./server_comm_manager.h
	./server_comm_handler.h
	./server_comm_handler.cc
	)

ADD_EXECUTABLE(agentd ${AGENTD_LIST}
)

##update_end
TARGET_LINK_LIBRARIES(agentd proto net base2 base dynamic_annotations protobuf libevent boost_signals boost_filesystem boost_system pthread rt)

