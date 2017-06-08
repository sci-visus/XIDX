#include <libxml/xmlreader.h>
#include <libxml/xinclude.h>
#include "idx_metadata.h"

using namespace idx_metadata;

int main(int argc, char** argv){
  IDX_Metadata meta("test.xmf"); // default: use simple layout

  int ret = meta.load();

  // write the metadata to compare with original
  meta.set_path("load_test.xmf");
  meta.save();

  return ret;
}
