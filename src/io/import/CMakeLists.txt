add_library(
  import
  coordinates.cpp
  coordinates_dlpoly.cpp
  coordinates_epsr.cpp
  coordinates_moscito.cpp
  coordinates_xyz.cpp
  data1d.cpp
  data1d_histo.cpp
  data1d_mint.cpp
  data1d_xy.cpp
  data2d.cpp
  data2d_cartesian.cpp
  data3d.cpp
  forces.cpp
  forces_dlpoly.cpp
  forces_moscito.cpp
  forces_simple.cpp
  trajectory.cpp
  trajectory_dlpoly.cpp
  values.cpp
  coordinates.h
  data1d.h
  data2d.h
  data3d.h
  forces.h
  trajectory.h
  values.h
)

include_directories(import PRIVATE ${PROJECT_SOURCE_DIR}/src ${PROJECT_BINARY_DIR}/src)
target_link_libraries(import PRIVATE base)
