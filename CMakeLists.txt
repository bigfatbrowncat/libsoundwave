cmake_minimum_required(VERSION 3.28)
project(libsoundwave)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(third_party/ogg)

add_subdirectory(third_party/vorbis)

set(INSTALL_MANPAGES OFF CACHE BOOL "No mans" FORCE)
add_subdirectory(third_party/flac)

add_subdirectory(third_party/opus)

set(OP_DISABLE_HTTP ON CACHE BOOL "" FORCE)     # because we don't want to depend on OpenSSL
set(OP_DISABLE_EXAMPLES ON CACHE BOOL "" FORCE)     # because examples need opusurl that needs http
set(OP_DISABLE_DOCS ON CACHE BOOL "" FORCE)     # because we don't want to depend on Doxygen
add_subdirectory(third_party/opusfile)

add_subdirectory(third_party/WavPack)

set(RTAUDIO_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(RTAUDIO_BUILD_STATIC_LIBS OFF CACHE BOOL "" FORCE)
set(RTAUDIO_API_DS ON CACHE BOOL "" FORCE)
set(RTAUDIO_TARGETNAME_UNINSTALL "rtaudio_uninstall" CACHE STRING "" FORCE)
add_subdirectory(third_party/rtaudio)

add_subdirectory(third_party/libcuefile_r475)
add_subdirectory(third_party/libreplaygain_r475)

set(CUEFILE_INCLUDE_DIR third_party/libcuefile_r475/include)
set(REPLAY_GAIN_INCLUDE_DIR third_party/libreplaygain_r475/include)
set(CUEFILE_LIBRARY cuefile-static)
set(REPLAY_GAIN_LIBRARY replaygain-static)
add_subdirectory(third_party/musepack_src_r475)

# modplug doesn't support unicode in some places. It's too old-school :)
remove_definitions(-DUNICODE)
remove_definitions(-D_UNICODE)
add_subdirectory(third_party/libmodplug)
add_definitions(-DDUNICODE)
add_definitions(-D_UNICODE)

#get_target_property(modplug_defs modplug COMPILE_DEFINITIONS)
#list(FILTER modplug_defs EXCLUDE REGEX [[^-DUNICODE$]])
#list(FILTER modplug_defs EXCLUDE REGEX [[^-D_UNICODE$]])
#set_property(TARGET modplug PROPERTY COMPILE_DEFINITIONS ${defs})

#target_compile_definitions(modplug PRIVATE -UUNICODE)
#target_compile_definitions(modplug PRIVATE -U_UNICODE)


add_library(libsoundwave STATIC
        include/libsoundwave/AudioDecoder.h
        include/libsoundwave/Common.h
        include/libsoundwave/Dither.h
        include/libsoundwave/FlacDecoder.h
        include/libsoundwave/IMA4Util.h
        include/libsoundwave/ModplugDecoder.h
        include/libsoundwave/MusepackDecoder.h
        include/libsoundwave/OpusDecoder.h
        include/libsoundwave/PostProcess.h
        include/libsoundwave/RiffUtils.h
        include/libsoundwave/VorbisDecoder.h
        include/libsoundwave/WavDecoder.h
        include/libsoundwave/WavEncoder.h
        include/libsoundwave/WavPackDecoder.h
        src/AudioDecoder.cpp
        src/Common.cpp
        src/FlacDecoder.cpp
#        src/FlacDependencies.c
        src/ModplugDecoder.cpp
#        src/ModplugDependencies.cpp
        src/MusepackDecoder.cpp
#        src/MusepackDependencies.c
        src/OpusDecoder.cpp
#        src/OpusDependencies.c
        src/RiffUtils.cpp
        src/VorbisDecoder.cpp
#        src/VorbisDependencies.c
        src/WavDecoder.cpp
        src/WavEncoder.cpp
        src/WavPackDecoder.cpp)

target_include_directories(libsoundwave PUBLIC include/libsoundwave)

target_include_directories(libsoundwave PRIVATE third_party)
target_include_directories(libsoundwave PRIVATE third_party/musepack_src_r475)
target_include_directories(libsoundwave PRIVATE third_party/musepack_src_r475/include)
target_include_directories(libsoundwave PRIVATE third_party/ogg/include)
target_include_directories(libsoundwave PRIVATE third_party/flac/include)
target_include_directories(libsoundwave PRIVATE third_party/opus/include)
target_include_directories(libsoundwave PRIVATE third_party/opusfile/include)
target_include_directories(libsoundwave PRIVATE third_party/vorbis/include)
target_include_directories(libsoundwave PRIVATE third_party/WavPack/include)

add_dependencies(libsoundwave vorbis ogg opus opusfile FLAC modplug)


add_executable(soundwave_example
    examples/src/AudioDevice.cpp
    examples/src/AudioDevice.h
    examples/src/Main.cpp
    #examples/src/sine.cpp
    examples/src/RingBuffer.h
)
target_include_directories(soundwave_example PRIVATE third_party/rtaudio)
target_include_directories(soundwave_example PRIVATE include)

add_dependencies(soundwave_example libsoundwave rtaudio)
target_link_libraries(soundwave_example libsoundwave rtaudio FLAC ogg opus opusfile vorbis vorbisenc vorbisfile wavpack mpcdec_static modplug)


#
#include_directories(examples/src)
#include_directories(include)
#include_directories(include/libsoundwave)
#include_directories(third_party/catch)
#include_directories(third_party/FLAC)
#include_directories(third_party/FLAC/src/ia32)
#include_directories(third_party/FLAC/src/include)
#include_directories(third_party/FLAC/src/include/private)
#include_directories(third_party/FLAC/src/include/protected)
#include_directories(third_party/FLAC/src/include/share)
#include_directories(third_party/libmodplug/src)
#include_directories(third_party/libogg/include)
#include_directories(third_party/libogg/include/ogg)
#include_directories(third_party/libvorbis/include)
#include_directories(third_party/libvorbis/include/vorbis)
#include_directories(third_party/libvorbis/src)
#include_directories(third_party/libvorbis/src/books)
#include_directories(third_party/libvorbis/src/books/coupled)
#include_directories(third_party/libvorbis/src/books/floor)
#include_directories(third_party/libvorbis/src/books/uncoupled)
#include_directories(third_party/libvorbis/src/modes)
#include_directories(third_party/musepack/include)
#include_directories(third_party/musepack/include/mpc)
#include_directories(third_party/musepack/libmpcdec)
#include_directories(third_party/musepack/libmpcenc)
#include_directories(third_party/opus/celt)
#include_directories(third_party/opus/celt/arm)
#include_directories(third_party/opus/celt/x86)
#include_directories(third_party/opus/libopus/include)
#include_directories(third_party/opus/libopus/src)
#include_directories(third_party/opus/opusfile/include)
#include_directories(third_party/opus/opusfile/src/include)
#include_directories(third_party/opus/silk)
#include_directories(third_party/opus/silk/arm)
#include_directories(third_party/opus/silk/fixed)
#include_directories(third_party/opus/silk/float)
#include_directories(third_party/rtaudio)
#include_directories(third_party/wavpack/include)
#include_directories(third_party/wavpack/src)
#
#add_executable(libsoundwave
#        examples/src/AudioDevice.cpp
#        examples/src/AudioDevice.h
#        examples/src/Main.cpp
#        examples/src/RingBuffer.h
#        include/libsoundwave/AudioDecoder.h
#        include/libsoundwave/Common.h
#        include/libsoundwave/Dither.h
#        include/libsoundwave/FlacDecoder.h
#        include/libsoundwave/IMA4Util.h
#        include/libsoundwave/ModplugDecoder.h
#        include/libsoundwave/MusepackDecoder.h
#        include/libsoundwave/OpusDecoder.h
#        include/libsoundwave/PostProcess.h
#        include/libsoundwave/RiffUtils.h
#        include/libsoundwave/VorbisDecoder.h
#        include/libsoundwave/WavDecoder.h
#        include/libsoundwave/WavEncoder.h
#        include/libsoundwave/WavPackDecoder.h
#        src/AudioDecoder.cpp
#        src/Common.cpp
#        src/FlacDecoder.cpp
#        src/FlacDependencies.c
#        src/ModplugDecoder.cpp
#        src/ModplugDependencies.cpp
#        src/MusepackDecoder.cpp
#        src/MusepackDependencies.c
#        src/OpusDecoder.cpp
#        src/OpusDependencies.c
#        src/RiffUtils.cpp
#        src/VorbisDecoder.cpp
#        src/VorbisDependencies.c
#        src/WavDecoder.cpp
#        src/WavEncoder.cpp
#        src/WavPackDecoder.cpp
#        third_party/catch/catch.hpp
#        third_party/FLAC/src/ia32/nasm.h
#        third_party/FLAC/src/include/private/all.h
#        third_party/FLAC/src/include/private/bitmath.h
#        third_party/FLAC/src/include/private/bitreader.h
#        third_party/FLAC/src/include/private/bitwriter.h
#        third_party/FLAC/src/include/private/cpu.h
#        third_party/FLAC/src/include/private/crc.h
#        third_party/FLAC/src/include/private/fixed.h
#        third_party/FLAC/src/include/private/_float.h
#        third_party/FLAC/src/include/private/format.h
#        third_party/FLAC/src/include/private/lpc.h
#        third_party/FLAC/src/include/private/macros.h
#        third_party/FLAC/src/include/private/md5.h
#        third_party/FLAC/src/include/private/memory.h
#        third_party/FLAC/src/include/private/metadata.h
#        third_party/FLAC/src/include/private/ogg_decoder_aspect.h
#        third_party/FLAC/src/include/private/ogg_encoder_aspect.h
#        third_party/FLAC/src/include/private/ogg_helper.h
#        third_party/FLAC/src/include/private/ogg_mapping.h
#        third_party/FLAC/src/include/private/stream_encoder.h
#        third_party/FLAC/src/include/private/stream_encoder_framing.h
#        third_party/FLAC/src/include/private/window.h
#        third_party/FLAC/src/include/protected/all.h
#        third_party/FLAC/src/include/protected/stream_decoder.h
#        third_party/FLAC/src/include/protected/stream_encoder.h
#        third_party/FLAC/src/include/share/alloc.h
#        third_party/FLAC/src/include/share/compat.h
#        third_party/FLAC/src/include/share/endswap.h
#        third_party/FLAC/src/include/share/getopt.h
#        third_party/FLAC/src/include/share/macros.h
#        third_party/FLAC/src/include/share/private.h
#        third_party/FLAC/src/include/share/safe_str.h
#        third_party/FLAC/src/include/share/utf8.h
#        third_party/FLAC/src/include/share/win_utf8_io.h
#        third_party/FLAC/src/bitmath.c
#        third_party/FLAC/src/bitreader.c
#        third_party/FLAC/src/bitwriter.c
#        third_party/FLAC/src/cpu.c
#        third_party/FLAC/src/crc.c
#        third_party/FLAC/src/fixed.c
#        third_party/FLAC/src/fixed_intrin_sse2.c
#        third_party/FLAC/src/fixed_intrin_ssse3.c
#        third_party/FLAC/src/float.c
#        third_party/FLAC/src/format.c
#        third_party/FLAC/src/lpc.c
#        third_party/FLAC/src/lpc_intrin_avx2.c
#        third_party/FLAC/src/lpc_intrin_sse.c
#        third_party/FLAC/src/lpc_intrin_sse2.c
#        third_party/FLAC/src/lpc_intrin_sse41.c
#        third_party/FLAC/src/md5.c
#        third_party/FLAC/src/memory.c
#        third_party/FLAC/src/metadata_iterators.c
#        third_party/FLAC/src/metadata_object.c
#        third_party/FLAC/src/ogg_decoder_aspect.c
#        third_party/FLAC/src/ogg_encoder_aspect.c
#        third_party/FLAC/src/ogg_helper.c
#        third_party/FLAC/src/ogg_mapping.c
#        third_party/FLAC/src/stream_decoder.c
#        third_party/FLAC/src/stream_encoder.c
#        third_party/FLAC/src/stream_encoder_framing.c
#        third_party/FLAC/src/stream_encoder_intrin_avx2.c
#        third_party/FLAC/src/stream_encoder_intrin_sse2.c
#        third_party/FLAC/src/stream_encoder_intrin_ssse3.c
#        third_party/FLAC/src/win_utf8_io.c
#        third_party/FLAC/src/window.c
#        third_party/FLAC/all.h
#        third_party/FLAC/_assert.h
#        third_party/FLAC/callback.h
#        third_party/FLAC/export.h
#        third_party/FLAC/format.h
#        third_party/FLAC/metadata.h
#        third_party/FLAC/ordinals.h
#        third_party/FLAC/stream_decoder.h
#        third_party/FLAC/stream_encoder.h
#        third_party/libmodplug/src/fastmix.cpp
#        third_party/libmodplug/src/it_defs.h
#        third_party/libmodplug/src/load_669.cpp
#        third_party/libmodplug/src/load_abc.cpp
#        third_party/libmodplug/src/load_amf.cpp
#        third_party/libmodplug/src/load_ams.cpp
#        third_party/libmodplug/src/load_dbm.cpp
#        third_party/libmodplug/src/load_dmf.cpp
#        third_party/libmodplug/src/load_dsm.cpp
#        third_party/libmodplug/src/load_far.cpp
#        third_party/libmodplug/src/load_it.cpp
#        third_party/libmodplug/src/load_j2b.cpp
#        third_party/libmodplug/src/load_mdl.cpp
#        third_party/libmodplug/src/load_med.cpp
#        third_party/libmodplug/src/load_mid.cpp
#        third_party/libmodplug/src/load_mod.cpp
#        third_party/libmodplug/src/load_mt2.cpp
#        third_party/libmodplug/src/load_mtm.cpp
#        third_party/libmodplug/src/load_okt.cpp
#        third_party/libmodplug/src/load_pat.cpp
#        third_party/libmodplug/src/load_pat.h
#        third_party/libmodplug/src/load_psm.cpp
#        third_party/libmodplug/src/load_ptm.cpp
#        third_party/libmodplug/src/load_s3m.cpp
#        third_party/libmodplug/src/load_stm.cpp
#        third_party/libmodplug/src/load_ult.cpp
#        third_party/libmodplug/src/load_umx.cpp
#        third_party/libmodplug/src/load_wav.cpp
#        third_party/libmodplug/src/load_xm.cpp
#        third_party/libmodplug/src/mmcmp.cpp
#        third_party/libmodplug/src/modplug.cpp
#        third_party/libmodplug/src/modplug.h
#        third_party/libmodplug/src/snd_dsp.cpp
#        third_party/libmodplug/src/snd_flt.cpp
#        third_party/libmodplug/src/snd_fx.cpp
#        third_party/libmodplug/src/sndfile.cpp
#        third_party/libmodplug/src/sndfile.h
#        third_party/libmodplug/src/sndmix.cpp
#        third_party/libmodplug/src/stdafx.h
#        third_party/libmodplug/src/tables.h
#        third_party/libogg/include/ogg/ogg.h
#        third_party/libogg/include/ogg/os_types.h
#        third_party/libogg/src/bitwise.c
#        third_party/libogg/src/framing.c
#        third_party/libvorbis/include/vorbis/codec.h
#        third_party/libvorbis/include/vorbis/vorbisenc.h
#        third_party/libvorbis/include/vorbis/vorbisfile.h
#        third_party/libvorbis/src/books/coupled/res_books_51.h
#        third_party/libvorbis/src/books/coupled/res_books_stereo.h
#        third_party/libvorbis/src/books/floor/floor_books.h
#        third_party/libvorbis/src/books/uncoupled/res_books_uncoupled.h
#        third_party/libvorbis/src/modes/floor_all.h
#        third_party/libvorbis/src/modes/psych_11.h
#        third_party/libvorbis/src/modes/psych_16.h
#        third_party/libvorbis/src/modes/psych_44.h
#        third_party/libvorbis/src/modes/psych_8.h
#        third_party/libvorbis/src/modes/residue_16.h
#        third_party/libvorbis/src/modes/residue_44.h
#        third_party/libvorbis/src/modes/residue_44p51.h
#        third_party/libvorbis/src/modes/residue_44u.h
#        third_party/libvorbis/src/modes/residue_8.h
#        third_party/libvorbis/src/modes/setup_11.h
#        third_party/libvorbis/src/modes/setup_16.h
#        third_party/libvorbis/src/modes/setup_22.h
#        third_party/libvorbis/src/modes/setup_32.h
#        third_party/libvorbis/src/modes/setup_44.h
#        third_party/libvorbis/src/modes/setup_44p51.h
#        third_party/libvorbis/src/modes/setup_44u.h
#        third_party/libvorbis/src/modes/setup_8.h
#        third_party/libvorbis/src/modes/setup_X.h
#        third_party/libvorbis/src/analysis.c
#        third_party/libvorbis/src/backends.h
#        third_party/libvorbis/src/barkmel.c
#        third_party/libvorbis/src/bitrate.c
#        third_party/libvorbis/src/bitrate.h
#        third_party/libvorbis/src/block.c
#        third_party/libvorbis/src/codebook.c
#        third_party/libvorbis/src/codebook.h
#        third_party/libvorbis/src/codec_internal.h
#        third_party/libvorbis/src/envelope.c
#        third_party/libvorbis/src/envelope.h
#        third_party/libvorbis/src/floor0.c
#        third_party/libvorbis/src/floor1.c
#        third_party/libvorbis/src/highlevel.h
#        third_party/libvorbis/src/info.c
#        third_party/libvorbis/src/lookup.c
#        third_party/libvorbis/src/lookup.h
#        third_party/libvorbis/src/lookup_data.h
#        third_party/libvorbis/src/lpc.c
#        third_party/libvorbis/src/lpc.h
#        third_party/libvorbis/src/lsp.c
#        third_party/libvorbis/src/lsp.h
#        third_party/libvorbis/src/mapping0.c
#        third_party/libvorbis/src/masking.h
#        third_party/libvorbis/src/mdct.c
#        third_party/libvorbis/src/mdct.h
#        third_party/libvorbis/src/misc.h
#        third_party/libvorbis/src/os.h
#        third_party/libvorbis/src/psy.c
#        third_party/libvorbis/src/psy.h
#        third_party/libvorbis/src/psytune.c
#        third_party/libvorbis/src/registry.c
#        third_party/libvorbis/src/registry.h
#        third_party/libvorbis/src/res0.c
#        third_party/libvorbis/src/scales.h
#        third_party/libvorbis/src/sharedbook.c
#        third_party/libvorbis/src/smallft.c
#        third_party/libvorbis/src/smallft.h
#        third_party/libvorbis/src/synthesis.c
#        third_party/libvorbis/src/tone.c
#        third_party/libvorbis/src/vorbisenc.c
#        third_party/libvorbis/src/vorbisfile.c
#        third_party/libvorbis/src/window.c
#        third_party/libvorbis/src/window.h
#        third_party/musepack/include/mpc/datatypes.h
#        third_party/musepack/include/mpc/minimax.h
#        third_party/musepack/include/mpc/mpc_types.h
#        third_party/musepack/include/mpc/mpcdec.h
#        third_party/musepack/include/mpc/mpcmath.h
#        third_party/musepack/include/mpc/reader.h
#        third_party/musepack/include/mpc/streaminfo.h
#        third_party/musepack/libmpcdec/crc32.c
#        third_party/musepack/libmpcdec/decoder.h
#        third_party/musepack/libmpcdec/huffman.c
#        third_party/musepack/libmpcdec/huffman.h
#        third_party/musepack/libmpcdec/internal.h
#        third_party/musepack/libmpcdec/mpc_bits_reader.c
#        third_party/musepack/libmpcdec/mpc_bits_reader.h
#        third_party/musepack/libmpcdec/mpc_decoder.c
#        third_party/musepack/libmpcdec/mpc_demux.c
#        third_party/musepack/libmpcdec/mpc_reader.c
#        third_party/musepack/libmpcdec/mpcdec_math.h
#        third_party/musepack/libmpcdec/requant.c
#        third_party/musepack/libmpcdec/requant.h
#        third_party/musepack/libmpcdec/streaminfo.c
#        third_party/musepack/libmpcdec/synth_filter.c
#        third_party/musepack/libmpcenc/analy_filter.c
#        third_party/musepack/libmpcenc/bitstream.c
#        third_party/musepack/libmpcenc/encode_sv7.c
#        third_party/musepack/libmpcenc/huffsv7.c
#        third_party/musepack/libmpcenc/libmpcenc.h
#        third_party/musepack/libmpcenc/quant.c
#        third_party/opus/celt/arm/arm_celt_map.c
#        third_party/opus/celt/arm/armcpu.c
#        third_party/opus/celt/arm/armcpu.h
#        third_party/opus/celt/arm/fixed_armv4.h
#        third_party/opus/celt/arm/fixed_armv5e.h
#        third_party/opus/celt/arm/kiss_fft_armv4.h
#        third_party/opus/celt/arm/kiss_fft_armv5e.h
#        third_party/opus/celt/arm/pitch_arm.h
#        third_party/opus/celt/x86/pitch_sse.h
#        third_party/opus/celt/_kiss_fft_guts.h
#        third_party/opus/celt/arch.h
#        third_party/opus/celt/bands.c
#        third_party/opus/celt/bands.h
#        third_party/opus/celt/celt.c
#        third_party/opus/celt/celt.h
#        third_party/opus/celt/celt_decoder.c
#        third_party/opus/celt/celt_encoder.c
#        third_party/opus/celt/celt_lpc.c
#        third_party/opus/celt/celt_lpc.h
#        third_party/opus/celt/cpu_support.h
#        third_party/opus/celt/cwrs.c
#        third_party/opus/celt/cwrs.h
#        third_party/opus/celt/ecintrin.h
#        third_party/opus/celt/entcode.c
#        third_party/opus/celt/entcode.h
#        third_party/opus/celt/entdec.c
#        third_party/opus/celt/entdec.h
#        third_party/opus/celt/entenc.c
#        third_party/opus/celt/entenc.h
#        third_party/opus/celt/fixed_debug.h
#        third_party/opus/celt/fixed_generic.h
#        third_party/opus/celt/float_cast.h
#        third_party/opus/celt/kiss_fft.c
#        third_party/opus/celt/kiss_fft.h
#        third_party/opus/celt/laplace.c
#        third_party/opus/celt/laplace.h
#        third_party/opus/celt/mathops.c
#        third_party/opus/celt/mathops.h
#        third_party/opus/celt/mdct.c
#        third_party/opus/celt/mdct.h
#        third_party/opus/celt/mfrngcod.h
#        third_party/opus/celt/modes.c
#        third_party/opus/celt/modes.h
#        third_party/opus/celt/opus_custom_demo.c
#        third_party/opus/celt/os_support.h
#        third_party/opus/celt/pitch.c
#        third_party/opus/celt/pitch.h
#        third_party/opus/celt/quant_bands.c
#        third_party/opus/celt/quant_bands.h
#        third_party/opus/celt/rate.c
#        third_party/opus/celt/rate.h
#        third_party/opus/celt/stack_alloc.h
#        third_party/opus/celt/static_modes_fixed.h
#        third_party/opus/celt/static_modes_float.h
#        third_party/opus/celt/vq.c
#        third_party/opus/celt/vq.h
#        third_party/opus/libopus/include/opus.h
#        third_party/opus/libopus/include/opus_custom.h
#        third_party/opus/libopus/include/opus_defines.h
#        third_party/opus/libopus/include/opus_multistream.h
#        third_party/opus/libopus/include/opus_types.h
#        third_party/opus/libopus/src/analysis.c
#        third_party/opus/libopus/src/analysis.h
#        third_party/opus/libopus/src/mlp.c
#        third_party/opus/libopus/src/mlp.h
#        third_party/opus/libopus/src/mlp_data.c
#        third_party/opus/libopus/src/opus.c
#        third_party/opus/libopus/src/opus_compare.c
#        third_party/opus/libopus/src/opus_decoder.c
#        third_party/opus/libopus/src/opus_demo.c
#        third_party/opus/libopus/src/opus_encoder.c
#        third_party/opus/libopus/src/opus_multistream.c
#        third_party/opus/libopus/src/opus_multistream_decoder.c
#        third_party/opus/libopus/src/opus_multistream_encoder.c
#        third_party/opus/libopus/src/opus_private.h
#        third_party/opus/libopus/src/repacketizer.c
#        third_party/opus/libopus/src/repacketizer_demo.c
#        third_party/opus/libopus/src/tansig_table.h
#        third_party/opus/opusfile/include/opusfile.h
#        third_party/opus/opusfile/src/include/internal.h
#        third_party/opus/opusfile/src/include/winerrno.h
#        third_party/opus/opusfile/src/http.c
#        third_party/opus/opusfile/src/info.c
#        third_party/opus/opusfile/src/internal.c
#        third_party/opus/opusfile/src/opusfile.c
#        third_party/opus/opusfile/src/stream.c
#        third_party/opus/opusfile/src/wincerts.c
#        third_party/opus/silk/arm/macros_armv4.h
#        third_party/opus/silk/arm/macros_armv5e.h
#        third_party/opus/silk/arm/SigProc_FIX_armv4.h
#        third_party/opus/silk/arm/SigProc_FIX_armv5e.h
#        third_party/opus/silk/fixed/apply_sine_window_FIX.c
#        third_party/opus/silk/fixed/autocorr_FIX.c
#        third_party/opus/silk/fixed/burg_modified_FIX.c
#        third_party/opus/silk/fixed/corrMatrix_FIX.c
#        third_party/opus/silk/fixed/encode_frame_FIX.c
#        third_party/opus/silk/fixed/find_LPC_FIX.c
#        third_party/opus/silk/fixed/find_LTP_FIX.c
#        third_party/opus/silk/fixed/find_pitch_lags_FIX.c
#        third_party/opus/silk/fixed/find_pred_coefs_FIX.c
#        third_party/opus/silk/fixed/k2a_FIX.c
#        third_party/opus/silk/fixed/k2a_Q16_FIX.c
#        third_party/opus/silk/fixed/LTP_analysis_filter_FIX.c
#        third_party/opus/silk/fixed/LTP_scale_ctrl_FIX.c
#        third_party/opus/silk/fixed/main_FIX.h
#        third_party/opus/silk/fixed/noise_shape_analysis_FIX.c
#        third_party/opus/silk/fixed/pitch_analysis_core_FIX.c
#        third_party/opus/silk/fixed/prefilter_FIX.c
#        third_party/opus/silk/fixed/process_gains_FIX.c
#        third_party/opus/silk/fixed/regularize_correlations_FIX.c
#        third_party/opus/silk/fixed/residual_energy16_FIX.c
#        third_party/opus/silk/fixed/residual_energy_FIX.c
#        third_party/opus/silk/fixed/schur64_FIX.c
#        third_party/opus/silk/fixed/schur_FIX.c
#        third_party/opus/silk/fixed/solve_LS_FIX.c
#        third_party/opus/silk/fixed/structs_FIX.h
#        third_party/opus/silk/fixed/vector_ops_FIX.c
#        third_party/opus/silk/fixed/warped_autocorrelation_FIX.c
#        third_party/opus/silk/float/apply_sine_window_FLP.c
#        third_party/opus/silk/float/autocorrelation_FLP.c
#        third_party/opus/silk/float/burg_modified_FLP.c
#        third_party/opus/silk/float/bwexpander_FLP.c
#        third_party/opus/silk/float/corrMatrix_FLP.c
#        third_party/opus/silk/float/encode_frame_FLP.c
#        third_party/opus/silk/float/energy_FLP.c
#        third_party/opus/silk/float/find_LPC_FLP.c
#        third_party/opus/silk/float/find_LTP_FLP.c
#        third_party/opus/silk/float/find_pitch_lags_FLP.c
#        third_party/opus/silk/float/find_pred_coefs_FLP.c
#        third_party/opus/silk/float/inner_product_FLP.c
#        third_party/opus/silk/float/k2a_FLP.c
#        third_party/opus/silk/float/levinsondurbin_FLP.c
#        third_party/opus/silk/float/LPC_analysis_filter_FLP.c
#        third_party/opus/silk/float/LPC_inv_pred_gain_FLP.c
#        third_party/opus/silk/float/LTP_analysis_filter_FLP.c
#        third_party/opus/silk/float/LTP_scale_ctrl_FLP.c
#        third_party/opus/silk/float/main_FLP.h
#        third_party/opus/silk/float/noise_shape_analysis_FLP.c
#        third_party/opus/silk/float/pitch_analysis_core_FLP.c
#        third_party/opus/silk/float/prefilter_FLP.c
#        third_party/opus/silk/float/process_gains_FLP.c
#        third_party/opus/silk/float/regularize_correlations_FLP.c
#        third_party/opus/silk/float/residual_energy_FLP.c
#        third_party/opus/silk/float/scale_copy_vector_FLP.c
#        third_party/opus/silk/float/scale_vector_FLP.c
#        third_party/opus/silk/float/schur_FLP.c
#        third_party/opus/silk/float/SigProc_FLP.h
#        third_party/opus/silk/float/solve_LS_FLP.c
#        third_party/opus/silk/float/sort_FLP.c
#        third_party/opus/silk/float/structs_FLP.h
#        third_party/opus/silk/float/warped_autocorrelation_FLP.c
#        third_party/opus/silk/float/wrappers_FLP.c
#        third_party/opus/silk/A2NLSF.c
#        third_party/opus/silk/ana_filt_bank_1.c
#        third_party/opus/silk/API.h
#        third_party/opus/silk/biquad_alt.c
#        third_party/opus/silk/bwexpander.c
#        third_party/opus/silk/bwexpander_32.c
#        third_party/opus/silk/check_control_input.c
#        third_party/opus/silk/CNG.c
#        third_party/opus/silk/code_signs.c
#        third_party/opus/silk/control.h
#        third_party/opus/silk/control_audio_bandwidth.c
#        third_party/opus/silk/control_codec.c
#        third_party/opus/silk/control_SNR.c
#        third_party/opus/silk/debug.c
#        third_party/opus/silk/debug.h
#        third_party/opus/silk/dec_API.c
#        third_party/opus/silk/decode_core.c
#        third_party/opus/silk/decode_frame.c
#        third_party/opus/silk/decode_indices.c
#        third_party/opus/silk/decode_parameters.c
#        third_party/opus/silk/decode_pitch.c
#        third_party/opus/silk/decode_pulses.c
#        third_party/opus/silk/decoder_set_fs.c
#        third_party/opus/silk/define.h
#        third_party/opus/silk/enc_API.c
#        third_party/opus/silk/encode_indices.c
#        third_party/opus/silk/encode_pulses.c
#        third_party/opus/silk/errors.h
#        third_party/opus/silk/gain_quant.c
#        third_party/opus/silk/HP_variable_cutoff.c
#        third_party/opus/silk/init_decoder.c
#        third_party/opus/silk/init_encoder.c
#        third_party/opus/silk/Inlines.h
#        third_party/opus/silk/inner_prod_aligned.c
#        third_party/opus/silk/interpolate.c
#        third_party/opus/silk/lin2log.c
#        third_party/opus/silk/log2lin.c
#        third_party/opus/silk/LP_variable_cutoff.c
#        third_party/opus/silk/LPC_analysis_filter.c
#        third_party/opus/silk/LPC_inv_pred_gain.c
#        third_party/opus/silk/MacroCount.h
#        third_party/opus/silk/MacroDebug.h
#        third_party/opus/silk/macros.h
#        third_party/opus/silk/main.h
#        third_party/opus/silk/NLSF2A.c
#        third_party/opus/silk/NLSF_decode.c
#        third_party/opus/silk/NLSF_del_dec_quant.c
#        third_party/opus/silk/NLSF_encode.c
#        third_party/opus/silk/NLSF_stabilize.c
#        third_party/opus/silk/NLSF_unpack.c
#        third_party/opus/silk/NLSF_VQ.c
#        third_party/opus/silk/NLSF_VQ_weights_laroia.c
#        third_party/opus/silk/NSQ.c
#        third_party/opus/silk/NSQ_del_dec.c
#        third_party/opus/silk/pitch_est_defines.h
#        third_party/opus/silk/pitch_est_tables.c
#        third_party/opus/silk/PLC.c
#        third_party/opus/silk/PLC.h
#        third_party/opus/silk/process_NLSFs.c
#        third_party/opus/silk/quant_LTP_gains.c
#        third_party/opus/silk/resampler.c
#        third_party/opus/silk/resampler_down2.c
#        third_party/opus/silk/resampler_down2_3.c
#        third_party/opus/silk/resampler_private.h
#        third_party/opus/silk/resampler_private_AR2.c
#        third_party/opus/silk/resampler_private_down_FIR.c
#        third_party/opus/silk/resampler_private_IIR_FIR.c
#        third_party/opus/silk/resampler_private_up2_HQ.c
#        third_party/opus/silk/resampler_rom.c
#        third_party/opus/silk/resampler_rom.h
#        third_party/opus/silk/resampler_structs.h
#        third_party/opus/silk/shell_coder.c
#        third_party/opus/silk/sigm_Q15.c
#        third_party/opus/silk/SigProc_FIX.h
#        third_party/opus/silk/sort.c
#        third_party/opus/silk/stereo_decode_pred.c
#        third_party/opus/silk/stereo_encode_pred.c
#        third_party/opus/silk/stereo_find_predictor.c
#        third_party/opus/silk/stereo_LR_to_MS.c
#        third_party/opus/silk/stereo_MS_to_LR.c
#        third_party/opus/silk/stereo_quant_pred.c
#        third_party/opus/silk/structs.h
#        third_party/opus/silk/sum_sqr_shift.c
#        third_party/opus/silk/table_LSF_cos.c
#        third_party/opus/silk/tables.h
#        third_party/opus/silk/tables_gain.c
#        third_party/opus/silk/tables_LTP.c
#        third_party/opus/silk/tables_NLSF_CB_NB_MB.c
#        third_party/opus/silk/tables_NLSF_CB_WB.c
#        third_party/opus/silk/tables_other.c
#        third_party/opus/silk/tables_pitch_lag.c
#        third_party/opus/silk/tables_pulses_per_block.c
#        third_party/opus/silk/tuning_parameters.h
#        third_party/opus/silk/typedef.h
#        third_party/opus/silk/VAD.c
#        third_party/opus/silk/VQ_WMat_EC.c
#        third_party/rtaudio/RtAudio.cpp
#        third_party/rtaudio/RtAudio.h
#        third_party/wavpack/include/wavpack.h
#        third_party/wavpack/src/common_utils.c
#        third_party/wavpack/src/decorr_tables.h
#        third_party/wavpack/src/decorr_utils.c
#        third_party/wavpack/src/entropy_utils.c
#        third_party/wavpack/src/extra1.c
#        third_party/wavpack/src/extra2.c
#        third_party/wavpack/src/open_filename.c
#        third_party/wavpack/src/open_legacy.c
#        third_party/wavpack/src/open_raw.c
#        third_party/wavpack/src/open_utils.c
#        third_party/wavpack/src/pack.c
#        third_party/wavpack/src/pack_dns.c
#        third_party/wavpack/src/pack_dsd.c
#        third_party/wavpack/src/pack_floats.c
#        third_party/wavpack/src/pack_utils.c
#        third_party/wavpack/src/read_words.c
#        third_party/wavpack/src/tag_utils.c
#        third_party/wavpack/src/tags.c
#        third_party/wavpack/src/unpack.c
#        third_party/wavpack/src/unpack3.c
#        third_party/wavpack/src/unpack3.h
#        third_party/wavpack/src/unpack3_open.c
#        third_party/wavpack/src/unpack3_seek.c
#        third_party/wavpack/src/unpack_dsd.c
#        third_party/wavpack/src/unpack_floats.c
#        third_party/wavpack/src/unpack_seek.c
#        third_party/wavpack/src/unpack_utils.c
#        third_party/wavpack/src/wavpack_local.h
#        third_party/wavpack/src/wavpack_version.h
#        third_party/wavpack/src/write_words.c)
#
#target_include_directories(libsoundwave PUBLIC third_party)
