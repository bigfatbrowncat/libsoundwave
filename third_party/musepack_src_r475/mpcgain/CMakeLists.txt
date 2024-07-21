include_directories(${libmpc_SOURCE_DIR}/include)
include_directories(${libmpc_SOURCE_DIR}/../libreplaygain_r475/include)

link_directories(${libmpc_BINARY_DIR}/libmpcdec)

add_executable(mpcgain mpcgain.c)

target_link_libraries(mpcgain replaygain-static)

if(SHARED)
  target_link_libraries(mpcgain mpcdec)
else(SHARED)
  target_link_libraries(mpcgain mpcdec_static)
endif(SHARED)

if(NOT MSVC)
target_link_libraries(mpcgain m)
endif(NOT MSVC)

install(TARGETS mpcgain RUNTIME DESTINATION bin)
