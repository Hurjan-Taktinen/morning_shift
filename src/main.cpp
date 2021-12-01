#include "config/config.h"
#include "logs/log.h"
#include "mc/client.h"

#include <iostream>
#include <string>
#include <string_view>
#include <utility>

#include <boost/program_options.hpp>

struct ProgramConf
{
    std::string configpath;
    int numClients = 0;
};

static auto parse(int argc, const char** argv)
{
    namespace po = boost::program_options;
    ProgramConf conf;

    // Options only from command line
    po::options_description generic("Example:\n"
                                    "./morning_shift -c configpath -n numClients\n"
                                    "Command line options");
    generic.add_options()("help,h", "Produce this help");

    po::options_description config("Configuration");

    // clang-format off
    config.add_options()
        ("config,c", po::value<std::string>(&conf.configpath)->default_value(""), "Specify optional config file path")
        ("num-clients,n", po::value<int>(&conf.numClients)->default_value(1), "Spawn n clients");
    // clang-format on

    // Options read from command line
    po::options_description cmdline("");
    cmdline.add(generic).add(config);

    // Parse command line
    po::variables_map vm;
    po::store(parse_command_line(argc, argv, cmdline), vm);
    po::notify(vm);

    // Extract help
    if(vm.count("help"))
    {
        std::cout << cmdline << std::endl;
        return std::make_tuple(true, conf);
    }

    return std::make_tuple(false, conf);
}

int main(int argc, const char** argv)
{
    const auto [exit, startupConf] = parse(argc, argv);
    if(exit)
    {
        return EXIT_SUCCESS;
    }

    logs::Log::init();
    LGINFO("HurjanTaktinen presents ... morning_shift version ({}.{}.{})", 0, 1, 0);

    config::Config::init(startupConf.configpath);

    std::string address{"127.0.0.1"};
    int port = 25565;
    std::string name{"Komentaja"};

    mc::Client client{address, port, name};
    client.start();
    client.join();

    return EXIT_SUCCESS;
}
