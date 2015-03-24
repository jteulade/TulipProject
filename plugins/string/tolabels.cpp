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

#include <tulip/TulipPluginHeaders.h>

using namespace tlp;

namespace {
const char * paramHelp[] = {
  // property
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "PropertyInterface" ) \
  HTML_HELP_BODY() \
  "Property to copy on labels" \
  HTML_HELP_CLOSE(),
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "BooleanProperty" ) \
  HTML_HELP_DEF( "default", "none" ) \
  HTML_HELP_BODY() \
  "Set of elements for which to set the labels." \
  HTML_HELP_CLOSE(),
  // on nodes
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "bool" ) \
  HTML_HELP_BODY() \
  "Copy nodes values" \
  HTML_HELP_CLOSE(),
  // property
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "bool" ) \
  HTML_HELP_BODY() \
  "Copy edges values" \
  HTML_HELP_CLOSE(),

    //level
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "String Collection" ) \
      HTML_HELP_DEF( "default", "ALL" )   \
    HTML_HELP_BODY() \
    "Allow to display only a specific level" \
    HTML_HELP_CLOSE()
};
}

//allow to display only a specific variable level
#define AGGREGATION_FUNCTIONS "ALL;SNP;VL1;VL2;VL3;VL4;VL5;VL6"
class ToLabels: public tlp::StringAlgorithm {
public:
  PLUGININFORMATION("To labels","Ludwig Fiolka","2012/03/16","Map a property on nodes/edges labels","1.0","")
  ToLabels(const tlp::PluginContext* context): StringAlgorithm(context) {
    addInParameter<PropertyInterface*>("input",paramHelp[0],"viewMetric",true);
    addInParameter<StringCollection>("levelSelection", paramHelp[4], AGGREGATION_FUNCTIONS);

    addInParameter<BooleanProperty>("selection",paramHelp[1],"",false);
    addInParameter<bool>("nodes",paramHelp[2],"true");
    addInParameter<bool>("edges",paramHelp[3],"true");
  }

  bool run() {
    PropertyInterface* input = NULL;
    StringCollection levelSelection(AGGREGATION_FUNCTIONS);
    levelSelection.setCurrent(0);
    BooleanProperty* selection = NULL;
    bool onNodes = true;
    bool onEdges = true;

    if (dataSet != NULL) {
      dataSet->get("input",input);
      dataSet->get("levelSelection",levelSelection);
      dataSet->get("selection",selection);
      dataSet->get("nodes",onNodes);
      dataSet->get("edges",onEdges);
    }
    IntegerProperty *level = graph->getProperty<IntegerProperty>("level");

    if (onNodes) {
      pluginProgress->setComment("Copying nodes values");
      int step=0,max_step = graph->numberOfNodes();
      node n;
      forEach(n, selection ? selection->getNonDefaultValuatedNodes() : graph->getNodes()) {
          //0 is when all the layers are selected
          if (levelSelection.getCurrent() == 0 || levelSelection.getCurrent() - 1 == level->getNodeValue(n)) {
            pluginProgress->progress(step++,max_step);
            //adding spaces allows to have distinct and readable labels on the graph
            result->setNodeValue(n,"\n\n " + input->getNodeStringValue(n) + " ");
          }
      }
    }

    if (onEdges) {
      pluginProgress->setComment("Copying edges values");
      int step=0,max_step = graph->numberOfEdges();
      edge e;
      forEach(e, selection ? selection->getNonDefaultValuatedEdges() : graph->getEdges()) {
        pluginProgress->progress(step++,max_step);
        result->setEdgeValue(e,input->getEdgeStringValue(e));
      }
    }

    return true;
  }
};

PLUGIN(ToLabels)
