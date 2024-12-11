#include <iostream>
#include "Connector_to_base.h"
#include "Interface.h"
#include "Client_Communicate.h"
#include "Calculator.h"
#include "Errors.h"
#include "Logger.h"

int main(int argc, const char** argv) {
    std::cout << "Server starting..." << std::endl;
    Interface interface;
    return interface.comm_proc(argc, argv);
}
