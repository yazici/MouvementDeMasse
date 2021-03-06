CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

SET(MOUVEMENT_DE_MASSE_PROJECT MouvementDeMasse)
MESSAGE(STATUS "Building Mouvement de Masse")
Project(${MOUVEMENT_DE_MASSE_PROJECT} CXX)

IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ELSEIF(MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251") # dll interface
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244") # int to float
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4351") # array init new behavior
ENDIF()

SET(MOUVEMENT_DE_MASSE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
MESSAGE(STATUS "Mouvement de Masse src dir: ${MOUVEMENT_DE_MASSE_SRC_DIR}")
SET(MOUVEMENT_DE_MASSE_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})
MESSAGE(STATUS "Mouvement de Masse bin dir: ${MOUVEMENT_DE_MASSE_BIN_DIR}")

INCLUDE(LibLists.cmake)
INCLUDE(FileLists.cmake)

ADD_EXECUTABLE(${MOUVEMENT_DE_MASSE_PROJECT} ${MOUVEMENT_DE_MASSE_SRC_FILES})
TARGET_LINK_LIBRARIES(${MOUVEMENT_DE_MASSE_PROJECT} ${MOUVEMENT_DE_MASSE_LIBRARIES})
QT5_USE_MODULES(${MOUVEMENT_DE_MASSE_PROJECT} ${MOUVEMENT_DE_MASSE_QT_MODULES})
INCLUDE_DIRECTORIES(${MOUVEMENT_DE_MASSE_INCLUDE_DIRS})
