

#ifndef IDX_METADATA_SIMPLE_LAYOUT_H_
#define IDX_METADATA_SIMPLE_LAYOUT_H_

#include "idx_metadata_layout.h"

namespace idx_metadata{

class IDX_Metadata_Simple_Layout : public IDX_Metadata_Layout{

public:

  IDX_Metadata_Simple_Layout(IDX_Metadata* idx_metadata) : IDX_Metadata_Layout(idx_metadata){};

  virtual int load();
  virtual int save();

};

};

#endif
