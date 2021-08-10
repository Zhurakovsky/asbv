#include "log_manager.hpp"

using namespace poc_autosar;
using namespace quill;

void LogManager::Start()
{
    enable_console_colours();
    start();
}

bool LogManager::Create(const LogConfig& config)
{
    bool result = false;

    if (!config.name.empty())
    {
        std::vector<Handler*> handlers;

        if (config.use_console)
        {
            std::string hStdoutName = config.name + "Stdout";
            ConsoleColours consoleColours;
            consoleColours.set_default_colours();

            if (!config.console_colour_foreground.empty())
            {
                consoleColours.set_colour(LogLevel::Info, "\033[" + config.console_colour_foreground);
            }

            handlers.push_back(stdout_handler(hStdoutName, consoleColours));
        }

        if (config.use_file)
        {
            std::string filename = config.name + ".log";
            
            if (!config.custom_filename.empty())
            {
                filename = config.custom_filename + ".log";
            }

            handlers.push_back(file_handler(filename, "a"));
        }

        if (!handlers.empty())
        {
            create_logger(config.name.c_str(), handlers);
            result = true;
        }
    }

    return result;
}
