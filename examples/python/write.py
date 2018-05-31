import XidxPy

from XidxPy import *

n_ts = 10
filepath = "test.xidx"

# create metadata file
meta = XidxFile(filepath)

# create time group
time_group = Group("TimeSeries", Group.TEMPORAL_GROUP_TYPE)

# set data source to the dataset file
source = DataSource("data", "file_path")
time_group.AddDataSource(source)

# create a list domain for the temporal group
time_dom = ListDomainDouble("Time")

# add attributes for the domain
time_dom.AddAttribute("units", "days since 1980")
time_dom.AddAttribute("calendar", "gregorian")

# add time values
for i in range(0,n_ts):
  ret = time_dom.AddDomainItem(float(i)*0.25)
  # you can also use a tuple (e.g., bounds for netcdf)
  #time_dom.AddDomainItems(IndexSpace([float(i)*0.25,float(i)*0.50]))

# set group time domain
time_group.SetDomain(time_dom)

# create grid domain 
geo_dom = MultiAxisDomain("Geospatial")

# create axis
latitude_axis = Variable("latitude");
longitude_axis = Variable("longitude");

# Populate the axis with explicit values (will be written in the XML)
for i in range(0,10):
  latitude_axis.AddValue(float(i)*0.5)
  longitude_axis.AddValue(float(i)*2*0.6)
  # you can also use a tuple (e.g., bounds for netcdf)
  #longitude_axis.AddValues(IndexSpace([float(i)*2*0.6,float(i)*2*1.2]))

# add attributes to axis
latitude_axis.AddAttribute("units", "degrees_north")
latitude_axis.AddAttribute("units", "degrees_east")

# add axis to the domain
geo_dom.AddAxis(latitude_axis)
geo_dom.AddAxis(longitude_axis)

geo_vars = Group("geo_vars", Group.SPATIAL_GROUP_TYPE, geo_dom);
  
# create and add a variable to the group
temp = geo_vars.AddVariable("geo_temperature", XidxDataType.FLOAT_32);
  
# add attribute to the variable (key-value) pairs
temp.AddAttribute("unit", "Celsius");
temp.AddAttribute("valid_min", "-100.0");
temp.AddAttribute("valid_max", "200.0");

time_group.AddGroup(geo_vars);
  
# set the root group of the metadata
meta.SetRootGroup(time_group);
# write to disk
meta.Save();

print ("write done")