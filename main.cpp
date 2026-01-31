#include "FZXLog/FZXLog.h"

#include <iostream>

int main()
{
    std::shared_ptr<FZXLog::FMT::DefaultFormatter> dfmt = std::make_shared<FZXLog::FMT::DefaultFormatter>(
        FZXLOG_FMT_COLOR | FZXLOG_FMT_LEVEL | FZXLOG_FMT_DATE | FZXLOG_FMT_TIME
    );

    FZXLog::Sink::RotationFileSink file = FZXLog::Sink::RotationFileSink("../../test.log", dfmt);
    FZXLog::Sink::ConsoleSink console = FZXLog::Sink::ConsoleSink(dfmt);

    // File logs
    file.log(FZXLog::Level::Trace, "Hello, World!");  
    file.log(FZXLog::Level::Debug, "Hello, World!");  
    file.log(FZXLog::Level::Info, "Hello, World!");   
    file.log(FZXLog::Level::Success, "Hello, World!");
    file.log(FZXLog::Level::Warning, "Hello, World!");
    file.log(FZXLog::Level::Error, "Hello, World!");  
    file.log(FZXLog::Level::Fatal, "Hello, World!");  

    // Consol logs
    console.log(FZXLog::Level::Trace, "Hello, World!");
    console.log(FZXLog::Level::Debug, "Hello, World!");
    console.log(FZXLog::Level::Info, "Hello, World!");
    console.log(FZXLog::Level::Success, "Hello, World!"); 
    console.log(FZXLog::Level::Warning, "Hello, World!"); 
    console.log(FZXLog::Level::Error, "Hello, World!");
    console.log(FZXLog::Level::Fatal, "Hello, World!");

    char y;
    std::cin >> y;

    return 0;
}
