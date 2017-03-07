cmake_minimum_required(VERSION 2.6 FATAL_ERROR)


############ Make your changes here ############

set(mytarget    Legend       )  # equivalent to "name := fisrtTest" in G4 GNUmakefiles
set(useROOT     true           )  # use true or false (or comment to set to false)

#set(myROOTclass TMyRootEvent.hpp)  # comment if none (please see http://root.cern.ch/phpBB3//viewtopic.php?t=6172)


############ What's below should not need to be changed ############

# http://www.cmake.org/cmake/help/cmake_tutorial.html
# http://www.cmake.org/cmake/help/cmake2.6docs.html
# http://www.cmake.org/Wiki/CMake_FAQ

set(mymain ${mytarget}.cc)
set(myexe  ${mytarget}   )
set(myproj ${mytarget}_  )  # any name as long as it is different from myexe

project(${myproj})


set(LROOTLIB LRoot )
MESSAGE( PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}  )

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED COMPONENTS RIO )
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/LRoot)

# root 
if(useROOT)
	#if(myROOTclass)
	#	EXECUTE_PROCESS(COMMAND rootcint -f dict.cxx -c ${PROJECT_SOURCE_DIR}/include/${myROOTclass} ${PROJECT_SOURCE_DIR}/include/linkdef.h)
	#endif(myROOTclass)
	EXECUTE_PROCESS(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")
  list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
  find_package( ROOT REQUIRED) 
endif(useROOT)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(${myexe} ${mymain} ${sources} ${headers})

add_library(LRoot SHARED IMPORTED)
set_property(TARGET LRoot PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/LRoot/ )
MESSAGE( Geant4_LIBRARIES = ${Geant4_LIBRARIES} )
find_library( LPATH LRoot PATHS ${PROJECT_SOURCE_DIR}/LRoot/ )  
MESSAGE( LPATH = ${LPATH})
target_link_libraries(${myexe} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} ${LPATH} )
#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(Legend_SCRIPTS
  vis.mac
  )

foreach(_script ${Legend_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS Legend DESTINATION bin)


