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
#include "GenomeSorting.h"
using namespace std;
using namespace tlp;


PLUGIN(GenomeSorting)

//====================================================================
GenomeSorting::GenomeSorting(const tlp::PluginContext* context)
:LayoutAlgorithm(context) {
  addNodeSizePropertyParameter(this);
  addOrientationParameters(this);
  addSpacingParameters(this);
}

//====================================================================
GenomeSorting::~GenomeSorting() {
}
//====================================================================
void GenomeSorting::computeLevelHeights(tlp::Graph *tree, tlp::node n, unsigned int depth,
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
bool GenomeSorting::run() {
  orientationType mask = getMask(dataSet);
  OrientableLayout *oriLayout = new OrientableLayout(result, mask);
  SizeProperty* size;

  if (!getNodeSizePropertyParameter(dataSet, size))
    size = graph->getProperty<SizeProperty>("viewSize");

  //allow to modify the nodes color and size
  ColorProperty *viewColor = graph->getProperty<ColorProperty>("viewColor");
  SizeProperty *viewSize = graph->getProperty<SizeProperty>("viewSize");

  OrientableSizeProxy oriSize(size, mask);
  getSpacingParameters(dataSet, nodeSpacing, spacing);

  if (pluginProgress)
    pluginProgress->showPreview(false);

  // push a temporary graph state (not redoable)
  // preserving layout updates
  std::vector<PropertyInterface*> propsToPreserve;

  if (result->getName() != "")
    propsToPreserve.push_back(result);

  //preserve the changes of nodes color
  if (viewColor->getName() != "")
    propsToPreserve.push_back(viewColor);

  //preserve the changes of nodes size
  if (viewSize->getName() != "")
    propsToPreserve.push_back(viewSize);

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

  //adjust the edge size according to the source node and the target node
  edge e;
  forEach(e,tree->getEdges())
      viewSize->setEdgeValue(e, getEdgeValue(e));

  // forget last temporary graph state
  graph->pop();

  delete oriLayout;
  return true;
}

//====================================================================
void GenomeSorting::setAllNodesCoordXY(OrientableLayout *oriLayout) {

    //Return the variable position on the genome
    IntegerProperty *position = tree->getProperty<IntegerProperty>("position");

    //Return the variable level (SNPs, latent variables)
    IntegerProperty *level = tree->getProperty<IntegerProperty>("level");

    Iterator<node> *itNode =  tree->getNodes();

    SizeProperty *viewSize = tree->getProperty<SizeProperty>("viewSize");

    ColorProperty *viewColor = tree->getProperty<ColorProperty>("viewColor");
    //adapt the colors according to the nodes level
    std::vector<Color> levelColor;
    levelColor.push_back(Color::Blue);
    levelColor.push_back(Color::Red);
    levelColor.push_back(Color::Green);
    levelColor.push_back(Color::Yellow);
    levelColor.push_back(Color::Magenta);
    levelColor.push_back(Color::Azure);

    while (itNode->hasNext()) {
        node currentNode   = itNode->next();
        OrientableCoord coord   =  oriLayout->getNodeValue(currentNode);

        //x depends of the variable position on the genome
        float newX      =   position->getNodeValue(currentNode) * nodeSpacing / 10000.;

        //y depends of the variable level
        float newY      = - level->getNodeValue(currentNode) * spacing;

        float coordZ    =   coord.getZ();

        setNodePosition(currentNode, newX, newY, coordZ, oriLayout);

        //we modify the current node color according to its level
        viewColor->setNodeValue(currentNode, levelColor[level->getNodeValue(currentNode)]);

        //we choose the nodes size according to its level.
        //the values have been chosen arbitrarily
        int levelSize = (level->getNodeValue(currentNode) + 0.1) * 10;
        viewSize->setNodeValue(currentNode,  Size(levelSize,levelSize,levelSize));
    }
    delete itNode;
}




//====================================================================
inline void GenomeSorting::setNodePosition(tlp::node n, float x, float y,
                                        float z, OrientableLayout *oriLayout) {
  OrientableCoord coord = oriLayout->createCoord(x, y, z);
  oriLayout->setNodeValue(n, coord);
}

Size GenomeSorting::getEdgeValue(const edge e) {
  Size s = result->getNodeValue(tree->source(e));
  Size t = result->getNodeValue(tree->target(e));
  Coord tmp(s.getW(),s.getH(),s.getD());
  Coord tmp2(t.getW(),t.getH(),t.getD());
  float sizes=tmp.norm();
  float sizet=tmp2.norm();
  return (Size(sizes/16,sizet/16,sizet/4));
}
