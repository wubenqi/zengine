ADD_SUBDIRECTORY(./lua-5.1.5/src)

INCLUDE_DIRECTORIES(./lua-5.1.5/src)

SET(SRC_LIST class.cpp
           create_class.cpp
           wrapper_base.cpp
           class_info.cpp
           error.cpp
           scope.cpp
           exception_handler.cpp
           function.cpp
           open.cpp
           weak_ref.cpp
           detail/class_registry.cpp
           detail/inheritance.cpp
           detail/object_rep.cpp
           detail/stack_content_by_name.cpp
           detail/class_rep.cpp
           detail/link_compatibility.cpp
           detail/pcall.cpp)

SET(LIBRARY_OUTPUT_PATH ${TF_CURRENT_BINARY_PATH_LIB})

ADD_LIBRARY(luabind STATIC ${SRC_LIST})
	  