cmake_minimum_required(VERSION 3.10)

# Proje adı ve dili
project(TransportationSystem VERSION 1.0 LANGUAGES CXX)

# Modern C++ standardını zorunlu kıl (C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Başlık (header) dosyalarının bulunacağı dizin
include_directories(include)

# Kaynak kod dosyalarını belirle
set(SOURCES
    src/main.cpp
    src/transport_system.cpp
)

# Çalıştırılabilir dosyayı oluştur
add_executable(app ${SOURCES})