#include "config/config.h"
#include "logs/log.h"

#include <string_view>

int main(int argc, const char** argv)
{
    logs::Log::init();
    LGINFO("HurjanTaktinen presents ... morning_shift version ({}.{}.{})", 0, 1, 0);

    std::string_view optionalConfigPath;
    if(argc > 1)
    {
        optionalConfigPath = argv[1];
    }

    config::Config::init(optionalConfigPath);
}
