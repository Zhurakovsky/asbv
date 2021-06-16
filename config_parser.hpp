#ifndef POC_EXAMPLE_CONFIG_PARSER_HPP
#define POC_EXAMPLE_CONFIG_PARSER_HPP

#include <map>
#include <vector>
#include <string>
#include "common/include/types.hpp"

namespace poc_autosar
{
enum config_type
{
    CONFIG_SENSOR,
    CONFIG_PID,
    CONFIG_ACTUATOR,
    CONFIG_COMMON
};

class ConfigParser
{
public:
    ConfigParser(std::string config_file_path);
    err_t print_parsed_config();
    err_t parse_config();
    std::vector<std::string> get_sensor_config_line();
    std::vector<std::string> get_pid_config_line();
    std::vector<std::string> get_actuator_config_line();
private:
    std::map<config_type, std::map<std::string, std::string>> parsed_config;
    std::string config_file_path;
    bool is_config_parsed;
    std::vector<std::string> get_config_line(config_type type);
};

}

#endif /* POC_EXAMPLE_CONFIG_PARSER_HPP */
