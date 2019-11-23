cmake_minimum_required(VERSION 3.16)
project(scene_raytracing)

# Static linking
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# C++17
set(CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set_property(GLOBAL PROPERTY CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(OpenGL_GL_PREFERENCE GLVND)
add_compile_options(-std=c++17)

# Set here your code locations
#-----------------------------------------------------------------------------
set(MY_STB_DIR /root/rendering/my_stb)                 # https://github.com/optozorax/my_stb
set(EASYBMP_DIR /root/rendering/EasyBMP/source)        # https://github.com/zer011b/EasyBMP
set(JSON_DIR /root/rendering/json)                     # https://github.com/nlohmann/json
#-----------------------------------------------------------------------------
set(SPACE_OBJECTS_DIR /root/rendering/space_objects)   # https://github.com/optozorax/space_objects
set(PORTALS_OPENGL_DIR /root/rendering/portals_opengl) # https://github.com/optozorax/portals_opengl
set(PATH_TRACING_DIR /root/rendering/path-tracing)     # https://github.com/optozorax/path-tracing
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# For threads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
link_libraries(Threads::Threads)
add_compile_options(-pthread)
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
add_library(json STATIC 
	${JSON_DIR}/json.hpp
)
include_directories(${JSON_DIR}/)
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
add_library(portals_opengl STATIC 
	${PORTALS_OPENGL_DIR}/src/prtl_vis/scene_reader.cpp

	${PORTALS_OPENGL_DIR}/include/prtl_vis/scene_reader.h
)
include_directories(${PORTALS_OPENGL_DIR}/include/)
link_libraries(portals_opengl)
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
	${PATH_TRACING_DIR}/src/scene2.cpp
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
	${PATH_TRACING_DIR}/include/pt/pt2twg.h
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
	${PATH_TRACING_DIR}/include/pt/object/scene2.h
	${PATH_TRACING_DIR}/include/pt/object/cubemap.h
	${PATH_TRACING_DIR}/include/pt/object/texture_polygon.h
	${PATH_TRACING_DIR}/include/pt/object/sky.h
)
include_directories(${PATH_TRACING_DIR}/include)
link_libraries(path_tracing)
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
include_directories(${COLOR_DIR}/src)
#-----------------------------------------------------------------------------

add_executable(scene_raytracing ${PATH_TRACING_DIR}/scene_raytracing/scene_raytracing.cpp)
