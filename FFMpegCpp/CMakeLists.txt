cmake_minimum_required(VERSION 2.6)
project(codec)

set(CMAKE_CXX_STANDARD 14)

option(TARGET_ANDROID "Target Android" OFF)

add_library( AVCODEC_LIBRARY SHARED IMPORTED )
add_library( AVFORMAT_LIBRARY SHARED  IMPORTED )
add_library( AVUTIL_LIBRARY  SHARED  IMPORTED )
add_library( AVSWS_LIBRARY  SHARED  IMPORTED )

# Link to the ffmpeg libs depending on the target platform
if (TARGET_ANDROID)
  message(STATUS "Targeting Android OS...")
  # Note: This variable is also needed by the Android toolchain.
  # Do NOT change the name of this variable
  if (NOT DEFINED ANDROID_ABI)
    message( FATAL_ERROR "Must specify ANDROID_ABI in order to target android")
  else()
    message(STATUS "Linking with ffmpeg so files for Android ABI ${ANDROID_ABI}")
    set_target_properties( AVCODEC_LIBRARY PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/android_libs/${ANDROID_ABI}/libavcodec.so )
    set_target_properties( AVFORMAT_LIBRARY PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/android_libs/${ANDROID_ABI}/libavformat.so )
    set_target_properties( AVUTIL_LIBRARY PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/android_libs/${ANDROID_ABI}/libavutil.so )
    set_target_properties( AVSWS_LIBRARY PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/android_libs/${ANDROID_ABI}/libswscale.so )
  endif()

elseif (UNIX)
  message( FATAL_ERROR "Unix based operating systems not currently suppported. Cmake will exit...")

elseif(WIN32 OR WIN64)
  message(STATUS "Targeting Windows OS...")

else()
  message( FATAL_ERROR "Could not detect target platform, Cmake will exit...")

endif()

#################################################################################
# RTP Server
#################################################################################
add_library(
    rtpserver
    SHARED
    ${PROJECT_SOURCE_DIR}/src/encoder_context.cpp
    ${PROJECT_SOURCE_DIR}/src/encoder_frame.cpp
    ${PROJECT_SOURCE_DIR}/src/frame_encoder.cpp
    ${PROJECT_SOURCE_DIR}/src/packet.cpp
    ${PROJECT_SOURCE_DIR}/src/packet_ref.cpp
    ${PROJECT_SOURCE_DIR}/src/pixel_converter.cpp
    ${PROJECT_SOURCE_DIR}/src/rtp_server.cpp
    ${PROJECT_SOURCE_DIR}/src/rtp_server_api.cpp
    ${PROJECT_SOURCE_DIR}/src/rtp_stream.cpp
    ${PROJECT_SOURCE_DIR}/src/stream_format_context.cpp
    ${PROJECT_SOURCE_DIR}/src/sdp_file.cpp
)

target_include_directories(
    rtpserver
    PUBLIC
    include
    ffmpeg_include
)


if (TARGET_ANDROID)
  target_link_libraries(
      rtpserver
      PRIVATE
      AVCODEC_LIBRARY
      AVFORMAT_LIBRARY
      AVUTIL_LIBRARY
      AVSWS_LIBRARY
  )

else()
  target_link_libraries(
      rtpserver
      PUBLIC
      ${PROJECT_SOURCE_DIR}/windows_libs/avformat.lib
      ${PROJECT_SOURCE_DIR}/windows_libs/avcodec.lib
      ${PROJECT_SOURCE_DIR}/windows_libs/avutil.lib
      ${PROJECT_SOURCE_DIR}/windows_libs/swscale.lib
  )
endif()

################################################################################
# RTP STREAMING CLIENT
################################################################################
add_library(
    rtpclient
    SHARED
    ${PROJECT_SOURCE_DIR}/src/circular_frame_buffer.cpp
    ${PROJECT_SOURCE_DIR}/src/client_format_context.cpp
    ${PROJECT_SOURCE_DIR}/src/decoded_frame_buffer.cpp
    ${PROJECT_SOURCE_DIR}/src/decoder_context.cpp
    ${PROJECT_SOURCE_DIR}/src/decoder_frame.cpp
    ${PROJECT_SOURCE_DIR}/src/frame_decoder.cpp
    ${PROJECT_SOURCE_DIR}/src/packet.cpp
    ${PROJECT_SOURCE_DIR}/src/pixel_converter.cpp
    ${PROJECT_SOURCE_DIR}/src/rtp_client.cpp
    ${PROJECT_SOURCE_DIR}/src/rtp_client_api.cpp
    ${PROJECT_SOURCE_DIR}/src/sdp_file.cpp
    ${PROJECT_SOURCE_DIR}/src/stream_spinner.cpp
)

target_include_directories(
    rtpclient
    PUBLIC
    include
    ffmpeg_include
)

if (TARGET_ANDROID)
  target_link_libraries(
        rtpclient
        PRIVATE
        AVCODEC_LIBRARY
        AVFORMAT_LIBRARY
        AVUTIL_LIBRARY
        AVSWS_LIBRARY
  )
else()
  target_link_libraries(
      rtpclient
      PRIVATE
      ${PROJECT_SOURCE_DIR}/windows_libs/avformat.lib
      ${PROJECT_SOURCE_DIR}/windows_libs/avcodec.lib
      ${PROJECT_SOURCE_DIR}/windows_libs/avutil.lib
      ${PROJECT_SOURCE_DIR}/windows_libs/swscale.lib
  )
endif()
