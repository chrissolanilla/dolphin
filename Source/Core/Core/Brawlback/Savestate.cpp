#include "Savestate.h"

#include <Common/MemoryUtil.h>
#include <Core/HW/EXI/EXI.h>
#include <Core/HW/Memmap.h>
#include <thread>
#include "Common/Thread.h"
#include "Common/Timer.h"
#include "VideoCommon/OnScreenDisplay.h"
#include "Common/Logging/Log.h"
#include "MemRegions.h"
#include <sstream>
#define LOW_BOUND_MEM 0x80000000

// lots of code here is heavily derived from Slippi's Savestates.cpp

BrawlbackSavestate::BrawlbackSavestate(std::vector<SlippiUtility::Savestate::ssBackupLoc> staticRegions)
{
  // init member list with proper addresses
  initStaticLocs(staticRegions);
  // iterate through address ranges and allocate mem for our savestates
  for (auto it = staticLocs.begin(); it != staticLocs.end(); ++it)
  {
    auto size = it->endAddress - it->startAddress;
    it->data = static_cast<u8*>(Common::AllocateAlignedMemory(size, 64));
  }
}

BrawlbackSavestate::~BrawlbackSavestate()
{
  for (auto it = staticLocs.begin(); it != staticLocs.end(); ++it)
  {
    Common::FreeAlignedMemory(it->data);
  }
}

void BrawlbackSavestate::getDolphinState(PointerWrap& p)
{
  // p.DoArray(Memory::m_pRAM, Memory::RAM_SIZE);
  // p.DoMarker("Memory");
  // VideoInterface::DoState(p);
  // p.DoMarker("VideoInterface");
  // SerialInterface::DoState(p);
  // p.DoMarker("SerialInterface");
  // ProcessorInterface::DoState(p);
  // p.DoMarker("ProcessorInterface");
  // DSP::DoState(p);
  // p.DoMarker("DSP");
  // DVDInterface::DoState(p);
  // p.DoMarker("DVDInterface");
  // GPFifo::DoState(p);
  // p.DoMarker("GPFifo");
  ExpansionInterface::DoState(p);
  p.DoMarker("ExpansionInterface");
  // AudioInterface::DoState(p);
  // p.DoMarker("AudioInterface");
}

void BrawlbackSavestate::initStaticLocs(std::vector<SlippiUtility::Savestate::ssBackupLoc> staticRegions)
{
    staticLocs.clear();
    staticLocs.insert(staticLocs.begin(), staticRegions.begin(), staticRegions.end());
}
void BrawlbackSavestate::initDynamicLocs(std::vector<SlippiUtility::Savestate::ssBackupLoc> dynamicRegions)
{
    dynamicLocs.clear();
    dynamicLocs.insert(dynamicLocs.begin(), dynamicRegions.begin(), dynamicRegions.end());
}

typedef std::vector<SlippiUtility::Savestate::ssBackupLoc>::iterator backupLocIterator;

void captureMemRegions(backupLocIterator start, backupLocIterator end) {
    for (auto it = start; it != end; ++it) {
        auto size = it->endAddress - it->startAddress;
        Memory::CopyFromEmu(it->data, it->startAddress, size);  // game -> emu
    }
}

void BrawlbackSavestate::Capture(std::vector<SlippiUtility::Savestate::ssBackupLoc> dynamicRegions)
{
    initDynamicLocs(dynamicRegions);
    captureMemRegions(staticLocs.begin(), staticLocs.end());
    captureMemRegions(dynamicLocs.begin(), dynamicLocs.end());
    u64 totalsize = 0;
    for (auto& loc : staticLocs)
    {
        totalsize += loc.endAddress - loc.startAddress;
    }
    for (auto& loc : dynamicLocs)
    {
      totalsize += loc.endAddress - loc.startAddress;
    }
    double dsize = ((double)totalsize / 1000000.0);
    INFO_LOG_FMT(BRAWLBACK, "backupLocs total size: {:.13} mb\n", dsize);
}

void BrawlbackSavestate::Load(std::vector<PreserveBlock> blocks)
{
    // Restore memory blocks
    for (auto it = staticLocs.begin(); it != staticLocs.end(); ++it)
    {
      auto size = it->endAddress - it->startAddress;
      Memory::CopyToEmu(it->startAddress, it->data, size);
    }
    for (auto it = dynamicLocs.begin(); it != dynamicLocs.end(); ++it)
    {
      auto size = it->endAddress - it->startAddress;
      Memory::CopyToEmu(it->startAddress, it->data, size);
    }
}
