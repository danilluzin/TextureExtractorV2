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
"\n"
"objFilePath = model.obj\n"
"\n"
"cameraListFilePath =  list.txt\n"
"\n"
"cameraInfoPath = bundle.rd.out\n"
"\n"
"photoFolderPath = .\n"
"\n"
"newTextureFolderPath = .\n"
"\n"
"textureWidth = 4000\n"
"textureHeight = 4000\n"
"\n"
"getLabelingFromFile = false\n"
"\n"
"writeLabelingToFile = true\n"
"\n"
"getDataCostsFromFile = false\n"
"\n"
"writeDataCostsToFile = true\n"
"\n"
"verbose = true\n"
"\n"
"imageFormat = png\n"
"\n"
"threadCount = 4\n"
"\n"
"[optional]\n"
"\n"
"labelingFilePath =\n"
"\n"
"dataCostsFilePath =\n"
"\n"
"addProjectNameToFiles = false\n"
"\n"
"projectName = \n"
"\n"
"doGloabalAdjustment = true\n"
"\n"
"doSeamLeveling = true\n"
"\n"
"doTextureExtension = true\n"
"\n"
"doColorConsistency = true\n"
"\n"
"colorConsistencyThreshold = 0.5\n"
"\n"
"\n"
"[perfomance]\n"
"\n"
"BVHMinNode = 200\n"
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
"resultRenderFolder = .\n"
"";

#endif /* misc_h */

