#include "FZXLog/FZXLog.h"

int main()
{
    FZXLog::initialize();
    FZXLog::setPattern(FZXLOG_FMT_PATTERN_FULL);
    FZXLog::info(FZXLOG_LOCATION(__FUNCTION__), "FZXLog initialized successfully.");
    FZXLog::shutdown();
    return 0;
}


