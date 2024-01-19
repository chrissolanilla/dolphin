#pragma once


#include "SlippiUtility.h"

// https://docs.google.com/spreadsheets/d/1xVvcsGZg930uVhIawacDp-brbNpLJQtzj3ry-ZQaXWo/edit?usp=sharing
#define RANGE_TO_SIZE(start, end) {start, end-start}
struct MemRegions
{
  std::vector<PreserveBlock> excludeSections;

  std::vector<SlippiUtility::Savestate::ssBackupLoc> memRegions;
  MemRegions() {
    excludeSections = {
        // {start address, size}
        {0x94000000, 0xF4240},
        {0x80663e00, 0x1a4},  // CameraController
        {0x80663b40, 0x198},  // cmAiController
        {0x805b6d20, 0x740},  // gfCameraManager

        // savestate fixes

        {0x80bd5a60, 0x99},  // r3 of processEffect/[ecMgr]. Size is a ghidra-based guess

        {0x80663300, 0x114},  // r3 of processEffect/[efManager]. Size is based on ghidra guess

        {0x80bccff0, 0x80},  // r3 of update_just/[ecEffectWork]. Size based on ghidra guess

        {0x8062fb40, 0x2},  // Global var clearGeneralTerm/[soTransitionTerm] inf loop fix

        {0x80672f40, 0x4e0},  // r3 of renderPre/[gfSceneRoot], size based on ghidra guess

        {0x80662b40, 0xe8},  // r3 of DefG3dProcScnGroup/[nw4r3g3d8ScnGroupFUlUlPv], size based on ghidra guess

        // whoopsie excluded regions
        RANGE_TO_SIZE(0x8062fb40, 0x806312c0),  // transition term stuff
        RANGE_TO_SIZE(0x80662c28, 0x80663040),  // model stuff? and acmd
        RANGE_TO_SIZE(0x80663414, 0x80663440),  // addEffect
        // RANGE_TO_SIZE(0x80663cd8, 0x80663ce0), // dump/[gfMemoryPool] - may be part of the mem
        // regions
        // RANGE_TO_SIZE(0x80663fa4, 0x80663fc0), // dump/[gfMemoryPool] - may be part of the mem
        // regions
        RANGE_TO_SIZE(0x80673420, 0x80673460),  // something to do with scenes (could have to do
                                                // with the G3d/renderPre crash)
        RANGE_TO_SIZE(0x80bcd070, 0x80bd5400),  // more effect/acmd stuff
        RANGE_TO_SIZE(0x80bd5af9, 0x80bd5b20),  // processEffect/[ecMgr] / render/[ecMgr]
    };

    memRegions = {
    };
  }
};
extern MemRegions* memRegions;
