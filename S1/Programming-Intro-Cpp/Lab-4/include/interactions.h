#pragma once

#include <regex>
#include <string>

#include "config.h"

namespace fs = std::filesystem;

namespace interactions {
    void create(const Config& config);

    void extract(const Config& config);

    void append(const Config& config);

    void remove(const Config& config);

    void concat(const Config& config);

    void list(const Config& config);

    void resolve_error(const std::string& message);
}
