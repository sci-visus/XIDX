#include <libxml/xmlreader.h>
#include <libxml/xinclude.h>

#include "xidx/xidx.h"

using namespace xidx;

#define IndexSpace3d IndexSpace<3>

int main(int argc, char** argv){

  if(argc < 2){
    fprintf(stderr, "Usage: read file_path [debug]\n");

    return 1;
  }

  clock_t start, finish;
  start = clock();

  XidxFile metadata(argv[1]);

  int ret = metadata.Load();

  finish = clock();

  printf("Time taken %fms\n",(double(finish)-double(start))/CLOCKS_PER_SEC);

  std::shared_ptr<Group> root_group = metadata.GetRootGroup();
  
  std::shared_ptr<Domain> time_domain = root_group->GetDomain();
  
  //DomainType dom_type = time_domain->type;
  
  std::shared_ptr<TemporalListDomain> domain = std::static_pointer_cast<TemporalListDomain>(time_domain);
  
  printf("Time Domain[%s]:\n", ToString(domain->type));
  
  for(auto& t : domain->GetLinearizedIndexSpace()){
    printf("Timestep %f\n", t);
    
    for(auto& grid: root_group->GetGroups()){
      std::shared_ptr<Domain> domain = grid->GetDomain();
      
      printf("\tGrid Domain[%s]:\n", ToString(domain->type));
      
      for(auto& var: grid->GetVariables()){
        printf("\t\tVariable: %s %s\n", var->name.c_str(), var->data_items[0]->GetXPath().c_str());
        
      }
    }
    
  }
  
  
//    auto it = domain->GetLogicalIterator();
//    
//    IndexSpace3d is = domain->GetIndexSpace<3>();
//    IndexSpaceIterator<3> isit(is);
//    Rect<3> r = isit.rect;
//    PointInRectIterator<3, PHY_TYPE> pit(r);
    
    
    
    
    
//
//    Rect<3> r = is.bounds;
    // for(auto idx: is)
    //   //IndexSpaceSelection iss(domain, idx);
    //   Group g = root_group->GetInstance(idx)
    //   IndexSpace gis = g.GetIndexSpace()
    //   vector<SubSpace>
  
  
  
  
  metadata.Save("verify.xidx");
  
  if (argc < 3) // if no args just load, otherwise debug
    return 0;

#if 0
  //typedef std::list<std::shared_ptr<XidxGroup> > XidxGroupList
  XidxGroup root = meta.getRootGroup();

  XidxDomain time_dom = root.getDomain();

  //print time_dom

  for(auto item : root.getGroups()){
    
    for(auto grid : item.getGroups()){
      XidxDomain dom = grid.getDomain();
      
      for(auto var : grid->getVariables()){

        // print var

        // get pointer to the data 
        var.getFilePointers();

        // printf("Variable %s cent %s type %s num_type %s prec %s endian %s DType %s Format %s\n",
        //   var.name.c_str(), ToString(att.centerType), ToString(att.attributeType),
        //   ToString(att.data.numberType), att.data.precision.c_str(), ToString(att.data.endianType), att.get_dtype_str().c_str(), ToString(att.data.formatType));

          // if(var.data.formatType == FormatType::BINARY_FORMAT){
          //   std::vector<int> idims;
          //   std::istringstream f(att.data.dimensions);

          //   std::string s;
          //   while (getline(f, s, ' ')) {
          //     idims.push_back(atoi(s.c_str()));
          //   }

          //   size_t size = 1;
          //   for(auto d:idims)
          //     size *= d;
          //   char data[size];
          //   att.get_raw_data(data,size);
          //   printf("\t<< reading binary attribute of size %zu\n", size);
          // }

        for(auto att: var.getAttributes()){
          printf("\t Info %s = %s \n", att.name.c_str(), att.value.c_str());
        }
      }

    }

  }

#endif

  // add a timestep to the current metadata
  // std::shared_ptr<TimeStep> ts(new TimeStep());
  // std::shared_ptr<Level> level = meta.get_timestep(0)->get_level(0);

  // ret = ts->add_level(level);

  // ts->set_timestep(777, float(7777));
  // meta.add_timestep(ts);

  // write the metadata to compare with original
  //meta.set_md_file_path("load/test_hpc.xmf");
  // meta.save();

  return ret;
}
