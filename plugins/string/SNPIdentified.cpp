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
#include <tulip/TulipViewSettings.h>

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
class SNPIdentified: public tlp::StringAlgorithm {
public:
  PLUGININFORMATION("SNP identified","Jules Teulade-Denantes","2012/03/16","Identify SNPs related to a disease","1.0","")
  SNPIdentified(const tlp::PluginContext* context): StringAlgorithm(context) {
    addInParameter<PropertyInterface*>("input",paramHelp[0],"viewMetric",true);
    addInParameter<StringCollection>("levelSelection", paramHelp[4], AGGREGATION_FUNCTIONS);
  }

  bool run() {
    PropertyInterface* input = NULL;
    StringCollection levelSelection(AGGREGATION_FUNCTIONS);
    levelSelection.setCurrent(0);

    if (dataSet != NULL) {
      dataSet->get("input",input);
      dataSet->get("levelSelection",levelSelection);
    }
    IntegerProperty *level = graph->getProperty<IntegerProperty>("level");
    IntegerProperty *position = graph->getProperty<IntegerProperty>("position");



    // Sets different shapes and colors for each layer of the tree
    IntegerProperty *viewShape = graph->getProperty<IntegerProperty>("viewShape");
    ColorProperty *viewColor = graph->getProperty<ColorProperty>("viewColor");
    SizeProperty *viewSize = graph->getProperty<SizeProperty>("viewSize");

      pluginProgress->setComment("Copying nodes values");
      node n;
      forEach(n, graph->getNodes()) {
          //all the level: 0 && condition on a node (pvalue for example)
          if ((levelSelection.getCurrent() == 0 || levelSelection.getCurrent() - 1 == level->getNodeValue(n))
                  && position->getNodeValue(n)%20 == 0 ) {
              //we choose a specific layout for the identified SNPS
              viewShape->setNodeValue(n, tlp::NodeShape::Star);
              viewColor->setNodeValue(n, Color::Azure);
              viewSize->setNodeValue(n, Size(30,30,30));
          }

      }


    return true;
  }
};

PLUGIN(SNPIdentified)
