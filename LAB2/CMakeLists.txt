add_library(seq_lib 
    "bit_sequence.cpp"
)

target_include_directories(seq_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(main_app main.cpp)

target_link_libraries(main_app PRIVATE seq_lib)