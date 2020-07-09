###############################################################################
##                                                                           ##
##        Configuration-file for DOCFSM to build the DOC-FSM project         ##
##                                                                           ##
##---------------------------------------------------------------------------##
## File:     CMakeLists.txt                                                  ##
## Author:   Ulrich Becker                                                   ##
## Date:     10.01.2018                                                      ##
## Revision:                                                                 ##
###############################################################################
cmake_minimum_required( VERSION 2.8 )


set( DOCFSM_VERSION "1.3" )

#set( DEBUG 1 )

set( PROJECT_NAME docfsm )


execute_process( COMMAND git tag OUTPUT_VARIABLE DOCFSM_VERSION )
if( NOT DOCFSM_VERSION )
 set( DOCFSM_VERSION "1.3 no git" )
endif(NOT DOCFSM_VERSION )

set( GIT_REPOSITORY_URL "https://raw.githubusercontent.com/UlrichBecker/command_line_option_parser_cpp11/master/src/" )

function( DownloadIfNotPresent filename )
   if( NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${filename}" )
      execute_process( COMMAND wget "${GIT_REPOSITORY_URL}${filename}" )
   endif()
endfunction()

project( ${PROJECT_NAME} )

set( CLOP_LIB_FILENAME "/usr/lib64/libParseOptsCpp11.so" )

if( NOT EXISTS ${CLOP_LIB_FILENAME} )
   set( PARSE_OPTS_SRC "parse_opts.cpp" )
   DownloadIfNotPresent( parse_opts.hpp )
   DownloadIfNotPresent( ${PARSE_OPTS_SRC} )
   add_definitions( "-DCONFIG_CLOP_NO_OPTIONAL_ARG" )
endif()

add_definitions( "-I${CMAKE_CURRENT_SOURCE_DIR}" )
#add_definitions( "-DCONFIG_PRINT_CALLER_LIST" )

# GSI specific compiler switch for Autodoc its only necessary when this tool
# will used in the GSI (www.gsi.de).
add_definitions( "-DCONFIG_GSI_AUTODOC_OPTION" )

add_definitions( "-DCONFIG_POINT_AS_ENTRY_EXIT_STATE" )
add_definitions( "-D__DOCFSM_VERSION__=${DOCFSM_VERSION}" )

add_executable( ${PROJECT_NAME}  df_preparser.cpp
                                 df_keyword_pool.cpp
                                 os_execute.cpp
                                 df_cpp_caller.cpp
                                 df_transition_finder.cpp
                                 df_dotgenerator.cpp
                                 df_keyword.cpp
                                 df_attribute_reader.cpp
                                 df_commandline.cpp
                                 df_state_collector.cpp
                                 df_dot_keywords.cpp
                                 ${PARSE_OPTS_SRC}
                                 df_docfsm.cpp )

if( EXISTS ${CLOP_LIB_FILENAME} )
  target_link_libraries( ${PROJECT_NAME} ParseOptsCpp11 )
endif()

if( DEBUG )
  add_definitions( -D_DEBUG )
#  add_definitions( -D_DEBUG_PREPARSER )
  add_definitions( -D_DEBUG_TRANSITION_FINDER_FSM )
  add_definitions( -D_DEBUG_ATTRIBUTE_READER_FSM )
  add_definitions( -D_DEBUG_ATTRIBUTE_READER )
  add_definitions( -g -O0 )
  message( "INFO: Building ${PROJECT_NAME} Debug-version" )
else( DEBUG )
  add_definitions( -Werror )
  add_definitions( -DNDEBUG )
  add_definitions( -Os )
  message( "INFO: Building ${PROJECT_NAME} Release-version" )
endif( DEBUG )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

install( FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} 
         DESTINATION /usr/bin
         PERMISSIONS WORLD_EXECUTE WORLD_READ 
                     OWNER_EXECUTE OWNER_READ
                     GROUP_EXECUTE GROUP_READ
       )

#=================================== EOF ======================================
