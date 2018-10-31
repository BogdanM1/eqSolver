# autor: Bogdan
cmake_minimum_required (VERSION 2.6)
LIST(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
message( STATUS "UNIX:  " ${UNIX} )
message( STATUS "HOME:  " $ENV{HOME} )
message( STATUS "PROJECT_BINARY_DIR:  " ${PROJECT_BINARY_DIR} )
message( STATUS "PROJECT_SOURCE_DIR:  " ${PROJECT_SOURCE_DIR} )
message( STATUS "PROJECT_NAME:        " ${PROJECT_NAME} )
message( STATUS "CMAKE_CONFIGURATION_TYPES: " ${CMAKE_CONFIGURATION_TYPES} )
message( STATUS "CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH} )
message( STATUS "CMAKE_SYSTEM:  " ${CMAKE_SYSTEM} )

if (NOT DEFINED haveCUDA)
    MESSAGE(FATAL_ERROR "Usage:  cmake .. -DhaveCUDA:STRING=YES/NO ")
    return()
endif()

#Petsc
	set(PETSC_DIR /opt/software/petsc-3.9.3)
	set(PETSC_ARCH "")
# Source 
	file(GLOB_RECURSE SRC_FILES 
	"src/log/*.cpp" "src/comm/*.cpp" "src/solvers/*.cpp" 
	"src/equations/*.cpp" "src/server/*.cpp"	"src/parallel/*.cpp" "src/conf/*.*")	
if(haveCUDA STREQUAL "YES")
	find_package(CUDA REQUIRED)	
#CUSOLVER 
    set(CUDA_cusolver_LIBRARY "/usr/local/cuda-9.0/lib64/libcusolver.so")		
	file(GLOB_RECURSE CUDA_SRC_FILES  "src/solvers/*.cu")
	list(APPEND  SRC_FILES ${CUDA_SRC_FILES})
	cuda_add_executable ( servis ${SRC_FILES})
    target_link_libraries( servis ${CUDA_cusparse_LIBRARY})
    target_link_libraries( servis ${CUDA_cusolver_LIBRARY})		
    add_definitions(-DHAVE_CUDA)    	
else()
	add_executable(servis ${SRC_FILES})
endif()	
	#MPI
	find_package(MPI REQUIRED)
	include_directories(${MPI_INCLUDE_PATH})
	target_link_libraries(servis ${MPI_LIBRARIES})

	#Mumps
	find_package(MUMPS REQUIRED )
	include_directories(${MUMPS_INCLUDE_DIR})
	target_link_libraries (servis ${MUMPS_LIBRARIES})

	#Petsc
	find_package(PETSc REQUIRED) 
	include_directories(AFTER ${PETSC_INCLUDES})
	target_link_libraries(servis  ${PETSC_LIBRARIES})

	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall")



