cmake_minimum_required(VERSION 3.10)

project(image-processing)

# Setting c++ standart
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# looking for gtk and Magick++ pakages
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
find_package(ImageMagick COMPONENTS Magick++ REQUIRED)
# find_package(GTK3 REQUIRED)

# setting definitions for Magick++
add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )


# adding link directories for macos
if (APPLE)
    link_directories(/usr/local/lib)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
# Adding executable file and setting directory for it
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../)
add_executable(${PROJECT_NAME} main.cpp img_processing_src/Image.cpp gui_src/GUI.cpp)


# adding headers files directories, libraries linking and compilator options
target_include_directories(${PROJECT_NAME} PRIVATE ${GTKMM_INCLUDE_DIRS} ${ImageMagick_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${GTKMM_LIBRARIES} ${ImageMagick_LIBRARIES})
target_compile_options(${PROJECT_NAME} PRIVATE ${GTKMM_CFLAGS_OTHER} ${ImageMagick_CXX_FLAGS})
