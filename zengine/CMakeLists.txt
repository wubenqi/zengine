CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT (zengine)

IF(CMAKE_SYSTEM MATCHES Linux)
  ADD_DEFINITIONS(-D__LINUX__)
  ADD_DEFINITIONS(-D__linux)
  ADD_DEFINITIONS(-D_REENTRANT)
ELSE(CMAKE_SYSTEM MATCHES Linux)
  ADD_DEFINITIONS(-D__WINDOWS__)
ENDIF(CMAKE_SYSTEM MATCHES Linux)

ADD_DEFINITIONS(-DZENGINE_STATIC_LIB)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
INCLUDE_DIRECTORIES(./module)


SET(EXECUTABLE_OUTPUT_PATH ${TF_CURRENT_BINARY_PATH_BIN})

LINK_DIRECTORIES(${TF_CURRENT_BINARY_PATH_LIB})
LINK_DIRECTORIES(/usr/lib64/mysql)

SET (SERVER_LIST 
		./base/rsa_help.cc
		./base/rsa_help.h
		
		./zengine/zengine_context.cc
		./zengine/zengine_context.h
		./zengine/zengine_context_manager.cc
		./zengine/zengine_context_manager.h
		./zengine/zengine_daemon.cc
		./zengine/zengine_daemon.h
		./zengine/zengine_plugin_dll_func.h
		./zengine/zengine_plugin_dll_manager.cc
		./zengine/zengine_plugin_dll_manager.h
#		./zengine/zengine_plugin_factory_manager.cc
		./zengine/zengine_plugin_factory_manager.h
		./zengine/zengine_plugin.h
		./zengine/zengine_root.cc
		./zengine/zengine_root.h
		./zengine/zengine_thread.cc
		./zengine/zengine_thread.h
		./zengine/script/script_file_manager.cc
		./zengine/script/script_file_manager.h
		./zengine/script/script_manager.cc
		./zengine/script/script_manager.h
		./zengine/script/register_helper/base_register_helper.cc
		./zengine/script/register_helper/base_register_helper.h
		./zengine/script/register_helper/zengine_register_helper.cc
		./zengine/script/register_helper/zengine_register_helper.h
		
		./module/mod_db/module_db_dll.cc
		./module/mod_db/module_db_dll.h
		./module/mod_db/module_db_plugin.cc
		./module/mod_db/module_db_plugin.h
		./module/mod_db/register_helper/db_register_helper.cc
		./module/mod_db/register_helper/db_register_helper.h

#		./module/mod_http_client/module_http_client_dll.cc
#		./module/mod_http_client/module_http_client_dll.h
#		./module/mod_http_client/module_http_client_plugin.cc
#		./module/mod_http_client/module_http_client_plugin.h
#		./module/mod_http_client/znet_http_client.cc
#		./module/mod_http_client/znet_http_client.h
#		./module/mod_http_client/register_helper/http_client_register_helper.cc
#		./module/mod_http_client/register_helper/http_client_register_helper.h

		./module/mod_xml/module_xml_dll.cc
		./module/mod_xml/module_xml_dll.h
		./module/mod_xml/module_xml_plugin.cc
		./module/mod_xml/module_xml_plugin.h
		./module/mod_xml/zxml_wrapper.cc
		./module/mod_xml/zxml_wrapper.h
		./module/mod_xml/register_helper/xml_register_helper.cc
		./module/mod_xml/register_helper/xml_register_helper.h
	)

ADD_EXECUTABLE(zengine ${SERVER_LIST}
)

##update_end
#TARGET_LINK_LIBRARIES(zengine db3 script_engine net http_client base2 base url libevent mysqlclient lua curl2 pthread rt)
TARGET_LINK_LIBRARIES(zengine db3 script_engine net base2 base libevent mysqlclient lua pthread rt)

