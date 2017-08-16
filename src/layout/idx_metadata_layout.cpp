
#include <libxml/encoding.h>
#include "idx_metadata_parse_utils.h"
#include "idx_metadata_layout.h"

using namespace idx_metadata;

std::string IDX_Metadata_Layout::read_layout_type(){

  if(metadata == NULL){
    fprintf(stderr, "Metadata NULL file %s line %d\n", __FILE__, __LINE__);
    assert(false);
    return NULL;
  }

  LIBXML_TEST_VERSION;

  xmlDocPtr doc; /* the resulting document tree */
  int ret = 0;

  doc = xmlReadFile(metadata->get_md_file_path().c_str(), NULL, 0);
  if (doc == NULL) {
    fprintf(stderr, "Failed to parse %s\n", metadata->get_md_file_path().c_str());
    return NULL;
  }

  xmlNode* root_element = xmlDocGetRootElement(doc);
  const char* layout_type = getProp(root_element,"Layout");

  if(layout_type == NULL){
    fprintf(stderr, "Invalid Layout type\n");
    assert(false);
    return NULL;
  }

  return std::string(layout_type);

}