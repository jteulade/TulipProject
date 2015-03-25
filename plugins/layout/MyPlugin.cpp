/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include <cmath>
#include <algorithm>
#include <tulip/GraphTools.h>
#include "TreeTools.h"
#include "Orientation.h"
#include "DatasetTools.h"
#include "MyPlugin.h"
using namespace std;
using namespace tlp;


PLUGIN(MyPlugin)

//====================================================================
MyPlugin::MyPlugin(const tlp::PluginContext* context)
:LayoutAlgorithm(context) {
  addNodeSizePropertyParameter(this);
  addOrientationParameters(this);
  addSpacingParameters(this);
}

//====================================================================
MyPlugin::~MyPlugin() {
}
//====================================================================
void MyPlugin::computeLevelHeights(tlp::Graph *tree, tlp::node n, unsigned int depth,
                                     OrientableSizeProxy *oriSize) {
  if (levelHeights.size() == depth)
    levelHeights.push_back(0);

  float nodeHeight = oriSize->getNodeValue(n).getH();

  if (nodeHeight > levelHeights[depth])
    levelHeights[depth] = nodeHeight;

  node on;
  forEach(on, tree->getOutNodes(n))
    computeLevelHeights(tree, on, depth + 1, oriSize);
}
//====================================================================
bool MyPlugin::run() {
  orientationType mask = getMask(dataSet);
  OrientableLayout *oriLayout = new OrientableLayout(result, mask);
  SizeProperty* size;

  if (!getNodeSizePropertyParameter(dataSet, size))
    size = graph->getProperty<SizeProperty>("viewSize");


  OrientableSizeProxy oriSize(size, mask);
  getSpacingParameters(dataSet, nodeSpacing, spacing);

  if (pluginProgress)
    pluginProgress->showPreview(false);

  // push a temporary graph state (not redoable)
  // preserving layout updates
  std::vector<PropertyInterface*> propsToPreserve;

  if (result->getName() != "")
    propsToPreserve.push_back(result);

  graph->push(false, &propsToPreserve);

  tree = TreeTest::computeTree(graph, pluginProgress);

  if (pluginProgress && pluginProgress->state() != TLP_CONTINUE) {
    graph->pop();
    return false;
  }

  root = tree->getSource();
  computeLevelHeights(tree, root, 0, &oriSize);

  // check if the specified layer spacing is greater
  // than the max of the minimum layer spacing of the tree
  for (unsigned int i = 0; i < levelHeights.size() - 1;  ++i) {
    float minLayerSpacing = (levelHeights[i] + levelHeights[i + 1]) / 2;

    if (minLayerSpacing + nodeSpacing > spacing)
      spacing = minLayerSpacing + nodeSpacing;
  }

  setAllNodesCoordXY(oriLayout);

  // forget last temporary graph state
  graph->pop();

  delete oriLayout;
  return true;
}

//====================================================================
void MyPlugin::setAllNodesCoordXY(OrientableLayout *oriLayout) {

    //Return the variable position on the genome
    IntegerProperty *position = tree->getProperty<IntegerProperty>("position");

    //Return the variable level (SNPs, latent variables)
    IntegerProperty *level = tree->getProperty<IntegerProperty>("level");

    Iterator<node> *itNode =  tree->getNodes();

    while (itNode->hasNext()) {
        node currentNode   = itNode->next();
        OrientableCoord coord   =  oriLayout->getNodeValue(currentNode);

        //x depends of the variable position on the genome
        float newX      =   position->getNodeValue(currentNode) * nodeSpacing / 10000.;

        //y depends of the variable level
        float newY      = - level->getNodeValue(currentNode) * spacing;

        float coordZ    =   coord.getZ();

        setNodePosition(currentNode, newX, newY, coordZ, oriLayout);
    }
    delete itNode;
}




//====================================================================
inline void MyPlugin::setNodePosition(tlp::node n, float x, float y,
                                        float z, OrientableLayout *oriLayout) {
  OrientableCoord coord = oriLayout->createCoord(x, y, z);
  oriLayout->setNodeValue(n, coord);
}
