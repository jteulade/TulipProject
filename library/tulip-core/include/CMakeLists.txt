INSTALL(FILES
  tulip/Plugin.h
  tulip/AbstractProperty.h
  tulip/AcyclicTest.h
  tulip/Algorithm.h
  tulip/Array.h
  tulip/Bfs.h
  tulip/BiconnectedTest.h
  tulip/BooleanProperty.h
  tulip/BoundingBox.h
  tulip/Circle.h
  tulip/Color.h
  tulip/ColorProperty.h
  tulip/ColorScale.h
  tulip/ConcatIterator.h
  tulip/ConnectedTest.h
  tulip/ConsoleUtils.h
  tulip/conversioniterator.h
  tulip/ConvexHull.h
  tulip/Coord.h
  tulip/DataSet.h
  tulip/Delaunay.h
  tulip/DoubleProperty.h
  tulip/DrawingTools.h
  tulip/Edge.h
  tulip/ExportModule.h
  tulip/Face.h
  tulip/FaceIterator.h
  tulip/filteriterator.h
  tulip/ForEach.h
  tulip/GraphAbstract.h
  tulip/GraphDecorator.h
  tulip/Graph.h
  tulip/GraphTest.h
  tulip/GraphMeasure.h
  tulip/GraphProperty.h
  tulip/GraphTools.h
  tulip/ImportModule.h
  tulip/IntegerProperty.h
  tulip/Iterator.h
  tulip/LayoutProperty.h
  tulip/MapIterator.h
  tulip/Matrix.h
  tulip/memorypool.h
  tulip/minmaxproperty.h
  tulip/MutableContainer.h
  tulip/Node.h
  tulip/NumericProperty.h
  tulip/Observable.h
  tulip/Ordering.h
  tulip/OuterPlanarTest.h
  tulip/PlanarConMap.h
  tulip/PlanarityTest.h
  tulip/PluginContext.h
  tulip/PluginLibraryLoader.h
  tulip/PluginLister.h
  tulip/PluginLoader.h
  tulip/PluginLoaderTxt.h
  tulip/PluginProgress.h
  tulip/TemplateAlgorithm.h
  tulip/PropertyAlgorithm.h
  tulip/PropertyInterface.h
  tulip/PropertyTypes.h
  tulip/PropertyWrapper.h
  tulip/QuadTree.h
  tulip/Rectangle.h
  tulip/SimplePluginProgress.h
  tulip/SimpleTest.h
  tulip/Size.h
  tulip/SizeProperty.h
  tulip/SortIterator.h
  tulip/StableIterator.h
  tulip/StlFunctions.h
  tulip/StlIterator.h
  tulip/StoredType.h
  tulip/StringCollection.h
  tulip/StringProperty.h
  tulip/SystemDefinition.h
  tulip/TlpTools.h
  tulip/TreeTest.h
  tulip/TriconnectedTest.h
  tulip/tulipconf.h
  tulip/TulipException.h
  tulip/tuliphash.h
  tulip/TulipPluginHeaders.h
  tulip/TulipViewSettings.h
  tulip/uniqueiterator.h
  tulip/VectorCast.h
  tulip/vectorgraph.h
  tulip/vectorgraphproperty.h
  tulip/Vector.h
  tulip/WithDependency.h
  tulip/WithParameter.h
  tulip/TypeInterface.h
  tulip/SerializableType.h
  tulip/YajlFacade.h
  ${CMAKE_CURRENT_BINARY_DIR}/tulip/TulipRelease.h
DESTINATION ${TulipIncludeInstallDir}/tulip/
COMPONENT tulip_core_dev
)

INSTALL(FILES
  tulip/cxx/AbstractProperty.cxx
  tulip/cxx/Array.cxx
  tulip/cxx/Circle.cxx
  tulip/cxx/DataSet.cxx
  tulip/cxx/Graph.cxx
  tulip/cxx/Matrix.cxx
  tulip/cxx/minmaxproperty.cxx
  tulip/cxx/MutableContainer.cxx
  tulip/cxx/Vector.cxx
DESTINATION ${TulipIncludeInstallDir}/tulip/cxx/
COMPONENT tulip_core_dev
)
