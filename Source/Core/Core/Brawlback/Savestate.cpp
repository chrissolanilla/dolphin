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
    else {
        const SavestateMemRegionInfo& info = regions[0];
        ssBackupLoc loc = {};
        loc.data = static_cast<u8*>(Common::AllocateAlignedMemory(info.size, 64));
        loc.startAddress = info.address;
        loc.endAddress = info.address + info.size;
        // add region to map
        if (info.TAddFRemove) {
            memRegionMap[loc.startAddress] = loc;
        }
        // remove region from map
        else {
            // already exists in map
            if (memRegionMap.count(loc.startAddress)) {
                WARN_LOG(BRAWLBACK, "Mem region already exists in map, overwriting with bigger sized alloc...\n");
                // use the bigger of the two
                memRegionMap[loc.startAddress].startAddress = 
                        Brawlback::MAX(loc.startAddress, memRegionMap[loc.startAddress].startAddress);
            }
            else {
                memRegionMap.erase(loc.startAddress);
            }
        }
        regionsToAppend.push_back(loc);
    }

    SlippiAppendBackupLocations(this->dynamicLocs, regionsToAppend, MemRegions::excludeSections);
    AllocateMemForBackupLocs(dynamicLocs);
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

void BrawlbackSavestate::DisplaySavestateSize() {
    DisplaySavestateSize(backupLocs);
    DisplaySavestateSize(dynamicLocs);
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
    captureMemRegions(backupLocs);
    captureMemRegions(dynamicLocs);
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
    loadMemRegions(backupLocs);
    loadMemRegions(dynamicLocs);

    // Restore preservation blocks
    /*for (auto it = blocks.begin(); it != blocks.end(); ++it)
    {
        Memory::CopyToEmu(it->address, &preservationMap[*it][0], it->length);
    }*/
  
}
