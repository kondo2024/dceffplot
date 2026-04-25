include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SRC_FILES "*.cc")
file(GLOB INC_FILES "*.hh")
file(GLOB TINC_FILES "T*.hh")

message(STATUS "Making dictionary dceffplot for ROOT")
set(LINKDEF_FILE "${CMAKE_CURRENT_BINARY_DIR}/dceffplot_linkdef.hh")

file(WRITE  ${LINKDEF_FILE} "#if defined(__CINT__) || defined(__CLING__)\n")
file(APPEND ${LINKDEF_FILE} "#pragma link off all globals;\n")
file(APPEND ${LINKDEF_FILE} "#pragma link off all classes;\n")
file(APPEND ${LINKDEF_FILE} "#pragma link off all functions;\n")

foreach(loop_var IN LISTS INC_FILES)
    get_filename_component(loop_var_filename "${loop_var}" NAME_WE)
    if(NOT ${loop_var_filename} MATCHES "linkdef")
        file(APPEND ${LINKDEF_FILE} "#pragma link C++ class ${loop_var_filename}+;\n")
    endif()
endforeach()
file(APPEND ${LINKDEF_FILE} "#endif\n")

ROOT_GENERATE_DICTIONARY(G__dceffplot
    ${TINC_FILES}
    LINKDEF ${LINKDEF_FILE}
)

add_library(dceffplot SHARED ${SRC_FILES} G__dceffplot.cxx)
target_link_libraries(dceffplot ${ROOT_LIBRARIES} XMLIO Geom -L${ANAROOT_LIB_DIR} anacore anasamurai)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/main.cc")
    add_executable(onlinemonitor main.cc)
    target_link_libraries(onlinemonitor dceffplot)
    install(TARGETS onlinemonitor DESTINATION bin)
endif()

install(TARGETS dceffplot LIBRARY DESTINATION lib)
install(FILES ${INC_FILES} DESTINATION include/dceffplot)

if (${ROOT_VERSION} VERSION_GREATER "6.0")
   install(FILES
      ${CMAKE_CURRENT_BINARY_DIR}/libdceffplot_rdict.pcm
      ${CMAKE_CURRENT_BINARY_DIR}/libdceffplot.rootmap
      DESTINATION lib/)
endif()
