#pragma once
#include <string>
#include <vector>
#include <map>
#include "Logger.h"
#include "Errors.h"
#include <openssl/evp.h>

class Client_Communicate {
public:
    static std::string sha256(std::string input_str);
    static std::string generate_salt();
    int connection(int port, std::map<std::string, std::string> database, Logger* l1);
};
