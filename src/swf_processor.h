
#include "swf.h"

#include <vector>

struct SWDInfo;

void ProcessSWF(const SWDInfo& swdInfo, const uint8_t* src, size_t length, std::vector<uint8_t>& dst);

