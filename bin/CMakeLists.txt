include_directories(${CMAKE_SOURCE_DIR}/lib)

# add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} FuzzySet FuzzyRelation)