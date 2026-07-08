# FZXLog

FZXLog is a small C++ logging library made for simple projects that need clean, readable logs without depending on a large framework. It is meant to be easy to understand, easy to plug into an application, and useful for both debugging and day-to-day development.

This project was built as a personal library for learning and practice, but it is still practical enough to use in small tools, test programs, and beginner-to-intermediate C++ applications.

## Why this library exists

Many logging libraries feel heavy or complicated for small programs. FZXLog tries to keep things simple:

- It uses standard C++ features.
- It supports different log levels so you can control how much output you want.
- It can send logs to the console or to files.
- It lets you change the output format with a simple formatter.
- It is easy to add to a project with CMake.

## Main features

- Log levels: Trace, Debug, Info, Warning, Error, Fatal, and Off
- Console sink for printing logs to the terminal
- Rotation file sink for writing logs to files with size-based rotation
- Pattern-based formatting for timestamps, levels, file names, function names, and messages
- Simple logger API with helper methods such as trace, debug, info, warning, error, and fatal
- Thread-safe console and file sinks
- A small log trace buffer that stores recent messages

## How it works

The library is built around three ideas:

1. Logger
   - This is the main object used to send log messages.
   - You can choose a level and decide which sinks should receive the messages.

2. Sink
   - A sink is the destination for logs.
   - The current library supports console output and file output.

3. Formatter
   - A formatter controls how a log message looks.
   - The built-in pattern formatter lets you define your own output format.

## Building the library

This project uses CMake.

From the project root, run:

```bash
cmake -S . -B build
cmake --build build
```

If you are using Visual Studio on Windows, the generated solution file can also be used from the build folder.

## Basic usage

Here is a simple example that writes logs to the console:

```cpp
#include "FZXLog/FZXLog.h"
#include <memory>

int main() {
    using namespace FZXLog;

    auto formatter = std::make_shared<Fmt::PatternFormatter>(FZXLOG_FMT_PATTERN_BASIC);
    auto consoleSink = std::make_shared<Sink::ConsoleSink>(formatter, Level::Trace);

    auto logger = std::make_shared<Logger::SyncLogger>(Level::Trace);
    logger->addSink(consoleSink);

    logger->info("Application started");
    logger->warning("This is a warning message");
    logger->error("Something went wrong");

    logger->flush();
    return 0;
}
```

## Writing to a file

You can also send logs to a file with the rotation file sink:

```cpp
#include "FZXLog/FZXLog.h"
#include <memory>

int main() {
    using namespace FZXLog;

    auto formatter = std::make_shared<Fmt::PatternFormatter>(FZXLOG_FMT_PATTERN_FULL);
    auto fileSink = std::make_shared<Sink::RotationFileSink>(
        "app.log",
        formatter,
        Level::Trace,
        Level::Error,
        1024 * 1024
    );

    auto logger = std::make_shared<Logger::SyncLogger>(Level::Trace);
    logger->addSink(fileSink);

    logger->info("File logging enabled");
    logger->fatal("A fatal issue happened");

    logger->flush();
    return 0;
}
```

## Log levels

The library uses these levels in order:

- Trace
- Debug
- Info
- Warning
- Error
- Fatal
- Off

A logger will only print messages that are equal to or above its current level.

## Pattern formatter options

The built-in pattern formatter supports tokens such as:

- %y: year
- %m: month
- %d: day
- %H: hour
- %M: minute
- %S: second
- %e: milliseconds
- %l: log level name
- %t: thread id
- %s: source file
- #: line number
- !: function name
- %v: log message

Example pattern:

```cpp
#define FZXLOG_FMT_PATTERN_ADVENCED "[%y-%m-%d %H:%M:%S.%e] [%l] [thread: %t] - %v"
```

## Notes about the async logger

There is also an async logger class in the project, but it is marked as deprecated in the source. The sync logger is the safer and simpler default for most use cases.

## When to use FZXLog

FZXLog is a good fit when you want:

- a small logging tool for a C++ project
- fast setup without a lot of configuration
- console logging while developing
- file logging for later inspection
- a library that is still easy to read and change

## License

This project is distributed under the MIT license. See the LICENSE file for details.
