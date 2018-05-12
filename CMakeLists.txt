set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_minimum_required(VERSION 3.0.0)

set(TARGET_NAME ar_board_games)
add_executable(${TARGET_NAME}  MACOSX_BUNDLE src/main.cpp)

find_package( CURL REQUIRED )
include_directories( ${CURL_INCLUDE_DIRS} )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_subdirectory(src/ys_library)
add_subdirectory(src/games)
add_subdirectory(src/track_draw)
add_subdirectory(src/utility)

target_link_libraries(${TARGET_NAME}  
    ${OpenCV_LIBS} 
    ${CURL_LIBRARIES}
    fslazywindow
    ysclass
    geblkernel
    games_lib
    trackdraw_lib
    util_lib
)

set(DATA_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/resources)

get_property(IS_MACOSX_BUNDLE TARGET ${TARGET_NAME} PROPERTY MACOSX_BUNDLE)
message(${IS_MACOSX_BUNDLE})
if(APPLE AND ${IS_MACOSX_BUNDLE})
    set(DATA_DESTINATION "$<TARGET_FILE_DIR:${TARGET_NAME}>/../Resources")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "WindowsStore")
    # YS_IS_UNIVERSAL_WINDOWS_APP is needed for ysgl to distinguish Win32 and UWP in VC
    add_definitions(-DYS_IS_UNIVERSAL_WINDOWS_APP)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /ZW")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /ZW")
    set(DATA_DESTINATION "$<TARGET_FILE_DIR:${TARGET_NAME}>/Assets")
elseif(MSVC)
    set(DATA_DESTINATION "$<TARGET_FILE_DIR:${TARGET_NAME}>")
else()
    set(DATA_DESTINATION "$<TARGET_FILE_DIR:${TARGET_NAME}>")
endif()

foreach(ONE_DATA_SOURCE ${DATA_SOURCE})
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD 
        COMMAND echo \"${ONE_DATA_SOURCE}\" \"${DATA_DESTINATION}\"
        COMMAND "${CMAKE_COMMAND}" -E make_directory \"${DATA_DESTINATION}\"
        COMMAND "${CMAKE_COMMAND}" -E copy_directory \"${ONE_DATA_SOURCE}\" \"${DATA_DESTINATION}\")
endforeach(ONE_DATA_SOURCE)
