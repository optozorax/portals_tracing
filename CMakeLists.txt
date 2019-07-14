cmake_minimum_required(VERSION 3.10.2)
project(portals_debug)

##############################################################################
# ENABLE WHEN DEBUG
set(CMAKE_BUILD_TYPE Debug)
##############################################################################

set_property(GLOBAL PROPERTY CXX_STANDARD 17)

#-----------------------------------------------------------------------------
# Set here your code locations
set(MY_STB_DIR ~/my/portals_project/my_stb)                 # https://github.com/nothings/stb + my_stb (see below)
set(EASYBMP_DIR ~/my/portals_project/EasyBMP)               # https://sourceforge.net/projects/easybmp/
set(SPACE_OBJECTS_DIR ~/my/portals_project/space_objects)   # https://github.com/optozorax/space_objects
set(PATH_TRACING_DIR ~/my/portals_project/path-tracing)     # https://github.com/optozorax/path-tracing
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# my_stb lib:
# my_stb/
#   include/
#     stb_image.h       # from original repo
#     stb_image_write.h # from original repo
#   src/
#     stb_image.cpp:
#       #define STB_IMAGE_IMPLEMENTATION
#       #define STBI_MSC_SECURE_CRT
#       #include <stb_image.h>
#     stb_image_write.cpp:
#       #define STB_IMAGE_WRITE_IMPLEMENTATION
#       #define STBI_MSC_SECURE_CRT
#       #include <stb_image_write.h>
# It maked because I used stb in many projects and got link errors
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# For threads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
link_libraries(Threads::Threads)
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
add_library(my_stb STATIC 
	${MY_STB_DIR}/src/stb_image.cpp 
	${MY_STB_DIR}/src/stb_image_write.cpp

	${MY_STB_DIR}/include/stb_image.h
	${MY_STB_DIR}/include/stb_image_write.h
)
include_directories(${MY_STB_DIR}/include)
link_libraries(my_stb)
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
add_library(easybmp STATIC 
	${EASYBMP_DIR}/EasyBMP.cpp 

	${EASYBMP_DIR}/EasyBMP.h
	${EASYBMP_DIR}/EasyBMP_DataStructures.h
	${EASYBMP_DIR}/EasyBMP_BMP.h
	${EASYBMP_DIR}/EasyBMP_VariousBMPutilities.h
)
include_directories(${EASYBMP_DIR}/)
link_libraries(easybmp)
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
add_library(space_objects STATIC 
	${SPACE_OBJECTS_DIR}/src/coordinates2.cpp 
	${SPACE_OBJECTS_DIR}/src/coordinates3.cpp

	${SPACE_OBJECTS_DIR}/include/spob/spob.h
	${SPACE_OBJECTS_DIR}/include/spob/coordinates3.h
	${SPACE_OBJECTS_DIR}/include/spob/vector2.h
	${SPACE_OBJECTS_DIR}/include/spob/coordinates2.h
	${SPACE_OBJECTS_DIR}/include/spob/common.h
	${SPACE_OBJECTS_DIR}/include/spob/vector3.h
	${SPACE_OBJECTS_DIR}/include/spob/spob2glm.h
)
include_directories(${SPACE_OBJECTS_DIR}/include)
link_libraries(space_objects)
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
add_library(path_tracing STATIC 
	# pt/
	${PATH_TRACING_DIR}/src/basics.cpp
	${PATH_TRACING_DIR}/src/camera.cpp
	${PATH_TRACING_DIR}/src/image.cpp
	${PATH_TRACING_DIR}/src/poly.cpp
	${PATH_TRACING_DIR}/src/pt2easybmp.cpp
	# ${PATH_TRACING_DIR}/src/pt2twg.cpp
	${PATH_TRACING_DIR}/src/renderer.cpp
	${PATH_TRACING_DIR}/src/transformation.cpp

	# pt/camera/
	${PATH_TRACING_DIR}/src/360.cpp 
	${PATH_TRACING_DIR}/src/orthogonal.cpp

	# pt/material/
	${PATH_TRACING_DIR}/src/light.cpp
	${PATH_TRACING_DIR}/src/reflect.cpp
	${PATH_TRACING_DIR}/src/refract.cpp
	${PATH_TRACING_DIR}/src/scatter.cpp

	# pt/object/
	${PATH_TRACING_DIR}/src/cubemap.cpp
	${PATH_TRACING_DIR}/src/mathroom.cpp
	${PATH_TRACING_DIR}/src/scene.cpp
	# ${PATH_TRACING_DIR}/src/scene.cpp
	${PATH_TRACING_DIR}/src/sky.cpp
	${PATH_TRACING_DIR}/src/texture_polygon.cpp

	# pt/shape/
	${PATH_TRACING_DIR}/src/contour.cpp
	${PATH_TRACING_DIR}/src/cylinder.cpp
	${PATH_TRACING_DIR}/src/polygon.cpp
	${PATH_TRACING_DIR}/src/portals.cpp
	${PATH_TRACING_DIR}/src/triangle.cpp
	${PATH_TRACING_DIR}/src/sphere.cpp

	# include
	${PATH_TRACING_DIR}/include/pt/pt2easybmp.h
	${PATH_TRACING_DIR}/include/pt/poly.h
	${PATH_TRACING_DIR}/include/pt/image.h
	# ${PATH_TRACING_DIR}/include/pt/pt2twg.h
	${PATH_TRACING_DIR}/include/pt/basics.h
	${PATH_TRACING_DIR}/include/pt/renderer.h
	${PATH_TRACING_DIR}/include/pt/pt.h
	${PATH_TRACING_DIR}/include/pt/object.h
	${PATH_TRACING_DIR}/include/pt/transformation.h
	${PATH_TRACING_DIR}/include/pt/camera.h
	${PATH_TRACING_DIR}/include/pt/camera/360.h
	${PATH_TRACING_DIR}/include/pt/camera/orthogonal.h
	${PATH_TRACING_DIR}/include/pt/shape/sphere.h
	${PATH_TRACING_DIR}/include/pt/shape/triangle.h
	${PATH_TRACING_DIR}/include/pt/shape/polygon.h
	${PATH_TRACING_DIR}/include/pt/shape/portals.h
	${PATH_TRACING_DIR}/include/pt/shape/contour.h
	${PATH_TRACING_DIR}/include/pt/shape/cylinder.h
	${PATH_TRACING_DIR}/include/pt/material/refract.h
	${PATH_TRACING_DIR}/include/pt/material/scatter.h
	${PATH_TRACING_DIR}/include/pt/material/reflect.h
	${PATH_TRACING_DIR}/include/pt/material/light.h
	${PATH_TRACING_DIR}/include/pt/object/mathroom.h
	${PATH_TRACING_DIR}/include/pt/object/scene.h
	# ${PATH_TRACING_DIR}/include/pt/object/scene.h
	${PATH_TRACING_DIR}/include/pt/object/cubemap.h
	${PATH_TRACING_DIR}/include/pt/object/texture_polygon.h
	${PATH_TRACING_DIR}/include/pt/object/sky.h
)
include_directories(${PATH_TRACING_DIR}/include)
link_libraries(path_tracing)
#-----------------------------------------------------------------------------

