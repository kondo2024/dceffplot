project(dceffplot) 

cmake_minimum_required(VERSION 3.0)

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

#----------------------------------
#----------------------------------
#----------------------------------


file(GLOB SRC_FILES "*.cc")
file(GLOB INC_FILES "*.hh")

add_library(dceffplot SHARED ${SRC_FILES} G__dceffplot.cxx)

install(FILES ${INC_FILES} DESTINATION include)
install(TARGETS dceffplot
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

#target_link_libraries(anaroot ${ROOT_LIBRARIES} anacore ananadeko anabrips anacatana anadali anaespri anasamurai anasilicons anawinds analoop analoopexample analoopencexample)
#



#### make dceffplot_linkdef.hh
message(STATUS "Making dictionary dceffplot for ROOT")
file(WRITE  dceffplot_linkdef.hh "#if defined(__CINT__) || defined(__CLING__)\n")
file(APPEND dceffplot_linkdef.hh "#pragma link off all globals;\n")
file(APPEND dceffplot_linkdef.hh "#pragma link off all classes;\n")
file(APPEND dceffplot_linkdef.hh "#pragma link off all functions;\n")

list(APPEND CMAKE_MESSAGE_INDENT "  ")
foreach(loop_var IN LISTS INC_FILES)
  message(STATUS "${loop_var}")
  get_filename_component(loop_var_filename "${loop_var}" NAME_WE)
  file(APPEND dceffplot_linkdef.hh "#pragma link C++ class ${loop_var_filename}+;\n")
endforeach()
list(POP_BACK CMAKE_MESSAGE_INDENT)
file(APPEND dceffplot_linkdef.hh "#endif\n")

ROOT_GENERATE_DICTIONARY(G__dceffplot
  ${INC_FILES}      
  LINKDEF dceffplot_linkdef.hh
)
###  ---------------------------
if (${ROOT_VERSION} VERSION_GREATER "6.0")
   install(
      FILES
      ${CMAKE_CURRENT_BINARY_DIR}/libdceffplot_rdict.pcm
      ${CMAKE_CURRENT_BINARY_DIR}/libdceffplot.rootmap
      DESTINATION lib/)
endif (${ROOT_VERSION} VERSION_GREATER "6.0")

