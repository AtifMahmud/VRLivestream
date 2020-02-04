cmake_minimum_required(VERSION 2.6)
project(codec)

set(CMAKE_CXX_STANDARD 14)

# Link to the android ffmpeg shared object files
# TODO This script should take an environment var to set the include paths so
# that it can be built for windows, linux, android, etc.
add_library( AVCODEC_LIBRARY SHARED IMPORTED )
add_library( AVFORMAT_LIBRARY SHARED  IMPORTED )
add_library( AVUTIL_LIBRARY  SHARED  IMPORTED )
add_library( AVSWS_LIBRARY  SHARED  IMPORTED )
set_target_properties( AVCODEC_LIBRARY PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/android_lib/libavcodec.so )
set_target_properties( AVFORMAT_LIBRARY PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/android_lib/libavformat.so )
set_target_properties( AVUTIL_LIBRARY PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/android_lib/libavutil.so )
set_target_properties( AVSWS_LIBRARY PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/android_lib/libswscale.so )

################################################################################
# DECODING
################################################################################
add_library(
    decoder 
    SHARED 
    ${PROJECT_SOURCE_DIR}/src/decoder_api.cpp
    ${PROJECT_SOURCE_DIR}/src/decoder_context.cpp
    ${PROJECT_SOURCE_DIR}/src/decoder_frame.cpp 
    ${PROJECT_SOURCE_DIR}/src/decoded_frame_buffer.cpp 
    ${PROJECT_SOURCE_DIR}/src/frame_decoder.cpp 
    ${PROJECT_SOURCE_DIR}/src/packet.cpp 
    ${PROJECT_SOURCE_DIR}/src/pixel_converter.cpp 
)

target_include_directories(
    decoder 
    PUBLIC 
    include
    android_include
)

target_link_libraries(
    decoder 
    PUBLIC 
    AVCODEC_LIBRARY
    AVFORMAT_LIBRARY 
    AVUTIL_LIBRARY
    AVSWS_LIBRARY
)

# ################################################################################
# # ENCODING
# ################################################################################
add_library(
    encoder 
    SHARED 
    ${PROJECT_SOURCE_DIR}/src/encoder_api.cpp
    ${PROJECT_SOURCE_DIR}/src/encoder_context.cpp
    ${PROJECT_SOURCE_DIR}/src/encoder_frame.cpp 
    ${PROJECT_SOURCE_DIR}/src/frame_encoder.cpp 
    ${PROJECT_SOURCE_DIR}/src/packet.cpp 
    ${PROJECT_SOURCE_DIR}/src/pixel_converter.cpp 
)

target_include_directories(
    encoder
    PUBLIC 
    include
    android_include
)

target_link_libraries(
    encoder 
    PRIVATE 
    AVCODEC_LIBRARY
    AVFORMAT_LIBRARY 
    AVUTIL_LIBRARY
    AVSWS_LIBRARY
)
