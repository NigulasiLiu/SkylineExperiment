cmake_minimum_required(VERSION 3.10)
project(PPARS_1)

set(CMAKE_CXX_STANDARD 14)

# 手动查找GMP库和头文件
find_library(GMP_LIBRARIES gmp REQUIRED)
find_library(GMPXX_LIBRARIES gmpxx REQUIRED)
find_path(GMP_INCLUDE_DIR gmp.h REQUIRED)

if(NOT GMP_LIBRARIES OR NOT GMPXX_LIBRARIES OR NOT GMP_INCLUDE_DIR)
    message(FATAL_ERROR "Could not find GMP or GMPXX library and include directory")
else()
    message(STATUS "Found GMP libraries: ${GMP_LIBRARIES}, ${GMPXX_LIBRARIES}")
    message(STATUS "Found GMP include directory: ${GMP_INCLUDE_DIR}")
endif()

# 包含头文件路径
include_directories(${GMP_INCLUDE_DIR})

# 添加源文件
set(SOURCE_FILES
    BFilter.cpp
    BloomFilter.cpp
    Data.cpp
    DataBases.cpp
    GenQueryToken.cpp
    QToken.cpp
    RangeEncoder.cpp
    RSkylinePlain.cpp
    Server.cpp
    SHSParamters.cpp
    StrSet.cpp
    SymHomSch.cpp
    PPARSCacheMain.cpp)

# 添加可执行文件
add_executable(PPARS_1 ${SOURCE_FILES})

# 链接GMP库
target_link_libraries(PPARS_1 ${GMPXX_LIBRARIES} ${GMP_LIBRARIES})
# pthread 库
target_link_libraries(PPARS_1 pthread)
