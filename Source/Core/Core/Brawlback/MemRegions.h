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
      {0x80A471A0, 0x80b8db60, nullptr, "OverlayCommon 4/4"},
      {0x81049e60, 0x81061060, nullptr, "Tmp"},
      {0x806414a0, 0x806414a0 + 0x60, nullptr, "g_soSlow"},
      {0x8062f440, 0x8062f440 + 0x500, nullptr, "g_grCollisionManager"},
      {0x8063ff60, 0x8063ff60 + 0x1520, nullptr, "g_soCollisionManager"},
      {0x8063dcc0, 0x8063dcc0 + 0x2280, nullptr, "g_soCollisionManager->collisionLogs"},
      {0x8063da80, 0x8063da80 + 0x220, nullptr, "g_soCollisionManager->logGroups"},
      {0x80624780, 0x80624780 + 0x50, nullptr, "ftEntryManager"},
      {0x806232e0, 0x806232e0 + 0x1480, nullptr, "ftEntryManager->ftEntries"},
      {0x806273c0, 0x806273c0 + 0x540, nullptr, "ftSlotManager"},
      {0x80624800, 0x80624800 + 0x2ba0, nullptr, "ftSlotManager->ftSlots"},
      {0x80622d20, 0x80622d20 + 0x388, nullptr, "aiMgr"},
      {0x80621260, 0x80621260 + 0x1aa0, nullptr, "aiMgr->aiStatArray__15"},
      {0x8061f920, 0x8061f920 + 0x1920, nullptr, "aiMgr->aiWeaponMgr"},
      {0x8061f460, 0x8061f460 + 0x4a0, nullptr, "aiMgr->aiEnemyMgr"},
      {0x80615620, 0x80615620 + 0x6f20, nullptr, "aiMgr->field_0x34c"},
      {0x8061c560, 0x8061c560 + 0x2EE0, nullptr, "aiMgr->field_0x348"},
      {0x8063d9a0, 0x8063d9a0 + 0xc0, nullptr, "g_soJostleManager"},
      {0x8063d8e0, 0x8063d8e0 + 0xa0, nullptr, "g_soCutInManager"},
      {0x806312e0, 0x806312e0 + 0xbba0, nullptr, "g_soGeneralTermManager->generalTerms"},
      {0x8062fb40, 0x8062fb40 + 0x1780, nullptr, "g_soGeneralTermManager->indices"},
      {0x8062f9e0, 0x8062f9e0 + 0x140, nullptr, "g_soEffectScreenManager"},
      {0x8062f960, 0x8062f960 + 0x60, nullptr, "g_soCatchManager"},
      {0x80663300, 0x80663300 + 0x140, nullptr, "g_efManager"}, // this is a gfTask "EffectManager", if there's issues these are a good place to start
      {0x80663280, 0x80663280 + 0x60, nullptr, "g_efManager[1].field8_0x20"},
      {0x80663060, 0x80663060 + 0x200, nullptr, "g_efScreen"},
      {0x8062b360, 0x8062b360 + 0x14c0, nullptr, "g_itManager"},
      {0x80613e00, 0x80613e00 + 0x28, nullptr, "g_clManager"},
      {0x80641520, 0x80641520 + 0x20, nullptr, "g_soWorld"},
      {0x80629a00, 0x80629a00 + 0x160, nullptr, "g_ftManager"},
      {0x80623180, 0x80623180 + 0x20, nullptr, "g_ftAudienceManager"},
      {0x806230e0, 0x806230e0 + 0x80, nullptr, "g_ftAudienceManager->audienceImpl"},
      {0x80627920, 0x80627920 + 0x10c0, nullptr, "g_ftManager->ftDataProviderPtr"},
      {0x805b8a00, 0x805b8a00 + 0x180, nullptr, "g_taskScheduler"},
      {0x8062f3e0, 0x8062f3e0 + 0x40, nullptr, "g_soArchiveDb"},
      {0x8062f360, 0x8062f360 + 0x60, nullptr, "g_utArchiveManagers[2]"},
      {0x8062c840, 0x8062c840 + 0x1e00, nullptr, "g_utArchiveManagers[2]->utListStart"},
      {0x80629980, 0x80629980 + 0x60, nullptr, "g_utArchiveManagers[0]"},
      {0x80628a00, 0x80628a00 + 0xAA0, nullptr, "g_utArchiveManagers[0]->utListStart"},
      {0x8123ab60, 0x8128cb60, nullptr, "Fighter1Instance"},
      {0x8128cb60, 0x812deb60, nullptr, "Fighter2Instance"},
      {0x8049edd8, 0x8049edd8 + 0x5064, nullptr, "g_efManager[1].field7_0x1c = nw4r2ef12EffectSystemFv"},
      {0x805b62a0, 0x805b62a0 + 0x100, nullptr, "gfRumble"},
      {0x80662b40, 0x80662b40 + 0x500, nullptr, "g_efManager->field_0x4c"},
      {0x80662620, 0x80662620 + 0x500, nullptr, "g_efManager->field_0x50"},
      {0x80672f40, 0x80672f40 + 0x520, nullptr, "g_gfSceneRoot"},
      {0x8154e560, 0x81601960, nullptr, "Physics"},
      {0x8066b5e0, 0x8066b5e0 + 0x4220, nullptr, "g_gfSceneRoot->field_0x40"},
      {0x806673a0, 0x806673a0 + 0x4220, nullptr, "g_gfSceneRoot->field_0x44"},
      {0x80663fe0, 0x80663fe0 + 0x33a0, nullptr, "g_gfSceneRoot->field_0x48"},
      {0x80672920, 0x80672920 + 0x600, nullptr, "g_gfSceneRoot->field_0x38c[0][1]"},
      {0x80672300, 0x80672300 + 0x600, nullptr, "g_gfSceneRoot->field_0x38c[1][1]"},
      {0x80671ce0, 0x80671ce0 + 0x600, nullptr, "g_gfSceneRoot->field_0x38c[2][1]"},
      {0x806716c0, 0x806716c0 + 0x600, nullptr, "g_gfSceneRoot->field_0x38c[3][1]"},
      {0x806710a0, 0x806710a0 + 0x600, nullptr, "g_gfSceneRoot->field_0x38c[4][1]"},
      {0x80670a80, 0x80670a80 + 0x600, nullptr, "g_gfSceneRoot->field_0x38c[5][1]"},
      {0x80670460, 0x80670460 + 0x600, nullptr, "g_gfSceneRoot->field_0x38c[6][1]"},
      {0x8066fe40, 0x8066fe40 + 0x600, nullptr, "g_gfSceneRoot->field_0x38c[7][1]"},
      {0x8066f820, 0x8066f820 + 0x600, nullptr, "g_gfSceneRoot->field_0x38c[8][1]"},
      {0x81601960, 0x81734d60, nullptr, "InfoInstance"},
      {0x814ce460, 0x8154e560, nullptr, "StageInstance"},
      {0x805b6c80, 0x805b6c80 + 0x80, nullptr, "g_gfCopyEFBMgr"},
      {0x8049e57c, 0x8049e57c + 0xC, nullptr, "g_grCollisionList"},
      {0x80b8db60, 0x80c23a60, nullptr, "Effect"}
    };
  }
};
extern MemRegions* memRegions;
