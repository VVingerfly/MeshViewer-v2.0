CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# project ANN
PROJECT(ANN)
FILE(GLOB ANN_SRC_LIST "src/*.cpp")

SET(ANN_HEADER_LIST 
	include/ANN/ANN.h
	include/ANN/ANNperf.h
	include/ANN/ANNx.h
)

include_directories(include include/ANN)
add_library(ANN STATIC ${ANN_SRC_LIST} ${ANN_HEADER_LIST})
SET_PROPERTY(TARGET ANN PROPERTY FOLDER "libs")

install(TARGETS ANN 
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
install(FILES ${ANN_HEADER_LIST}
	DESTINATION include/ANN)
