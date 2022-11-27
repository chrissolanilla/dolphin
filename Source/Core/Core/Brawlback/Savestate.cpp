#include "Savestate.h"

#include <Common/MemoryUtil.h>
#include <Core/HW/EXI/EXI.h>
#include <Core/HW/Memmap.h>
#include <thread>
#include "Common/Thread.h"
#include "Common/Timer.h"
#include "VideoCommon/OnScreenDisplay.h"
#include "common/Logging/Log.h"

#include "MemRegions.h"

#define LOW_BOUND_MEM 0x80000000

// lots of code here is heavily derived from Slippi's Savestates.cpp

typedef std::vector<SlippiUtility::Savestate::ssBackupLoc> backupLocList;

void AllocateMemForBackupLocs(backupLocList& locs) {
    for (SlippiUtility::Savestate::ssBackupLoc& memInfo : locs) {
        if (!memInfo.data) {
            auto size = memInfo.endAddress - memInfo.startAddress;
            memInfo.data = static_cast<u8*>(Common::AllocateAlignedMemory(size, 64));
        }
    }
}

BrawlbackSavestate::BrawlbackSavestate(SavestateMemRegionInfo* regions, u32 numRegions)
{

    // init member list with proper addresses
    //initBackupLocs();
    UpdateDynamicMemRegionsForSavestate(regions, numRegions);

    // iterate through address ranges and allocate mem for our savestates
    AllocateMemForBackupLocs(backupLocs);
    AllocateMemForBackupLocs(dynamicLocs);

}

BrawlbackSavestate::~BrawlbackSavestate()
{
  for (auto it = backupLocs.begin(); it != backupLocs.end(); ++it)
  {
    Common::FreeAlignedMemory(it->data);
  }
  for (auto it = dynamicLocs.begin(); it != dynamicLocs.end(); ++it)
  {
    Common::FreeAlignedMemory(it->data);
  }
}


void BrawlbackSavestate::UpdateDynamicMemRegionsForSavestate(SavestateMemRegionInfo* regions, u32 numRegions) {
    if (!numRegions) return;

    std::vector<SlippiUtility::Savestate::ssBackupLoc> regionsToAppend = {};

    // process multiple regions at once
    if (numRegions > 1) {
        
        for (auto& loc : dynamicLocs) {
            Common::FreeAlignedMemory(loc.data);
        }
        dynamicLocs.clear();
        

        for (u32 i = 0; i < numRegions; i++) {
            const SavestateMemRegionInfo& info = regions[i];
            ssBackupLoc loc = {};
            loc.data = static_cast<u8*>(Common::AllocateAlignedMemory(info.size, 64));
            loc.startAddress = info.address;
            loc.endAddress = info.address + info.size;
            regionsToAppend.push_back(loc);
        }
    }
    // process a single new region
    else if (numRegions == 1) {
        const SavestateMemRegionInfo& memRegionInfo = regions[0];
        ssBackupLoc newMemRegion = {};
        newMemRegion.startAddress = memRegionInfo.address;
        newMemRegion.endAddress = memRegionInfo.address + memRegionInfo.size;
        // add region to map
        if (memRegionInfo.TAddFRemove) {
            // if we're adding it, need to alloc
            // if already exists in map
            if (memRegionMap.count(newMemRegion.startAddress)) {
                //WARN_LOG(BRAWLBACK, "Mem region already exists in map, overwriting with bigger sized alloc...\n");
                ssBackupLoc& oldMemRegion = memRegionMap[newMemRegion.startAddress];
                // if there's an alloc at a spot we already have one, 
                // and it's bigger than the previous, replace it with the bigger allocated block
                if (newMemRegion.endAddress > oldMemRegion.endAddress) {
                    newMemRegion.data = static_cast<u8*>(Common::AllocateAlignedMemory(memRegionInfo.size, 64));
                    oldMemRegion.endAddress = newMemRegion.endAddress;
                    free(oldMemRegion.data);
                    oldMemRegion = newMemRegion;
                }
            }
            // doesn't already exist, so just add it normally
            else {
                INFO_LOG(BRAWLBACK, "Adding new single allocation to savestave. addr = 0x%08x size = 0x%08x", memRegionInfo.address, memRegionInfo.size);
                newMemRegion.data = static_cast<u8*>(Common::AllocateAlignedMemory(memRegionInfo.size, 64));
                memRegionMap[newMemRegion.startAddress] = newMemRegion;
            }
            ExcludeSectionsFromMap(memRegionMap, MemRegions::excludeSections);
        }
        // remove region from map
        else {
            memRegionMap.erase(newMemRegion.startAddress);
        }
    }

    if (!regionsToAppend.empty()) {
        SlippiAppendBackupLocations(this->dynamicLocs, regionsToAppend, MemRegions::excludeSections);
        AllocateMemForBackupLocs(dynamicLocs);
    }

}


