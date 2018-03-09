

#ifndef XIDX_FILE_H_
#define XIDX_FILE_H_

#include "xidx.h"
//#include "xidx_list.h"
#include "elements/xidx_group.h"

namespace xidx{
  
  //class GroupList : public XidxList {};
  
class XidxFile{

private:
  std::vector<std::shared_ptr<Group> > groups;
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

  const std::vector<std::shared_ptr<Group> > getGroups() const { return groups; }

  inline size_t GetNumberOfGroups() const { return groups.size(); };
};

};

#endif
