cmake_minimum_required(VERSION 3.20)

project(Myrenderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_library(myrenderer
    src/model.cpp
    src/rasterizer.cpp
    src/tgaimage.cpp
)


target_include_directories(myrenderer
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

function(add_renderer_example target source)
    add_executable(${target} ${source})
    target_link_libraries(${target} PRIVATE myrenderer)
endfunction()



# 主程序执行
add_executable(main app/main.cpp )
# 主程序链接 renderer 核心库
target_link_libraries(main PRIVATE myrenderer)


add_renderer_example(example_08 examples/08_specular_lighting/head_lighting.cpp)