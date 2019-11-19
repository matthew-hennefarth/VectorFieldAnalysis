file(GLOB SOURCE_FILES *.cpp *.h)

include_directories(${PROJECT_SOURCE_DIR}/lib/Eigen)
add_executable(VectorFieldAnalysis ${SOURCE_FILES})