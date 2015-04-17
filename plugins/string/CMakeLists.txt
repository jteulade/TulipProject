INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(ToLabels-${TulipVersion} SHARED ToLabels.cpp)
TARGET_LINK_LIBRARIES(ToLabels-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(SNPIdentified-${TulipVersion} SHARED SNPIdentified.cpp)
TARGET_LINK_LIBRARIES(SNPIdentified-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS ToLabels-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT ToLabels)
INSTALL(TARGETS SNPIdentified-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT SNPIdentified)

