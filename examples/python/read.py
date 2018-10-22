import xidx
import sys

from xidx import *

if(len(sys.argv) < 2):
  print("usage: ", sys.argv[0], "<input file>")
  exit(1)

filepath = sys.argv[1]

# create metadata file object
metadata = MetadataFile(filepath)
# read data from input file
ret = metadata.Load()

# get root group (e.g. Time series)
root_group = metadata.GetRootGroup()

# get domain of the group
domain = root_group.GetDomain()

print ("Time Domain[",Domain.ToString(domain.type),"]:")

# print attributes if any
for att in domain.GetAttributes():
  print ("\t\tAttribute:", att.name, "Value:", att.value)

# loop over the list of timesteps
for t in domain.GetLinearizedIndexSpace():
  print ("Timestep", t)
    
  # loop over the groups (e.g. grids)
  for grid in root_group.GetGroups():
    # get domain of current grid
    grid_domain = grid.GetDomain()
    
    print ("\tGrid Domain[", Domain.ToString(grid_domain.type), "]")

    # print attributes if any
    for att in grid_domain.GetAttributes():
      print ("\t\tAttribute:", att.name, "Value:", att.value)

    if(grid_domain.type == Domain.SPATIAL_DOMAIN_TYPE):
      print ("\tTopology", Topology.ToString(grid_domain.topology.type), "volume ", grid_domain.GetVolume())
      print ("\tGeometry", Geometry.ToString(grid_domain.geometry.type))
    elif(grid_domain.type == Domain.MULTIAXIS_DOMAIN_TYPE):
      # loop over the axis 
      for a in range(0,grid_domain.GetNumberOfAxis()):
        # get axis
        axis = grid_domain.GetAxis(a);
        print ("\tAxis", axis.name,"volume", axis.GetVolume(),": [ ", end='')

        for v in axis.GetValues():
          print (v, end='')

        print(" ]")

        # print attributes of the axis if any
        for att in axis.GetAttributes():
          print ("\t\tAttribute:", att.name, "Value:", att.value)
  
      print("\n");
      
      # loop over variables
      for var in grid.GetVariables():
        # get datasource used by the first variable
        source = var.GetDataItems()[0].GetDataSource();
        print("\t\tVariable:", var.name)
        if(source):
          print("\t\t\tdata source url: ", source.GetUrl())
        else:
          print("\n")
        
        # print attributes of the variable if any
        for att in var.GetAttributes():
          print ("\t\tAttribute:", att.name, "Value:", att.value)
        
# (Debug) Saving the content in a different file to compare with the original
metadata.Save("verify.xidx");
print("debug saved into verify.xidx")


