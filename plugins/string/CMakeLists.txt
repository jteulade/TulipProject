INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(ToLabels-${TulipVersion} SHARED tolabels.cpp)
TARGET_LINK_LIBRARIES(ToLabels-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(FLTMLabels-${TulipVersion} SHARED FLTMLabels.cpp)
TARGET_LINK_LIBRARIES(FLTMLabels-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS ToLabels-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT ToLabels)
INSTALL(TARGETS FLTMLabels-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT FLTMLabels)

