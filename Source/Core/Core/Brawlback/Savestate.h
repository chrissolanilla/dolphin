#pragma once

#include "SlippiUtility.h"
#include "BrawlbackUtility.h"

using namespace SlippiUtility::Savestate;

// thank you Slippi :)

class BrawlbackSavestate
{

public:


    BrawlbackSavestate(SavestateMemRegionInfo* regions = nullptr, u32 numRegions = 0);
    ~BrawlbackSavestate();


    void Capture();
    void Load(std::vector<PreserveBlock> blocks);
    void DisplaySavestateSize(const std::vector<SlippiUtility::Savestate::ssBackupLoc>& regions);
    void DisplaySavestateSize();

    void UpdateDynamicMemRegionsForSavestate(SavestateMemRegionInfo* regions, u32 numRegions);

    std::vector<ssBackupLoc>* getBackupLocs() { return &backupLocs; }
private:


    std::vector<ssBackupLoc> backupLocs = {};
    std::vector<ssBackupLoc> dynamicLocs = {};

    std::unordered_map<u32, ssBackupLoc> memRegionMap = {};

    //std::unordered_map<PreserveBlock, std::vector<u8>, preserve_hash_fn, preserve_eq_fn> preservationMap;



    void initBackupLocs();


};
