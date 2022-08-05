#pragma once


#include "SlippiUtility.h"

// https://docs.google.com/spreadsheets/d/1xVvcsGZg930uVhIawacDp-brbNpLJQtzj3ry-ZQaXWo/edit?usp=sharing

namespace MemRegions {


// for use with excludeSections since PreserveBlocks are addr + size, and sometimes I want to input a start/end addr
#define RANGE_TO_SIZE(start, end) {start, end-start}

static std::vector<PreserveBlock> excludeSections = {
    // {start address, size}

   RANGE_TO_SIZE(0x935D0000, 0x935E0000), // CPP Framework heap (subject to change...??)
    // might also need the initializer sections(?) where global statics are stored
    // cpp framework code sections
    RANGE_TO_SIZE(0x817da5a4, 0x81FFFFFF),
    RANGE_TO_SIZE(0x8055A600, 0x80563100),
    RANGE_TO_SIZE(0x805B5200, 0x805B61D0),
    RANGE_TO_SIZE(0x817CE880, 0x817DA590),

    {0x80663e00, 0x1a4}, // CameraController
    {0x80663b40, 0x198}, // cmAiController
    {0x805b6d20, 0x740}, // gfCameraManager

    // savestate fixes
    
    {0x80bd5a60, 0x99}, // r3 of processEffect/[ecMgr]. Size is a ghidra-based guess                        

    {0x80663300, 0x114}, // r3 of processEffect/[efManager]. Size is based on ghidra guess                  

    {0x80bccff0, 0x80}, // r3 of update_just/[ecEffectWork]. Size based on ghidra guess                     

    {0x8062fb40, 0x2}, // Global var clearGeneralTerm/[soTransitionTerm] inf loop fix          

    {0x80672f40, 0x4e0}, // r3 of renderPre/[gfSceneRoot], size based on ghidra guess                      

    {0x80662b40, 0xe8}, // r3 of DefG3dProcScnGroup/[nw4r3g3d8ScnGroupFUlUlPv], size based on ghidra guess
    

    // whoopsie excluded regions
    RANGE_TO_SIZE(0x8062fb40, 0x806312c0), // transition term stuff
    RANGE_TO_SIZE(0x80662c28, 0x80663040), // model stuff? and acmd
    RANGE_TO_SIZE(0x80663414, 0x80663440), // addEffect
    //RANGE_TO_SIZE(0x80663cd8, 0x80663ce0), // dump/[gfMemoryPool] - may be part of the mem regions
    //RANGE_TO_SIZE(0x80663fa4, 0x80663fc0), // dump/[gfMemoryPool] - may be part of the mem regions
    RANGE_TO_SIZE(0x80673420, 0x80673460), // something to do with scenes (could have to do with the G3d/renderPre crash)
    RANGE_TO_SIZE(0x80bcd070, 0x80bd5400), // more effect/acmd stuff
    RANGE_TO_SIZE(0x80bd5af9, 0x80bd5b20), // processEffect/[ecMgr] / render/[ecMgr]
};

// used if dynamic regions hasn't been populated

static std::vector<ssBackupLoc> memRegions = {
    // {start address, end address, nullptr, "NameOfSection"},

    // ============================= mem1 =============================

    {0x805b8a00, 0x805b8a00+0x17c, nullptr, "gfTaskScheduler"}, // gfTaskScheduler 
    
    {0x80611f60, 0x80673460, nullptr, "System"}, // System
    {0x80b8db60, 0x80c23a60, nullptr, "Effect"}, // Effect
    //{0x8123ab60, 0x8128cb60, nullptr, "Fighter1Instance"}, // Fighter1Instance
    //{0x8128cb60, 0x812deb60, nullptr, "Fighter2Instance"}, // Fighter2Instance
    //{0x81601960, 0x81734d60, nullptr, "InfoInstance"}, // InfoInstance
    //{0x815edf60, 0x817bad60, nullptr, "InfoExtraResource"}, // InfoExtraResource
    //{0x80c23a60, 0x80da3a60, nullptr, "InfoResource"}, // InfoResource
    {0x8154e560, 0x81601960, nullptr, "Physics"}, // Physics

    //{0x80673460, 0x80b8db60, nullptr, "OverlayCommon"}, // OverlayCommon
    {0x80A471A0, 0x80b8db60, nullptr, "OverlayCommon 4/4"}, // OverlayCommon 4/4

    // ============================= mem2 =============================


    {0x90e61400, 0x90e77500, nullptr, "WiiPad"}, // WiiPad

    //{0x9151fa00, 0x91a72e00, nullptr, "Fighter1Resource"},
    //{0x9151fa00, 0x917C9400, nullptr, "first half of Fighter1Resource"}, // first half of Fighter1Resource
    //{0x91a72e00, 0x91b04c80, nullptr, "Fighter1Resource2"}, // Fighter1Resource2

    //{0x91b04c80, 0x92058080, nullptr, "Fighter2Resource"},
    //{0x91b04c80, 0x91DAE680, nullptr, "Fighter2Resource first half"}, // Fighter2Resource first half
    //{0x92058080, 0x920e9f00, nullptr, "Fighter2Resource2"}, // Fighter2Resource2

    {0x91478e00, 0x914d2900, nullptr, "FighterEffect"}, // FighterEffect


    {0x92cb4400, 0x92dcdf00, nullptr, "FighterTechqniq"}, // FighterTechqniq

    { 0x9134cc00, 0x9134cc10, nullptr, "CopyFB_Edited" },


    {0x90167400, 0x90199800, nullptr, "GameGlobal"}, // GameGlobal

};


} // namespace MemRegions