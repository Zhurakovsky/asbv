#ifndef POC_EXAMPLE_LOG_MANAGER_HPP
#define POC_EXAMPLE_LOG_MANAGER_HPP

#include "string"
#include "types.hpp"

namespace poc_autosar
{

class LogManager
{
public:
    static void Start();

    static bool Create(const LogConfig& config);
    static void Log(const std::string& loggerName, const std::string& message);
};

}

#endif /* POC_EXAMPLE_CONFIG_PARSER_HPP */
