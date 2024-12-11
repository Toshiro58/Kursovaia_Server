#include "Interface.h"
#include "Connector_to_base.h"
#include "Client_Communicate.h"
#include "Errors.h"
#include "Logger.h"
#include <boost/program_options.hpp>
#include <iostream>
#include <string>

namespace po = boost::program_options;

int Interface::comm_proc(int argc, const char** argv)
{
    struct Config {
        int port = 33333;
        std::string logfile = "/home/stud/local_git/Kursovaia_Server/test/log.txt";
        std::string basefile = "/home/stud/local_git/Kursovaia_Server/test/base.txt";
    } config;

    try {
        po::options_description opts("Parametrs for using the server");
        opts.add_options()
            ("help,h", "Usage help infirmation")
            ("basefile,b", po::value<std::string>(&config.basefile), "Path to database file")
            ("logfile,l", po::value<std::string>(&config.logfile), "Path to log file")
            ("PORT,p", po::value<int>(&config.port), "Port for server");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, opts), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << opts << "\n";
            return 0;
        }

        if (config.port < 1024 || config.port > 65535) {
            throw crit_err("Incorrect port");
        }

        Logger logger(config.logfile);
        logger.writelog("Log file path set to: " + config.logfile);
        logger.writelog("Database file path set to: " + config.basefile);
        logger.writelog("Port set to: " + std::to_string(config.port));

        Connector_to_base connector;
        connector.connect_to_base(config.basefile);
        logger.writelog("Connected to database is ok!");
        logger.writelog("Server started");

        Client_Communicate clientComm;
        clientComm.connection(config.port, connector.get_data(), &logger);
    } catch (const crit_err& e) {
        std::cerr << "Critical error: " << e.what() << "\n";
    } catch (const po::error& e) {
        std::cerr << "Error: " << e.what() << "\nUse -h for help\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\nUse -h for help\n";
    } catch (...) {
        std::cerr << "Unknown error!\nUse -h for help\n";
        std::terminate();
    }
    return 0;
}
