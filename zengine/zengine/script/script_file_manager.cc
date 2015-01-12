// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "zengine/script/script_file_manager.h"

#include "base/logging.h"
#include "base/files/file_util.h"
#include "base/files/file_enumerator.h"
#include "base2/file_path_util.h"

//#include "base/string_util.h"

//const char kScriptsUtilPath[] = "../scripts2/util/";
//const char kScriptsMainPath[] = "../scripts2/app/";

namespace {
const int FILES_AND_DIRECTORIES =
        base::FileEnumerator::FILES |
        base::FileEnumerator::DIRECTORIES;
}

void ScriptFileManager::Initialize(const std::vector<base::FilePath>& file_paths) {
  // 保证只初始化一次
  if (!watcher_file_paths_.empty()) return;

  for (size_t i=0; i<file_paths.size(); ++i) {
    // 是目录
    if (base::DirectoryExists(file_paths[i])) {
      base::FileEnumerator enumerator(file_paths[i], true,
          FILES_AND_DIRECTORIES,
          FILE_PATH_LITERAL("*"));

      for (base::FilePath file2 = enumerator.Next(); !file2.value().empty(); file2 = enumerator.Next()) {
        base::FilePath file;
        if (!base::NormalizeFilePath(file2, &file)) {
          file = file2;
        }

        if (file.Extension() == FILE_PATH_LITERAL(".lua")) {
          base::NormalizeFilePath(file, &file);
          // 保证只加载一次
          if (script_datas_.find(file) == script_datas_.end()) {
            ScriptFileData script_fild_data;
            base::FileEnumerator::FileInfo file_info = enumerator.GetInfo();
            script_fild_data.SetLastModifiedTime(file_info.GetLastModifiedTime());
            script_datas_.insert(std::make_pair(file, script_fild_data));

            LOG(INFO) << "ScriptFileManager::Initialize - Ready load scipt file_path: " << file.AsUTF8Unsafe() << ".";
          }
        }
      }
    } else if (base::PathExists(file_paths[i])) {
      base::FilePath file;
      if (!base::NormalizeFilePath(file_paths[i], &file)) {
        file = file_paths[i];
      }
      // = file_paths[i];
      if (file.Extension() == FILE_PATH_LITERAL(".lua")) {
        // 保证只加载一次
        if (script_datas_.find(file) == script_datas_.end()) {
          base::File::Info file_info;
          if (base::GetFileInfo(file, &file_info)) {
            ScriptFileData script_fild_data;
            // base::File::Info file_info2;
            // bool file_exists = base::GetFileInfo(file, &file_info2);
            script_fild_data.SetLastModifiedTime(file_info.last_modified);
            script_datas_.insert(std::make_pair(file, script_fild_data));
            LOG(INFO) << "ScriptFileManager::Initialize - Ready load scipt file_path: " << file.AsUTF8Unsafe() << ".";
          }
        }
        
/*
        return RemoveDirectory(path.value().c_str()) != 0;

        file_util::FileEnumerator::FindInfo find_info;
        // enumerator.GetFindInfo(&find_info);
        script_fild_data.SetLastModifiedTime(file_util::FileEnumerator::GetLastModifiedTime(find_info));
        script_datas_.insert(std::make_pair(file_path_util::ToStringHack(file), script_fild_data));
*/
      }
    } else {
      LOG(ERROR) << "ScriptFileManager::Initialize - " << file_paths[i].AsUTF8Unsafe() << " is a invlaid file_path";
    }
  }

  watcher_file_paths_ = file_paths;

  //if (watcher_thread_ == NULL) {
  //  watcher_thread_ = new ScriptFileWatcherThread("script_file_watcher_thread");
  //  watcher_thread_->Start();
  //}
}

void ScriptFileManager::Destroy() {
  //if (watcher_thread_) {
  //  delete watcher_thread_;
  //  watcher_thread_ = NULL;
  //}
  watcher_file_paths_.clear();
}

// 开始监控
//void ScriptFileManager::DoWatchSignal() {
//  CHECK(watcher_thread_);
//  typedef base::ThreadEventCallBack0<void ()> ScriptFileWatcherCallBack;
//  ScriptFileWatcherCallBack* cb = new ScriptFileWatcherCallBack(boost::bind(&ScriptFileManager::OnWatchSignalThreadSafe, this));
//  watcher_thread_->PostEvent(cb);
//
//}
//
//void ScriptFileManager::RegisterWatcherFilePath(const std::string& file_path) {
//  watcher_file_paths_.push_back(file_path_util::FromStringHack(file_path));
//}

#if 0
// 需要重新加载的脚本
void ScriptFileManager::OnWatchSignalThreadSafe() {
  for (size_t i=0; i<watcher_file_paths_.size(); ++i) {
    boost::filesystem::path path(watcher_file_paths_[i]);
    if (!boost::filesystem::exists(path)) {
      LOG(ERROR) << "[ScriptManager::Initialize()] " << " scripts_datafile_path " << watcher_file_paths_[i] << " not exist!";
    } else {
      boost::filesystem::directory_iterator end_itr;
      for (boost::filesystem::directory_iterator itr(path); itr != end_itr; ++itr) {
        if (!base::strncasecmp(itr->path().extension().c_str(), ".lua", 4)) {
          std::string script_file_string = itr->path().file_string();
          base::linked_map<std::string, ScriptFileData>::iterator it = script_datas_.find(script_file_string);
          if (it == script_datas_.end()) {
            ScriptFileData script_fild_data;
            script_fild_data.SetLastModifiedTime(boost::filesystem::last_write_time(itr->path()));
            script_datas_.insert(std::make_pair(itr->path().file_string(), script_fild_data));
          } else {
            it->second.SetLastModifiedTime(boost::filesystem::last_write_time(itr->path()));
          }
        }
      }
    }
  }

  //DCHECK(io_service_);
  //DCHECK(script_manager_);
  //if (io_service_ && script_manager_) {
  //  io_service_->post(boost::bind(&ScriptManager::LoadScripts, script_manager_));
  //}
}
#endif
