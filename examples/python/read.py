import XidxPy
import sys

from test import * 
from XidxPy import *

n_ts = 10
filepath = "test.xidx" #sys.argv[1]
metadata = XidxFile(filepath)

ret = metadata.Load()

root_group = metadata.GetRootGroup()
  
domain = root_group.GetDomain()
  
#domain = ListDomainDouble(time_domain)
print(domain)
print ("Time Domain[",Domain.ToString(domain.type),"]:")

timesteps = domain.GetLinearizedIndexSpace()

#print timesteps
#print timesteps.size()
for t in domain.GetLinearizedIndexSpace():
  print ("Timestep", t)
    
  for grid in root_group.GetGroups():
    domain = grid.GetDomain();
      
    print ("\tGrid Domain[", Domain.ToString(domain.type), "]")
    print (isinstance(domain, MultiAxisDomain))
    print (domain)
    print (domain.GetAttributes().size())

    # for att in domain.GetAttributes():
    #     print "\t\tAttribute ", att.name, "value", att.value
      
    # if(domain.type == Domain.SPATIAL_DOMAIN_TYPE):
    #   sdom = domain #SpatialDomain
    #   print "\tTopology", Topology.ToString(sdom.topology.type), "volume ", sdom.GetVolume()
    #   print "\tGeometry", Geometry.ToString(sdom.geometry.type)
    # elif(domain.type == Domain.MULTIAXIS_DOMAIN_TYPE):
    #   mdom = domain #MultiAxisDomain

    #   for a in range(0,mdom.GetNumberOfAxis()):
    #     axis = mdom.GetAxis(a);
    #     print "\tAxis", axis.name,"volume", axis.GetVolume()
    #     for att in axis.GetAttributes():
    #       print "\t\tAttribute", att.name, "value", att.value.c_str()
      
  #     printf("\n");
      
  #     for(auto& var: grid->GetVariables()){
  #       DataSource* source = var->GetDataItems()[0]->GetDataSource();
  #       printf("\t\tVariable: %s ", var->name.c_str());
  #       if(source != nullptr)
  #         printf("data source url: %s\n", source->GetUrl().c_str());
  #       else printf("\n");
        
  #       for(auto att: var->GetAttributes()){
  #         printf("\t\t\tAttribute %s value %s\n", att.name.c_str(), att.value.c_str());
  #       }
  #     }
  #   }
    
  # }
  
  # // (Debug) Saving the content in a different file to compare with the original
  # metadata.Save("verify.xidx");

