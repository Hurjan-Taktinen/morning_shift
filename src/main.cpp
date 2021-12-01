#include "config/config.h"
#include "logs/log.h"
#include "mc/client.h"

#include <string>
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

    std::string address{"127.0.0.1"};
    int port = 25565;
    std::string name{"Komentaja"};

    mc::Client client{address, port, name};
    client.start();
    client.join();

    return 0;
}
