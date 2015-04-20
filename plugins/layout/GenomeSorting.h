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
#ifndef GENOMESORTING_H
#define GENOMESORTING_H

#include <map>
#include <vector>
#include <tulip/TulipPluginHeaders.h>

class OrientableLayout;
class OrientableCoord;
class OrientableSizeProxy;

/** \addtogroup layout */

/** This plugin is an implementation of a GenomeSorting, an extended implementation
 *  of a "Bio representation" which includes variable orientation
 *  and variable node sizelayout.
 *
 *  \note This works on tree.
 *  Let n be the number of nodes, the algorithm complexity is in O(n).
 *
 *  \author Jules Teulade-Denantes
 *  University Nantes France
 **/
class GenomeSorting: public tlp::LayoutAlgorithm {
 public:
  PLUGININFORMATION("GenomeSorting",
                    "Jules Teulade-Denantes",
                    "03/12/04", "ok", "1.0","Tree")
    GenomeSorting(const tlp::PluginContext* context);
  ~GenomeSorting();

  bool run();

 private:
  float spacing;
  float nodeSpacing;

  std::map<tlp::node, float>   leftshift;
  tlp::node                    root;
  tlp::Graph *tree;
  std::vector<float> levelHeights;


  void   setAllNodesCoordXY(OrientableLayout *oriLayout);

  void    setNodePosition(tlp::node n, float x, float y, float z,
                          OrientableLayout *oriLayout);

  void computeLevelHeights(tlp::Graph* tree, tlp::node n, unsigned int depth,
                           OrientableSizeProxy *oriSize);

  //this function calculate the edge size according to the source node and the target node
  tlp::Size getEdgeValue(const tlp::edge e);
};

#endif


