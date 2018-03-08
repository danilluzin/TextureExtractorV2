//
//  misc.h
//  TextureExtractorV2
//
//  Created by Danil Luzin on 08/03/2018.
//  Copyright © 2018 Danil Luzin. All rights reserved.
//

#ifndef Misc_h
#define Misc_h

#include<iostream>
 
static const std::string COLOR_BLACK = "\033[0;30m";
static const std::string COLOR_DARKBLUE = "\033[0;34m";
static const std::string COLOR_DARKGREEN = "\033[0;32m";
static const std::string COLOR_DARKTEAL = "\033[0;36m";
static const std::string COLOR_DARKRED = "\033[0;31m";
static const std::string COLOR_DARKPINK = "\033[0;35m";
static const std::string COLOR_DARKYELLOW = "\033[0;33m";
static const std::string COLOR_GRAY = "\033[0;37m";
static const std::string COLOR_DARKGRAY = "\033[1;30m";
static const std::string COLOR_BLUE = "\033[1;34m";
static const std::string COLOR_GREEN = "\033[1;32m";
static const std::string COLOR_TEAL = "\033[1;36m";
static const std::string COLOR_RED = "\033[1;31m";
static const std::string COLOR_PINK = "\033[1;35m";
static const std::string COLOR_YELLOW = "\033[1;33m";
static const std::string COLOR_WHITE = "\033[1;37m";
static const std::string COLOR_RESET = "\033[0m";
static const std::string COLOR_BOLD = "\e[1m";

static const std::string exampleIni ="\n"
"[basics]\n"
" \n"
"objFilePath = resources/slany/slany_cut4_uvseperate.obj\n"
"\n"
"cameraListFilePath = resources/slany/list3.txt\n"
"\n"
"cameraInfoPath = resources/slany/bundle.rd.out\n"
"\n"
"photoFolderPath = resources/slany\n"
"\n"
"newTextureFolderPath = working_resources/slany/derived9\n"
"\n"
"textureWidth = 4000\n"
"textureHeight = 4000\n"
"\n"
"getLabelingFromFile = false\n"
"\n"
"writeLabelingToFile = true\n"
"\n"
"getDataCostsFromFile = true\n"
"\n"
"writeDataCostsToFile = true\n"
"\n"
"verbose = true\n"
"\n"
"imageFormat = png\n"
"\n"
"threadCount = 4\n"
"\n"
"\n"
"\n"
"[optional]\n"
"\n"
"labelingFilePath = working_resources/slany/derived9/new_labeling.txt\n"
"\n"
"newLabelingFilePath = working_resources/slany/derived9/new_labeling.txt\n"
"\n"
"dataCostsFilePath = working_resources/slany/derived9/out_dataCosts.txt\n"
"\n"
"newDataCostsFilePath = working_resources/slany/derived9/out_dataCosts.txt\n"
"\n"
"addProjectNameToFiles = true\n"
"\n"
"projectName = slanyOut\n"
"\n"
"doGloabalAdjustment = true\n"
"\n"
"doSeamLeveling = true\n"
"\n"
"doTextureExtension = true\n"
"\n"
"doColorConsistency = true\n"
"\n"
"colorConsistencyThreshold = 0.7\n"
"\n"
"[perfomance]\n"
"\n"
"BVHMinNode = 200;\n"
"\n"
"\n"
"\n"
"[debug]\n"
"\n"
"genRawTexture = false\n"
"\n"
"genLevelingTexture = false\n"
"\n"
"genMaskTexture = false\n"
"\n"
"genLebelingTexture = false\n"
"\n"
"genGlobalTexture = false\n"
"\n"
"justRender = false\n"
"\n"
"renderInTheEnd = false\n"
"\n"
"rasterLabelAssignmentFolder = working_resources/slany/derived9/labeling\n"
"\n"
"resultRenderFolder = working_resources/slany/derived9/res\n"
"\n"
"rasterLabelAssignment = true\n"
"\n"
"";

#endif /* misc_h */

