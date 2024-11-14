#pragma once
#include <map>
#include <string>

class Connector_to_base {
private:
    std::map<std::string, std::string> data_base;

public:
    int connect_to_base(std::string base_file = "/home/stud/local_git/Kursovaia_Server/test/base.txt");
    std::map<std::string, std::string> get_data(); 
};
