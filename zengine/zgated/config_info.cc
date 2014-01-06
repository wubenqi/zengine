// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "zgated/config_info.h"

#include "base/logging.h"
#include "base/file_util.h"
#include "base/string_util.h"

#include "base2/file_path_util.h"
#include "base2/xml_ini_file.h"

int ConfigInfo::Initialize(const FilePath& config_file) {
	config_file_ = config_file;
	exe_file_ = config_file_.DirName();
	return Load();
}

int ConfigInfo::Load() {
	if (config_file_.empty()) {
		LOG(ERROR) << "config_file is empty()";
		return -1;
	}

	XMLIniFile xml_ini;
	if(!xml_ini.LoadXMLIniFile(file_path_util::ToStringHack(config_file_).c_str())) {
		LOG(ERROR) << "parser config_file " << file_path_util::ToStringHack(config_file_) << " error.";
		return -1;
	}
	
	Destroy();

	xml_ini.GetString("Client", "service_host", client_node_.instance_name, "client");
	xml_ini.GetString("Client", "listen_host", client_node_.addr,"0.0.0.0");
	xml_ini.GetString("Client", "listen_port", client_node_.port, "9900");

  xml_ini.GetString("Server", "service_host", server_node_.instance_name, "login");
  xml_ini.GetString("Server", "listen_host", server_node_.addr,"0.0.0.0");
  xml_ini.GetString("Server", "listen_port", server_node_.port, "9901");

	return 0;
}

void ConfigInfo::Destroy() {
	client_node_.Destroy();
  server_node_.Destroy();
}
