
#include "swf.h"
#include "IActionProcessor.h"

#include <vector>

struct SWDInfo;

void ProcessSWF(IActionProcessor& actionProcessor, const uint8_t* src, size_t length, std::vector<uint8_t>& dst);

