INCLUDE_DIRECTORIES(/usr/include/mysql/)

SET(SRC_LIST base_database.cc
			base_db_obj.cc
			database.cc
			db_util.cc
			mysql/mysql_database.cc
			txtdb/txtdb.cc)

SET(LIBRARY_OUTPUT_PATH ${TF_CURRENT_BINARY_PATH_LIB})

ADD_LIBRARY(db3 STATIC ${SRC_LIST})
	  