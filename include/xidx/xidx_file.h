

#ifndef XIDX_FILE_H_
#define XIDX_FILE_H_

#include "xidx.h"

namespace xidx{
  
  //class GroupList : public XidxList {};
  
class XidxFile{

private:
  std::shared_ptr<Group> root_group;
  std::string file_path;

  bool loaded;

public:

  XidxFile(std::string path) : file_path(path){};

  int load();
  int save();

  // std::string get_idx_file_path(int timestep, int level, CenterType ctype);
  // std::string get_md_file_path(){ return file_path; }
  // int set_md_file_path(const char* new_path){ set_correct_path(new_path); return 0; }

  // int clear(){ groups.clear(); return 0; }

  int SetRootGroup(std::shared_ptr<Group> _root_group){
    root_group = _root_group;
    return 0;
  }
  
  inline size_t GetNumberOfGroups() const { return root_group->groups.size(); };
};

}

#endif
