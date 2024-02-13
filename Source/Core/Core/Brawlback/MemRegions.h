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
        {0x94000000, 0xF4240}, // syriinge
        /*
        
        */
        /*
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
        */
    };

    memRegions = {
      {0x81049e60, 0x81061060, nullptr, "Tmp"},
      {0x806414a0, 0x806414a0 + 0x4c, nullptr, "g_soSlow"},
      {0x8062f440, 0x8062f440 + 0x4f8, nullptr, "g_grCollisionManager"},
      {0x8063ff60, 0x8063ff60 + 0x1508, nullptr, "g_soCollisionManager"},
      {0x8063dcd0, 0x8063dcd0 + 0x2270, nullptr, "g_soCollisionManager->collisionLogs"},
      {0x8063da80, 0x8063da80 + 0x2270, nullptr, "g_soCollisionManager->logGroups"},
      {0x80624780, 0x80624780 + 0x50, nullptr, "ftEntryManager"},
      {0x806232f0, 0x806232f0 + 0x1464, nullptr, "ftEntryManager->ftEntries"},
      {0x806273c0, 0x806273c0 + 0x540, nullptr, "ftSlotManager"},
      {0x80624810, 0x80624810 + 0x2B74, nullptr, "ftSlotManager->ftSlots"},
      {0x80622d20, 0x80622d20 + 0x388, nullptr, "aiMgr"},
      {0x80621270, 0x80621270 + 0x1A7C, nullptr, "aiMgr->aiStatArray__15"},
      {0x8061f920, 0x8061f920 + 0x1964, nullptr, "aiMgr->aiWeaponMgr"},
      {0x8061f460, 0x8061f460 + 0x484, nullptr, "aiMgr->aiEnemyMgr"},
      {0x80615630, 0x80615630 + 0x6f08, nullptr, "aiMgr->field_0x34c"},
      {0x8061c570, 0x8061c570 + 0x2EC0, nullptr, "aiMgr->field_0x348"},
      {0x8063d9a0, 0x8063d9a0 + 0xa4, nullptr, "g_soJostleManager"},
      {0x8063d8e0, 0x8063d8e0 + 0x90, nullptr, "g_soCutInManager"},
      {0x80b879ac, 0x80b879ac + 0x14, nullptr, "g_soGeneralTermManager"},
      {0x806312f0, 0x806312f0 + 0xBB80, nullptr, "g_soGeneralTermManager->generalTerms"},
      {0x8062fb40, 0x8062fb40 + 0x1770, nullptr, "g_soGeneralTermManager->indices"},
      {0x8062f9e0, 0x8062f9e0 + 0x140, nullptr, "g_soEffectScreenManager"},
      {0x8062f960, 0x8062f960 + 0x4c, nullptr, "g_soCatchManager"},
      {0x80663300, 0x80663300 + 0x13c, nullptr, "g_efManager"}, // this is a gfTask "EffectManager", if there's issues these are a good place to start
      {0x80663280, 0x80663280 + 0x4c, nullptr, "g_efManager[1].field8_0x20"},
      {0x80663060, 0x80663060 + 0x1ec, nullptr, "g_efScreen"},
      {0x8062b360, 0x8062b360 + 0x14c0, nullptr, "g_itManager"},
      {0x80b86970, 0x80b86970 + 0xfc0, nullptr, "g_eventSystem"},
      {0x80613e00, 0x80613e00 + 0x28, nullptr, "g_clManager"},
      {0x80641520, 0x80641520 + 0x10, nullptr, "g_soWorld"},
      {0x80629a00, 0x80629a00 + 0x160, nullptr, "g_ftManager"},
      {0x80623180, 0x80623180 + 0xc, nullptr, "g_ftAudienceManager"},
      {0x806230e0, 0x806230e0 + 0x80, nullptr, "g_ftAudienceManager->audienceImpl"},
      {0x80627920, 0x80627920 + 0x10a4, nullptr, "g_ftManager->ftDataProviderPtr"},
      {0x805b8a00, 0x805b8a00 + 0x178, nullptr, "g_taskScheduler"},
      {0x8062f3e0, 0x8062f3e0 + 0x40, nullptr, "g_soArchiveDb"},
      {0x8062f360, 0x8062f360 + 0x4c, nullptr, "g_utArchiveManagers[2]"},
      {0x8062c840, 0x8062c840 + 0x1DDC, nullptr, "g_utArchiveManagers[2]->utListStart"},
      {0x80629980, 0x80629980 + 0x4c, nullptr, "g_utArchiveManagers[0]"},
      {0x80628a00, 0x80628a00 + 0xA80, nullptr, "g_utArchiveManagers[0]->utListStart"},
      {0x8123ab60, 0x8128cb60, nullptr, "Fighter1Instance"},
      {0x8128cb60, 0x812deb60, nullptr, "Fighter2Instance"},
      {0x8049edd8, 0x8049edd8 + 0x5064, nullptr, "g_efManager[1].field7_0x1c = nw4r2ef12EffectSystemFv"},
      {0x805b62a0, 0x805b62a0 + 0x100, nullptr, "gfRumble"},
      {0x80662b40, 0x80662b40 + 0xe8, nullptr, "g_efManager->field_0x4c"},
      {0x80662620, 0x80662620 + 0xe8, nullptr, "g_efManager->field_0x50"},
      {0x80672f40, 0x80672f40 + 0x50c, nullptr, "g_gfSceneRoot"},
      {0x8154e560, 0x81601960, nullptr, "Physics"},
      {0x8066b5e0, 0x8066b5e0 + 0xe8, nullptr, "g_gfSceneRoot->field_0x40"},
      {0x806673a0, 0x806673a0 + 0xe8, nullptr, "g_gfSceneRoot->field_0x44"},
      {0x80663fe0, 0x80663fe0 + 0xe8, nullptr, "g_gfSceneRoot->field_0x48"},
      {0x80672920, 0x80672920 + 0xe8, nullptr, "g_gfSceneRoot->field_0x38c[0][1]"},
      {0x80672300, 0x80672300 + 0xe8, nullptr, "g_gfSceneRoot->field_0x38c[1][1]"},
      {0x80671ce0, 0x80671ce0 + 0xe8, nullptr, "g_gfSceneRoot->field_0x38c[2][1]"},
      {0x806716c0, 0x806716c0 + 0xe8, nullptr, "g_gfSceneRoot->field_0x38c[3][1]"},
      {0x806710a0, 0x806710a0 + 0xe8, nullptr, "g_gfSceneRoot->field_0x38c[4][1]"},
      {0x80670a80, 0x80670a80 + 0xe8, nullptr, "g_gfSceneRoot->field_0x38c[5][1]"},
      {0x80670460, 0x80670460 + 0xe8, nullptr, "g_gfSceneRoot->field_0x38c[6][1]"},
      {0x8066fe40, 0x8066fe40 + 0xe8, nullptr, "g_gfSceneRoot->field_0x38c[7][1]"},
      {0x8066f820, 0x8066f820 + 0xe8, nullptr, "g_gfSceneRoot->field_0x38c[8][1]"},
      {0x81601960, 0x81734d60, nullptr, "InfoInstance"},
      {0x814ce460, 0x8154e560, nullptr, "StageInstance"},
      {0x805b6c80, 0x805b6c80 + 0x70, nullptr, "g_gfCopyEFBMgr"},
      {0x8049e57c, 0x8049e57c + 0xC, nullptr, "g_grCollisionList"},
      {0x80bd5a60, 0x80bd5a60 + 0xc0, nullptr, "g_ecMgr"},
      {0x80bccfe0, 0x80bccfe0 + 0x8420, nullptr, "g_ecMgr->field_0x60"},
      {0x80bccca0, 0x80bccca0 + 0x320, nullptr, "g_ecMgr->field_0x58"},
      {0x80bde380, 0x80c23a60, nullptr, "g_ecMgr->field_0x2c"},
      {0x80bd8360, 0x80bde360, nullptr, "g_ecMgr->field_0x30"},
      {0x80bd5b40, 0x80bd8340, nullptr, "g_ecMgr->field_0x34"},
      {0x80bd5420, 0x80bd5a40, nullptr, "g_ecMgr->gfEffectList"},
        /*
      {0x80bccc00, 0x80bccc80, nullptr, "g_ecMgr->field_0x44"},
      {0x80bcc940, 0x80bccbe0, nullptr, "g_ecMgr->field_0x44->entryArray"},
      {0x80bcc900, 0x80bcc920, nullptr, "g_ecMgr->field_0x44->prevArchive"},
      {0x80bcc7e0, 0x80bcc880, nullptr, "g_ecMgr->field_0x44->field_0x4"},
      {0x80bcc740, 0x80bcc7c0, nullptr, "g_ecMgr->field_0x44->nextArchive"},
      {0x80bcc620, 0x80bcc720, nullptr, "???Something in effect???"},
      {0x80bcc5e0, 0x80bcc600, nullptr, "???Something in effect???"},
      {0x80bcc540, 0x80bcc580, nullptr, "???Something in effect???"},
      */
    };
  }
};
extern MemRegions* memRegions;
