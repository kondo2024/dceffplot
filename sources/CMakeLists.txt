project(dceffplot) 

file(GLOB SRC_FILES "*.cc")
file(GLOB INC_FILES "*.hh")


add_library(dceffplot SHARED ${SRC_FILES})

install(FILES ${INC_FILES} DESTINATION include)
install(TARGETS dceffplot
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

#target_link_libraries(anaroot ${ROOT_LIBRARIES} anacore ananadeko anabrips anacatana anadali anaespri anasamurai anasilicons anawinds analoop analoopexample analoopencexample)
#

### for ANAROOT ---------------------------
if(DEFINED ENV{TARTSYS})
  message(STATUS "Found ANAROOT, TARTSYS = " $ENV{TARTSYS})
else()
  message(FATAL_ERROR "\$TARTSYS is not defined. please define the environment for ANAROOT.")
endif()

set(ANAROOT_LIBRARIES)
foreach(_cpt anaroot XMLParser)
  find_library(ANAROOT_${_cpt}_LIBRARY ${_cpt} $ENV{TARTSYS}/lib)
  if(ANAROOT_${_cpt}_LIBRARY)
    list(APPEND ANAROOT_LIBRARIES ${ANAROOT_${_cpt}_LIBRARY})
  endif()
endforeach()

include_directories($ENV{TARTSYS}/include)

### for ROOT ----------------------
if(DEFINED ENV{ROOTSYS})
  message(STATUS "Found ROOT, ROOTSYS = " $ENV{ROOTSYS})
else()
  message(FATAL_ERROR "\$ROOTSYS is not defined. please define the environment for ROOT.")
endif()

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS Core RIO Net Hist Graf Graf3d Gpad Tree Rint Postscript Matrix Physics MathCore Thread)
include(${ROOT_USE_FILE})
include_directories(${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})
