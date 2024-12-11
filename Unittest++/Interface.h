#pragma once
#include "Logger.h"
#include <string>

class Interface {
public:
    Interface() = default;  
    int comm_proc(int argc, const char** argv);
};
