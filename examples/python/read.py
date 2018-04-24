import XidxPy
import sys

from test import * 
from XidxPy import *

if(len(sys.argv) < 2):
  print("usage: ", sys.argv[0], "<input file>")
  exit(1)

filepath = sys.argv[1]
metadata = XidxFile(filepath)

ret = metadata.Load()

root_group = metadata.GetRootGroup()
  
domain = root_group.GetListDomain()

# print (isinstance(domain, ListDomainDouble))
# print ("Time Domain[",Domain.ToString(domain.type),"]:")

timesteps = domain.GetLinearizedIndexSpace()

for att in domain.GetAttributes():
  print ("\t\tAttribute:", att.name, "Value:", att.value)

#print timesteps
#print timesteps.size()

for t in domain.GetLinearizedIndexSpace():
  print ("Timestep", t)
    
  for grid in root_group.GetGroups():
    grid_domain = grid.GetDomain()
      
    print ("\tGrid Domain[", Domain.ToString(grid_domain.type), "]")
    #print (isinstance(domain, MultiAxisDomain))
    # mdom = grid.GetMultiAxisDomain()

    # print (len(mdom.GetAttributes()))

    for att in grid_domain.GetAttributes():
      print ("\t\tAttribute:", att.name, "Value:", att.value)
      
    if(grid_domain.type == Domain.SPATIAL_DOMAIN_TYPE):
      sdom = grid_domain.GetSpatialDomain()
      print ("\tTopology", Topology.ToString(sdom.topology.type), "volume ", sdom.GetVolume())
      print ("\tGeometry", Geometry.ToString(sdom.geometry.type))
    elif(grid_domain.type == Domain.MULTIAXIS_DOMAIN_TYPE):
      mdom = grid.GetMultiAxisDomain()
      
      for a in range(0,mdom.GetNumberOfAxis()):
        axis = mdom.GetAxis(a);
        print ("\tAxis", axis.name,"volume", axis.GetVolume())
        for att in axis.GetAttributes():
          print ("\t\tAttribute:", att.name, "Value:", att.value)
  
      print("\n");
      
      for var in grid.GetVariables():
        source = var.GetDataItems()[0].GetDataSource();
        print("\t\tVariable:", var.name)
        #print(source,var.GetDataItems()[0])
        if(source):
          print("data source url: ", source.GetUrl())
        else:
          print("\n")
        
        for att in var.GetAttributes():
          print ("\t\tAttribute:", att.name, "Value:", att.value)
        
  # (Debug) Saving the content in a different file to compare with the original
  metadata.Save("verify.xidx");
  print("debug saved into verify.xidx")


