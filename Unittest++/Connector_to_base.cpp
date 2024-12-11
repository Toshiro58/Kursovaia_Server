#include "Connector_to_base.h"
#include "Errors.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sys/stat.h>

int Connector_to_base::connect_to_base(std::string base_file)
{
    // Проверка существования файла
    std::ifstream file_check(base_file);
    if (!file_check) {
        std::cerr << "Error: File " << base_file << " does not exist." << std::endl;
        throw crit_err("File does not exist");
    }
    file_check.close();

    // Установка прав доступа к файлу базы данных (чтение для всех)
    if (chmod(base_file.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) != 0) {
        std::cerr << "Error: Unable to set file permissions for " << base_file << std::endl;
        throw crit_err("Unable to set file permissions");
    }

    if (base_file.find('.') == std::string::npos) {
        throw crit_err("invalid_base_path");
    }

    std::ifstream file_read(base_file);
    if (!file_read.is_open()) {
        std::cerr << "Error: Unable to open file " << base_file << std::endl;
        throw crit_err("Unable to open file");
    }

    std::string line;
    std::string temp_login;
    std::string temp_pass;

    while (std::getline(file_read, line)) {
        size_t delimiter_pos = line.find(':');
        if (delimiter_pos != std::string::npos) {
            temp_login = line.substr(0, delimiter_pos);
            temp_pass = line.substr(delimiter_pos + 1);
            data_base[temp_login] = temp_pass;
        } else {
            std::cerr << "Warning: Invalid line format in database file: " << line << std::endl;
        }
    }

    file_read.close();

    if (data_base.empty()) {
        throw crit_err("Empty database");
    }

    std::cout << "Database loaded successfully. " << data_base.size() << " entries found." << std::endl;
    return 0;
}

std::map<std::string, std::string> Connector_to_base::get_data()
{
    return data_base;
}
