add_library(FuzzySet FuzzySet.hpp FuzzySet.cpp)
add_library(FuzzyRelation FuzzyRelation.hpp FuzzyRelation.cpp)

target_include_directories(FuzzySet PUBLIC ${CMAKE_SOURCE_DIR}/lib)
target_include_directories(FuzzyRelation PUBLIC ${CMAKE_SOURCE_DIR}/lib)