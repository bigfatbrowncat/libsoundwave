add_definitions(-DYY_NEVER_INTERACTIVE)
include_directories(${libcuefile_SOURCE_DIR}/include)

add_library(cuefile-static STATIC cd.c cdtext.c cue_parse.c cue_print.c cue_scan.c cuefile.c time.c toc.h toc_parse.c toc_print.c toc_scan.c)
set_target_properties(cuefile-static PROPERTIES OUTPUT_NAME cuefile CLEAN_DIRECT_OUTPUT 1)

#add_library(cuefile-shared SHARED cd cdtext cue_parse cue_print cue_scan cuefile time toc toc_parse toc_print toc_scan)
#set_target_properties(cuefile-shared PROPERTIES OUTPUT_NAME cuefile CLEAN_DIRECT_OUTPUT 1 VERSION 0.0.0 SOVERSION 0)

install(TARGETS cuefile-static  LIBRARY DESTINATION "lib${LIB_SUFFIX}" ARCHIVE DESTINATION "lib${LIB_SUFFIX}") # cuefile-shared
