CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(TULIP_DIR "%TulipPath%")
SET(CMAKE_MODULE_PATH "%TulipPath%/share/tulip")

PROJECT(%ProjectName:l%)
SET(PLUGIN_NAME %ProjectName:l%)

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
FIND_PACKAGE(TULIP REQUIRED)

INCLUDE_DIRECTORIES(${TULIP_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

SET(PLUGIN_SRCS
 %ProjectName:l%.%CppSourceSuffix% %ProjectName:l%.%CppHeaderSuffix%
# Add source files here
)

QTX_WRAP_CPP(PLUGIN_MOC_SRCS
%ProjectName:l%.%CppHeaderSuffix%
# Add headers with Q_OBJECT here
)

ADD_LIBRARY(${PLUGIN_NAME}-${TULIP_VERSION} SHARED ${PLUGIN_SRCS} ${PLUGIN_MOC_SRCS})
TARGET_LINK_LIBRARIES(${PLUGIN_NAME}-${TULIP_VERSION} ${TULIP_LIBRARIES} ${QT_LIBRARIES})

INSTALL(TARGETS ${PLUGIN_NAME}-${TULIP_VERSION} DESTINATION ${TULIP_PLUGINS_DIR})
