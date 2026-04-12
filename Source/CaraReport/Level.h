#pragma once

#include <CaraReport/API.h>
#include <cstdint>
#include <string>

namespace CaraReport
{
    enum class Level : std::uint8_t
    {
        Error,
        Warning,
        Info
    };

    [[nodiscard]] CARAREPORT_API std::string stringify(Level level);
}