# Replace DEBUG to RELEASE if needed

add_executable(pt_example01_DEBUG ${PATH_TRACING_DIR}/examples/hello_world.cpp)
add_executable(pt_example02_DEBUG ${PATH_TRACING_DIR}/examples/polygon_test.cpp)
add_executable(pt_example03_DEBUG ${PATH_TRACING_DIR}/examples/portal_in_portal_anim.cpp)
add_executable(pt_example04_DEBUG ${PATH_TRACING_DIR}/examples/portal_light.cpp)
add_executable(pt_example05_DEBUG ${PATH_TRACING_DIR}/examples/portal_light_test.cpp)
#add_executable(pt_example06_DEBUG ${PATH_TRACING_DIR}/examples/portal_light_test2.cpp)
#add_executable(pt_example07_DEBUG ${PATH_TRACING_DIR}/examples/propeller.cpp)
#add_executable(pt_example08_DEBUG ${PATH_TRACING_DIR}/examples/scene_raytracing.cpp)
add_executable(pt_example09_DEBUG ${PATH_TRACING_DIR}/examples/standard_scene_2.cpp)
add_executable(pt_example10_DEBUG ${PATH_TRACING_DIR}/examples/standart_scene.cpp)
#add_executable(pt_example11_DEBUG ${PATH_TRACING_DIR}/examples/texture_polygon_test.cpp)
#add_executable(pt_example12_DEBUG ${PATH_TRACING_DIR}/examples/transparent_light.cpp)
#add_executable(pt_example13_DEBUG ${PATH_TRACING_DIR}/examples/transparent_test.cpp)