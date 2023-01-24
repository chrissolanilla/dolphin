#pragma once


#include "SlippiUtility.h"

// https://docs.google.com/spreadsheets/d/1xVvcsGZg930uVhIawacDp-brbNpLJQtzj3ry-ZQaXWo/edit?usp=sharing

namespace MemRegions {

extern std::vector<PreserveBlock> excludeSections;

extern std::vector<SlippiUtility::Savestate::ssBackupLoc> memRegions;


} // namespace MemRegions
