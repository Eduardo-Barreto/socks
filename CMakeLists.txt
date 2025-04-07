cmake_minimum_required(VERSION 3.10)
project(RawSocketTransfer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add common library
add_library(common_lib
    src/common/socket_utils.cpp
    src/common/file_utils.cpp
)

# Add sender executable
add_executable(sender
    src/sender/main.cpp
    src/sender/sender.cpp
)

# Add receiver executable
add_executable(receiver
    src/receiver/main.cpp
    src/receiver/receiver.cpp
)

file(GLOB_RECURSE FORMAT_SOURCES CONFIGURE_DEPENDS "src/*.c*") 
file(GLOB_RECURSE FORMAT_HEADERS CONFIGURE_DEPENDS "include/*.h*")
file(GLOB_RECURSE FORMAT_SCRIPTS CONFIGURE_DEPENDS "scripts/*.sh")


function(generate_format_target)
    foreach(FILE ${ARGV})
        list(APPEND FILES_LIST ${${FILE}})
    endforeach()

    add_custom_target(format
        COMMAND clang-format -style=file -i ${FILES_LIST} --verbose
        COMMAND shfmt -w -i 4 -ci ${FORMAT_SCRIPTS}
    )
endfunction()

generate_format_target(FORMAT_SOURCES FORMAT_HEADERS)

# Link common library to both executables
target_link_libraries(sender common_lib)
target_link_libraries(receiver common_lib)

# Include directories
target_include_directories(common_lib PUBLIC include)
target_include_directories(sender PUBLIC include)
target_include_directories(receiver PUBLIC include) 
