#ifndef POC_EXAMPLE_LOG_MANAGER_HPP
#define POC_EXAMPLE_LOG_MANAGER_HPP

#include "string"
#include "types.hpp"
#include "quill/detail/LogMacros.h"

#define LOG(loggerName, message) LOG_INFO_BY_NAME(loggerName, "{}", message);

namespace poc_autosar
{

class LogManager
{
public:
    static void Start();
    static bool Create(const LogConfig& config);
};

}

#endif /* POC_EXAMPLE_CONFIG_PARSER_HPP */