void BrawlbackSavestate::DisplaySavestateSize(const std::vector<SlippiUtility::Savestate::ssBackupLoc>& regions) {
    u64 totalsize = 0;
    for (const auto& loc : regions) {
        u32 size = loc.endAddress-loc.startAddress;
        double newsize = ((double)size / 1000.0) / 1000.0;
        if (!loc.data) {
            ERROR_LOG(BRAWLBACK, "[NO DATA] Savestate region: 0x%x - 0x%x : size %f mb   %s\n", loc.startAddress, loc.endAddress, newsize, loc.regionName.c_str());
        }
        else {
            INFO_LOG(BRAWLBACK, "Savestate region: 0x%x - 0x%x : size %f mb   %s\n", loc.startAddress, loc.endAddress, newsize, loc.regionName.c_str());
        }
        totalsize += size;
    }
    if (totalsize) {
        double dsize = ((double)totalsize / 1000.0) / 1000.0;
        std::string savestates_str = "Savestate total size: " + std::to_string(dsize) + " mb\n";
        OSD::AddTypedMessage(OSD::MessageType::Typeless, savestates_str, OSD::Duration::VERY_LONG, OSD::Color::GREEN);
        INFO_LOG(BRAWLBACK, "Savestate total size: %f mb\n", dsize);
    }
}

void BrawlbackSavestate::DisplaySavestateSize(const std::map<u32, ssBackupLoc>& regions) {
    u64 totalsize = 0;
    for (const auto& [k,v] : regions) {
        const auto& loc = v;
        u32 size = loc.endAddress-loc.startAddress;
        double newsize = ((double)size / 1000.0) / 1000.0;
        if (!loc.data) {
            ERROR_LOG(BRAWLBACK, "[NO DATA] Savestate region: 0x%x - 0x%x : size %f mb   %s\n", loc.startAddress, loc.endAddress, newsize, loc.regionName.c_str());
        }
        else {
            INFO_LOG(BRAWLBACK, "Savestate region: 0x%x - 0x%x : size %f mb   %s\n", loc.startAddress, loc.endAddress, newsize, loc.regionName.c_str());
        }
        totalsize += size;
    }
    if (totalsize) {
        double dsize = ((double)totalsize / 1000.0) / 1000.0;
        std::string savestates_str = "Savestate total size: " + std::to_string(dsize) + " mb\n";
        OSD::AddTypedMessage(OSD::MessageType::Typeless, savestates_str, OSD::Duration::VERY_LONG, OSD::Color::GREEN);
        INFO_LOG(BRAWLBACK, "Savestate total size: %f mb\n", dsize);
    }
}

void BrawlbackSavestate::DisplaySavestateSize() {
    DisplaySavestateSize(backupLocs);
    DisplaySavestateSize(dynamicLocs);
    DisplaySavestateSize(memRegionMap);
}



void BrawlbackSavestate::initBackupLocs()
{
    SlippiAppendBackupLocations(this->backupLocs, MemRegions::memRegions, MemRegions::excludeSections);
}


void captureMemRegions(const backupLocList& locs) {
    for (auto it = locs.begin(); it != locs.end(); ++it) {
        auto size = it->endAddress - it->startAddress;
        if (size && it->data)
            Memory::CopyFromEmu(it->data, it->startAddress, size);  // game -> emu
        else
            ERROR_LOG(BRAWLBACK, "Invalid size/data ptr in savestate! [%x, %x]\n", it->startAddress, it->endAddress);
    }
}
void loadMemRegions(const backupLocList& locs) {
    for (auto it = locs.begin(); it != locs.end(); ++it) {
        auto size = it->endAddress - it->startAddress;
        if (size && it->data)
            Memory::CopyToEmu(it->startAddress, it->data, size);  // emu -> game
        else
            ERROR_LOG(BRAWLBACK, "Invalid size/data ptr in savestate! [%x, %x]\n", it->startAddress, it->endAddress);
    }
}

void BrawlbackSavestate::Capture()
{
    #if 1
    captureMemRegions(backupLocs);
    captureMemRegions(dynamicLocs);
    #endif

    #if 1
    for (auto it = memRegionMap.begin(); it != memRegionMap.end(); ++it) {
        ssBackupLoc info = it->second;
        auto size = info.endAddress - info.startAddress;
        if (size && info.data)
            Memory::CopyFromEmu(info.data, info.startAddress, size);  // emu -> game
        else
            ERROR_LOG(BRAWLBACK, "Invalid size/data ptr in savestate! [%x, %x]\n", info.startAddress, info.endAddress);
    }
    #endif
}

void BrawlbackSavestate::Load(std::vector<PreserveBlock> blocks)
{
    // Back up regions of game that should stay the same between savestates

    /*for (auto it = blocks.begin(); it != blocks.end(); ++it)
    {
      if (!preservationMap.count(*it))  // if this PreserveBlock is NOT in our preservationMap
      {
        // TODO: Clear preservation map when game ends
        preservationMap[*it] =
            std::vector<u8>(it->length);  // init new entry at this PreserveBlock key
      }

      Memory::CopyFromEmu(&preservationMap[*it][0], it->address, it->length);
    }*/

    // Restore memory blocks
    #if 1
    loadMemRegions(backupLocs);
    loadMemRegions(dynamicLocs);
    #endif

    #if 1
    for (auto it = memRegionMap.begin(); it != memRegionMap.end(); ++it) {
        ssBackupLoc info = it->second;
        auto size = info.endAddress - info.startAddress;
        if (size && info.data)
            Memory::CopyToEmu(info.startAddress, info.data, size);  // emu -> game
        else
            ERROR_LOG(BRAWLBACK, "Invalid size/data ptr in savestate! [%x, %x]\n", info.startAddress, info.endAddress);
    }
    #endif

    // Restore preservation blocks
    /*for (auto it = blocks.begin(); it != blocks.end(); ++it)
    {
        Memory::CopyToEmu(it->address, &preservationMap[*it][0], it->length);
    }*/
  
}
