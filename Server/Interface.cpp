#include "Interface.h"
#include "Connector_to_base.h"
#include "Client_Communicate.h"
#include "Errors.h"
#include "Logger.h"
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <fstream>

namespace po = boost::program_options;

int Interface::comm_proc(int argc, const char** argv) {
    struct Config {
        int port = 33333;  
        std::string logfile = "/etc/vcalc.log";
        std::string basefile = "/var/log/vcalc.log";
    } config;

    try {
        po::options_description opts("Parameters for using the server");
        opts.add_options()
            ("help,h", "Usage help information")
            ("basefile,b", po::value<std::string>(&config.basefile)->default_value(config.basefile), "Path to database file (default: /var/log/vcalc.log)")
            ("logfile,l", po::value<std::string>(&config.logfile)->default_value(config.logfile), "Path to log file (default: /etc/vcalc.log)")
            ("PORT,p", po::value<int>(&config.port)->default_value(config.port), "Port for server (default: 33333)");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, opts), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << opts << "\n";
            return 0;
        }

        if (config.port < 1024 || config.port > 65535) {
            throw crit_err("Port must be between 1024 and 65535");
        }

        std::ofstream file;
        file.open(config.logfile, std::ios::app);

        if (!file.is_open()) {
            std::cerr << "Cannot write to default log path. Using /tmp/vcalc.log instead.\n";
            config.logfile = "/tmp/vcalc.log";
            file.open(config.logfile, std::ios::app);
            if (!file.is_open()) {
                throw crit_err("Unable to create log file");
            }
        }
        file.close();

        file.open(config.basefile, std::ios::app);
        if (!file.is_open()) {
            std::cerr << "Cannot write to default database path. Using /tmp/vcalc-db.log instead.\n";
            config.basefile = "/tmp/vcalc-db.log";
            file.open(config.basefile, std::ios::app);
            if (!file.is_open()) {
                throw crit_err("Unable to create database file");
            }
        }
        file.close();

        Logger logger(config.logfile);
        logger.writelog("Log file path set to: " + config.logfile);
        logger.writelog("Database file path set to: " + config.basefile);
        logger.writelog("Port set to: " + std::to_string(config.port));

        Connector_to_base connector;
        connector.connect_to_base(config.basefile);
        logger.writelog("Connected to database successfully!");
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
