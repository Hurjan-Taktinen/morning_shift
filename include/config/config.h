#pragma once

#include "mINI/ini.h"
#include "logs/log.h"

#include <string>
#include <optional>

namespace config
{

struct ClientConfig
{
    std::string configpath;
    int numClients = 0;
    std::optional<std::string> server_ip;
    std::optional<int> server_port = 0;
};

struct ConnectionConfig
{
    std::string server_ip;
    int server_port = 0;
};

class Config final
{
public:
    static void init(std::string_view optionalConfigPath);

    [[nodiscard]] static auto getConnectionConfig()
    {
        return m_ConnectionConfig;
    }

private:
    inline static logs::Logger m_Log;
    inline static std::string m_ConfigFilePath = "data/settings.ini";
    inline static mINI::INIStructure m_IniStruct;

    inline static ConnectionConfig m_ConnectionConfig;
};

} // namespace config
