cmake_minimum_required(VERSION 3.28)


project(NetChat CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



include_directories(
    ${PROJECT_SOURCE_DIR}/server/include
    ${PROJECT_SOURCE_DIR}/client/include
    ${PROJECT_SOURCE_DIR}/messageSerialize/include

)


file(GLOB client_SRCS
    "${PROJECT_SOURCE_DIR}/client/include/*.h"
    "${PROJECT_SOURCE_DIR}/client/src/*.cpp"
)

file(GLOB server_SRCS
    "${PROJECT_SOURCE_DIR}/server/include/*.h"
    "${PROJECT_SOURCE_DIR}/server/src/*.cpp"
)

file(GLOB messageSerialize_SRCS
    "${PROJECT_SOURCE_DIR}/messageSerialize/include/*.h"
    "${PROJECT_SOURCE_DIR}/messageSerialize/src/*.cpp"
)

add_library(messageSerialize STATIC ${messageSerialize_SRCS})

add_executable(client ${client_SRCS})
target_link_libraries(client messageSerialize)

add_executable(server ${server_SRCS})
target_link_libraries(server messageSerialize)


