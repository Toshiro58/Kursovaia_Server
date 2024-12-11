#include "Logger.h"
#include "Connector_to_base.h" 
#include "Calculator.h"
#include "Client_Communicate.h"
#include "Interface.h"
#include <UnitTest++/UnitTest++.h>
#include <limits>
#include <chrono>
#include <thread>
#include <fstream>

SUITE(ConnectorTests) {
    TEST(IncorrectPath) {
        Connector_to_base con;
        std::string path = "invalid_path";
        CHECK_THROW(con.connect_to_base(path), crit_err);
    }
    TEST(VoidPath) {
        Connector_to_base con;
        std::string path = "";
        CHECK_THROW(con.connect_to_base(path), crit_err);
    }
    TEST(WrongPath) {
        Connector_to_base con;
        std::string path = "/lalalala/Sverchkovgeroi/base.txt";
        CHECK_THROW(con.connect_to_base(path), crit_err);
    }
    TEST(RightPath) {
        std::ofstream outfile("test1/base.txt");
        outfile << "user:P@ssW0rd\n"; 
        outfile.close();

        Connector_to_base con;
        CHECK_EQUAL(0, con.connect_to_base("test1/base.txt"));
    }
    TEST(CheckReading) {
        Connector_to_base con;
        std::string path = "test1/base.txt";
        con.connect_to_base(path);
        auto data = con.get_data();
        CHECK(data.find("user") != data.end() && data["user"] == "P@ssW0rd");
    }
}

SUITE(CalculatorTests) {
    TEST(PositiveOverflow) {
        std::vector<int64_t> v = {std::numeric_limits<int64_t>::max() / 2, std::numeric_limits<int64_t>::max() / 2 + 2};
        Calculator c(v);
        CHECK_EQUAL(std::numeric_limits<int64_t>::max(), c.send_res());
    }

    TEST(NegativeOverflow) {
        std::vector<int64_t> v = {std::numeric_limits<int64_t>::min() / 2, std::numeric_limits<int64_t>::min() / 2 - 1};
        Calculator c(v);
        CHECK_EQUAL(std::numeric_limits<int64_t>::min(), c.send_res());
    }

    TEST(ZeroVector) {
        std::vector<int64_t> v = {};
        Calculator c(v);
        CHECK_EQUAL(0, c.send_res());
    }

    TEST(SingleElement) {
        std::vector<int64_t> v = {1};
        Calculator c(v);
        CHECK_EQUAL(1, c.send_res());
    }

    TEST(MixedNumbers) {
        std::vector<int64_t> v = {50, -30, 20, -10, -30};
        Calculator c(v);
        CHECK_EQUAL(0, c.send_res());
    }

    TEST(LargeNumbers) {
        std::vector<int64_t> v(1000000, 1);
        Calculator c(v);
        CHECK_EQUAL(1, c.send_res());
    }
}

SUITE(LoggerTests) {
    TEST(WrongPath) {
        Logger logger;
        std::string path = "invalid_path/log.txt";
        CHECK_THROW(logger.set_path(path), crit_err);
    }
    TEST(VoidPath) {
        Logger logger;
        std::string path = "";
        CHECK_THROW(logger.set_path(path), std::invalid_argument);
    }
    TEST(CorrectPath) {
        Logger logger;
        std::string path = "test1/log.txt";
        CHECK_EQUAL(0, logger.set_path(path));
    }
}

SUITE(ClientCommunicateTests) {
    TEST(SHA256Gen) {
        Client_Communicate com;
        std::string input = "49C8E927291DDB01";
        std::string expected_hash = "A5B12ACEA9A8D045A0933815830437DCBD7BB2E2B9F314FC3B050CAE09BCA897";  
        std::string hash = com.sha256(input);
        CHECK_EQUAL(expected_hash, hash);
    }

    TEST(HashSaltPassword) {
        Client_Communicate com;
        std::string salt = "49C8E927291DDB01";
        std::string password = "P@ssW0rd";
        std::string combined = salt + password;
        std::string expected_hash = com.sha256(combined); 
        std::string hash = com.sha256(combined);
        CHECK_EQUAL(expected_hash, hash);
    }
}

int main(int argc, char **argv) {
    return UnitTest::RunAllTests();
}
