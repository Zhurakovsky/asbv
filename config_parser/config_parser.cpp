#include "config_parser.hpp"
#include <iostream>
#include <fstream>

using namespace std;
using namespace poc_autosar;

namespace
{

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string &s)
{
    return rtrim(ltrim(s));
}

}

ConfigParser::ConfigParser(std::string config_file_path)
{
    this->config_file_path = config_file_path;
    is_config_parsed = false;
}

err_t ConfigParser::print_parsed_config()
{
    cout << "Common configuration" << endl;
    for (const auto& [key, val] : this->parsed_config[CONFIG_COMMON])
        cout << key << " | " << val << endl;

    cout << endl << "Sensor configuration" << endl;
    for (const auto& [key, val] : this->parsed_config[CONFIG_SENSOR])
        cout << key << " | " << val << endl;

    cout << endl << "Pid configuration" << endl;
    for (const auto& [key, val] : this->parsed_config[CONFIG_PID])
        cout << key << " | " << val << endl;

    cout << endl << "Actuator configuration" << endl;
    for (const auto& [key, val] : this->parsed_config[CONFIG_ACTUATOR])
        cout << key << " | " << val << endl << endl;

    return RC_SUCCESS;
}

err_t ConfigParser::parse_config()
{
    string line;
    map<string, string> *current_secton;
    ifstream configfile(this->config_file_path);

    while (getline(configfile, line))
    {
        if (line.length() <= 1)
            continue;
        if (line[0] == '#')
            continue;
        else if (line.find("[common]") != string::npos)
            current_secton = &(this->parsed_config[CONFIG_COMMON]);
        else if (line.find("[sensor]") != string::npos)
            current_secton = &(this->parsed_config[CONFIG_SENSOR]);
        else if (line.find("[actuator]") != string::npos)
            current_secton = &(this->parsed_config[CONFIG_ACTUATOR]);
        else if (line.find("[pid]") != string::npos)
            current_secton = &(this->parsed_config[CONFIG_PID]);
        else
        {
            if (!current_secton)
                return RC_FAIL;
            string key = trim(line.substr(0, line.find("=")));
            string value = trim(line.substr(line.find("=") + 1));
            current_secton->insert({key, value});
        }
  }
    this->is_config_parsed = true;
    return RC_SUCCESS;
}

std::vector<std::string> ConfigParser::get_config_line(config_type type)
{
    std::vector<std::string> ret;
    if (!this->is_config_parsed)
        return ret;


    for (const auto& [key, val] : this->parsed_config[type])
    {
        if (val == "y")
            ret.push_back("+" + key);
        else
            ret.push_back(key + "=" + val);
    }

    return ret;
}

std::vector<std::string> ConfigParser::get_sensor_config_line()
{
    return get_config_line(CONFIG_SENSOR);
}
std::vector<std::string> ConfigParser::get_pid_config_line()
{
    return get_config_line(CONFIG_PID);
}

std::vector<std::string> ConfigParser::get_actuator_config_line()
{
    return get_config_line(CONFIG_ACTUATOR);
}
