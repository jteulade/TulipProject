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
    HTML_HELP_DEF( "type", "String Collection" ) \
    HTML_HELP_BODY() \
    "Property to copy on labels" \
    HTML_HELP_CLOSE(),

    //level
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "String Collection" ) \
      HTML_HELP_DEF( "default", "SNP" )   \
    HTML_HELP_BODY() \
    "Display only a specific level" \
    HTML_HELP_CLOSE(),

    //options
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "String Collection" ) \
      HTML_HELP_DEF( "default", "ALL" )   \
    HTML_HELP_BODY() \
    "Choose the variables to display" \
    HTML_HELP_CLOSE(),

    //approximation
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "String Collection" ) \
      HTML_HELP_DEF( "default", "bp" )   \
    HTML_HELP_BODY() \
    "Choose the unit of measure to display" \
    HTML_HELP_CLOSE()
};
}
//properties we can copy on labels
#define PROPERTIES "position;label;id;cardinality;pval"
#define LEVEL "SNP;ALL"
#define SELECTION "ALL;DISEASES"
#define APPROXIMATIONS "bp;kbp;Mbp"
class FLTMLabels: public StringAlgorithm  {
public:
  PLUGININFORMATION("FLTM labels","Jules Teulade-Denantes","2012/03/16","Labeling FLTM tree","1.0","")
  FLTMLabels(const tlp::PluginContext* context): StringAlgorithm(context)  {
    addInParameter<StringCollection>("Property to display",paramHelp[0],PROPERTIES);
    addInParameter<StringCollection>("Variable level", paramHelp[1], LEVEL);
    addInParameter<StringCollection>("Variable selection", paramHelp[2], SELECTION);
    addInParameter<StringCollection>("Unit of measure (for position only)", paramHelp[3], APPROXIMATIONS);
  }

  bool run() {
    StringCollection property(PROPERTIES);
    StringCollection levelSelection(LEVEL);
    StringCollection displaySelection(SELECTION);
    StringCollection positionApproximation(APPROXIMATIONS);

    if (dataSet != NULL) {
      dataSet->get("Property to display",property);
      dataSet->get("Variable level",levelSelection);
      dataSet->get("Variable selection",displaySelection);
      dataSet->get("Unit of measure (for position only)",positionApproximation);
    }

    // Sets different shapes and colors for each layer of the tree
    IntegerProperty *viewShape = graph->getProperty<IntegerProperty>("viewShape");
    ColorProperty *viewColor = graph->getProperty<ColorProperty>("viewColor");

    pluginProgress->setComment("Copying nodes values");
    int step=0,max_step = graph->numberOfNodes();
    node n;
    //we need to have all the properties required
    if (graph->existProperty("level") && graph->existProperty("position") && graph->existProperty(property.getCurrentString())) {
        IntegerProperty *level = graph->getProperty<IntegerProperty>("level");
        IntegerProperty *position = graph->getProperty<IntegerProperty>("position");
        forEach(n, graph->getNodes()) {
            pluginProgress->progress(step++,max_step);
            //all the levels or only SNP level
            if (levelSelection.getCurrentString() == "ALL" || level->getNodeValue(n) == 0) {
                std::string toDisplay = graph->getProperty(property.getCurrentString())->getNodeStringValue(n);
                //if the property to display is the position we can truncate it
                if (property.getCurrentString() == "position") {
                    //truncatedIndex indicates how many decimals we have to truncate
                    int truncatedIndex = toDisplay.size() - positionApproximation.getCurrent() * 3;
                    //we truncate the label node related to the position approximation chosen
                    toDisplay = (truncatedIndex >0) ? toDisplay.substr(0, truncatedIndex) : "0";
                }
                std::string nodeLabel = "";
                //if we choose to display diseases  && condition on a node (pvalue for example)
                if (displaySelection.getCurrent() == 1 && position->getNodeValue(n)%20 == 0) {
                    //we choose a specific layout for the diseased SNPS
                    viewShape->setNodeValue(n, tlp::NodeShape::Star);
                    viewColor->setNodeValue(n, Color::Brown);
                    //we print † for the disease and the node position
                    nodeLabel = "\n\n†\n " + toDisplay + " ";
                } else if (displaySelection.getCurrent() == 0) { //if we choose to display all the nodes positions
                    nodeLabel = "\n\n " + toDisplay + " ";
                } //otherwise, we don't display anything
                result->setNodeValue(n,nodeLabel);
            }
        }
    }

    return true;
  }
};

PLUGIN(FLTMLabels)
