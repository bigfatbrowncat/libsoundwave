include_directories(${libmpc_SOURCE_DIR}/include)
if(SHARED)
  add_library(mpcdec SHARED huffman.c mpc_decoder.c mpc_reade.cr streaminfo.c mpc_bits_reader.c mpc_demux.c requant.c synth_filter.c ${libmpc_SOURCE_DIR}/common/crc32.c)
else(SHARED)
  add_library(mpcdec_static STATIC huffman.c mpc_decoder.c mpc_reader.c streaminfo.c mpc_bits_reader.c mpc_demux.c requant.c synth_filter.c ${libmpc_SOURCE_DIR}/common/crc32.c)
endif(SHARED)

