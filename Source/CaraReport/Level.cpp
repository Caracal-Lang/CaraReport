#include <CaraReport/Level.h>

namespace CaraReport
{
    std::string stringify(Level level)
    {
        switch (level)
        {
        case Level::Error:
            return "Error";
        case Level::Warning:
            return "Warning";
        case Level::Info:
            return "Info";
        }
        return {};
    }
}
