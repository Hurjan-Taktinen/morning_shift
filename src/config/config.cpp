#include "config/config.h"

#include <filesystem>
#include <spdlog/fmt/ostr.h>
#include <string_view>
#include <charconv>

namespace config
{

void Config::init(std::string_view optionalConfigPath)
{
    m_Log = logs::Log::create("Config");

    if(!optionalConfigPath.empty()
       && std::filesystem::exists(optionalConfigPath))
    {
        m_Log->info("Optional config file given ({})", optionalConfigPath);
        m_ConfigFilePath = optionalConfigPath;
    }

    mINI::INIFile file(m_ConfigFilePath);
    if(!file.read(m_IniStruct))
    {
        m_Log->warn("Failed to open config file, generating one with "
                    "defaults");
    }

    auto fromchars = [](const std::string& src, auto& dst) {
        const auto last = src.data() + src.size();
        std::from_chars(src.data(), last, dst);
    };

    m_Log->info("Configuration file found at {}", m_ConfigFilePath.size());
    m_Log->info("Found {} keys, parsing...", m_IniStruct.size());

    { // BaseConfig
        ConnectionConfig config;
        if(m_IniStruct.has("connection"))
        {
            auto section = m_IniStruct.get("connection");
            config.server_ip = section.get("server_ip");
            fromchars(section.get("server_port"), config.server_port);
        }
        else
        {
            // use default values given in struct declarations
        }

        m_Log->info("connection::server_ip {}", config.server_ip);
        m_Log->info("connection::server_port {}", config.server_port);

        m_ConnectionConfig = config;
    }
}

} // namespace config
