#include "FZXLog/FZXLog.h"

int main()
{
    FZXLog::initialize();
    FZXLog::setPattern(FZXLOG_FMT_PATTERN_FULL);
    FZXLog::info(FZXLOG_LOCATION(__FUNCTION__), "FZXLog initialized successfully.");
    FZXLog::success(FZXLOG_LOCATION(__FUNCTION__), "This is a success message with number: {}", 42);
    FZXLog::shutdown();
    return 0;
}


