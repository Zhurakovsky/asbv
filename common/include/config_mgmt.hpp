#ifndef POC_EXAMPLE_COMMON_CONFIG_MGMT_HPP
#define POC_EXAMPLE_COMMON_CONFIG_MGMT_HPP

#include <string>
#include <typeinfo>
#include <list>
#include <algorithm>

namespace poc_autosar
{

class CmdLineArgsParser
{
public:
    CmdLineArgsParser(int argc, char** argv)
    {
        for (int i = 0; i < argc; i++)
            arguments.push_back(argv[i]);
    }

    template<typename T>
    T config_get(std::string requested_arg)
    {
        if constexpr (std::is_same_v<T, bool>)
        {
            return (std::find(arguments.begin(), arguments.end(), requested_arg.insert(0, "+")) != arguments.end());
        }
        else if constexpr (std::is_same_v<T, int>)
        {
            auto match = std::find_if(arguments.cbegin(), arguments.cend(), [&requested_arg] (const std::string& s) {
                return s.find(requested_arg) == 0;
            });
            if (match != arguments.cend())
            {
                return  std::atoi(getVal(*match).c_str());
            }
        }
        else if constexpr (std::is_same_v<T, std::string>)
        {
            auto match = std::find_if(arguments.cbegin(), arguments.cend(), [&requested_arg] (const std::string& s) {
                return s.find(requested_arg) == 0;
            });
            if (match != arguments.cend())
            {
                return getVal(*match);
            }
        }
        else if constexpr (std::is_same_v<T, float>)
        {
            auto match = std::find_if(arguments.cbegin(), arguments.cend(), [&requested_arg] (const std::string& s) {
                return s.find(requested_arg) == 0;
            });
            if (match != arguments.cend())
            {
                return std::stof(getVal(*match));
            }
        }
        /* Below code shouldn't be executed. Adde Only to avoid warninigs */
       return  T();
    }

private:
    std::list<std::string> arguments;
    std::string getVal(std::string cfgLine)
    {
        return cfgLine.substr(cfgLine.find("=") + 1);
    }
};

};

#endif // POC_EXAMPLE_COMMON_CONFIG_MGMT_HPP
