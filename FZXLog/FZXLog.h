#pragma once

#include "FZXLog/Fmt/Formatter.h"
#include "FZXLog/Fmt/PatternFormatter.h"

#include "FZXLog/Sink/Sink.h"
#include "FZXLog/Sink/ConsoleSink.h"
#include "FZXLog/Sink/RotationFileSink.h"

#include "FZXLog/Logger/SyncLogger.h"
#include "FZXLog/Logger/AsyncLogger.h"

#include <memory>

// Aliases

namespace FZXLog::Sink {

using ConsoleSink = ConsoleSink_mt;
using RotationFileSink = RotationFileSink_mt;

} // namespace FZXLog::Sink
