cmake_minimum_required(VERSION 3.16)
project(TechStore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Libreria ---
add_library(techstore STATIC
    src/utilidades.cpp
    src/fecha.cpp
    src/repositorio.cpp
    src/datos_iniciales.cpp
)
target_include_directories(techstore PUBLIC include)

# --- Aplicacion de consola ---
add_executable(techstore_consola app/main.cpp)
target_link_libraries(techstore_consola PRIVATE techstore)