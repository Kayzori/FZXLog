#pragma once

#include "BaseSink.h"
#include "RotationFileSink.h"
#include "ConsoleSink.h"

// Provide convenient aliases (default to thread-safe implementations)
namespace FZXLog::Sink {
    using RotationFileSink = RotationFileSink_mt;
    using ConsoleSink = ConsoleSink_mt;
}

