

#ifndef IDX_METADATA_PARSE_UTILS_H_
#define IDX_METADATA_PARSE_UTILS_H_

#include <cassert>
#include <string>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "idx_metadata_datatypes.h"

namespace idx_metadata{
class Level;
}

namespace idx_metadata{
  int parse_level(xmlNode *grid, std::shared_ptr<Level> lvl);

};

#endif
