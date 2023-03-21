#pragma once


#include "SlippiUtility.h"

// https://docs.google.com/spreadsheets/d/1xVvcsGZg930uVhIawacDp-brbNpLJQtzj3ry-ZQaXWo/edit?usp=sharing

struct MemRegions
{
  std::vector<PreserveBlock> excludeSections;

  std::vector<SlippiUtility::Savestate::ssBackupLoc> memRegions;
  MemRegions() {
      excludeSections = {
          // {start address, size}

          {0x935D0000,
           0x935E0000 - 0x935D0000},  // CPP Framework heap (subject to change...??)
                                      // might also need the initializer sections(?) where global
                                      // statics are stored cpp framework code sections
          {0x817da5a4, 0x81FFFFFF - 0x817da5a4},
          {0x8055A600, 0x80563100 - 0x8055A600},
          {0x805B5200, 0x805B61D0 - 0x805B5200},
          {0x817CE880, 0x817DA590 - 0x817CE880},

          {0x80663e00, 0x1a4},  // CameraController
          {0x80663b40, 0x198},  // cmAiController
          {0x805b6d20, 0x740},  // gfCameraManager
          {0x80673460, 0x3D3D40}, // OverlayCommon
          {0x80c2c860, 0x8E00}, // InfoResource


      };
    memRegions = {};
  }
};
extern MemRegions* memRegions;
