#include "Calculator.h"
#include "Client_Communicate.h"
#include "Errors.h"
#include "Logger.h"
#include <arpa/inet.h>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <openssl/evp.h>
#include <random>
#include <sstream>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <cryptopp/hex.h>
#include <cryptopp/base64.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/sha.h>

std::string Client_Communicate::sha256(std::string input_str) {
    using namespace CryptoPP;

    SHA256 hash;
    std::string digest;

    StringSource(input_str, true,
        new HashFilter(hash, 
            new HexEncoder(new StringSink(digest))
        )
    );

    return digest;
}

// Function to generate SALT
std::string Client_Communicate::generate_salt() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dist(0, std::numeric_limits<uint64_t>::max());
    std::stringstream s;
    s << std::hex << std::setw(16) << std::setfill('0') << dist(gen);
    return s.str();
}

int Client_Communicate::connection(int port, std::map<std::string, std::string> database, Logger* l1) {
    try {
        int queue_len = 100;
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_aton("127.0.0.1", &addr.sin_addr);
        int s = socket(AF_INET, SOCK_STREAM, 0);
        
        if(s <= 0) {
            throw crit_err("Socket created err");
        } else {
            l1->writelog("Listen socket created");
        }

        int rc = bind(s, (const sockaddr*)&addr, sizeof(sockaddr_in));
        if(rc == -1) {
            throw crit_err("Socket bind err");
        } else {
            l1->writelog("Bind success");
        }

        rc = listen(s, queue_len);
        if(rc == -1) {
            throw crit_err("Socket listen err");
        }

        for(;;) {
            try {
                sockaddr_in client_addr;
                socklen_t len = sizeof(client_addr);
                int work_sock = accept(s, (sockaddr*)&client_addr, &len);
                if(work_sock <= 0) {
                    throw no_crit_err("[Uncritical] Client socket error");
                }
                l1->writelog("Client socket created");

                // Receive authentication message
                char buffer[1024];
                rc = recv(work_sock, buffer, 1024, 0);
                if(rc <= 0) {
                    close(work_sock);
                    throw no_crit_err("[Uncritical] ID receive error");
                }
                buffer[rc] = 0;
                std::string auth_msg(buffer, rc);
                l1->writelog("Received authentication message: " + auth_msg);

                // Extract login, salt, and hash from message
                std::string login, salt, client_hash;
                if (auth_msg.length() < 80) { // Check length for login + salt + hash
                    close(work_sock);
                    throw no_crit_err("[Uncritical] Invalid auth message format");
                }

                // Extracting parts from the message
                client_hash = auth_msg.substr(auth_msg.length() - 64, 64);
                salt = auth_msg.substr(auth_msg.length() - 80, 16);
                login = auth_msg.substr(0, auth_msg.length() - 80);

                l1->writelog("Auth message parsed. Login: " + login + ", Salt: " + salt +
                             ", Client Hash: " + client_hash);

                // Verify authentication
                if(database.find(login) == database.end()) {
                    close(work_sock);
                    throw no_crit_err("[Uncritical] Unknown ID");
                }

                std::string expected_hash = sha256(salt + database[login]);
                l1->writelog("Calculated expected hash: " + expected_hash);

                if(expected_hash != client_hash) {
                    send(work_sock, "ERR", 3, 0);
                    close(work_sock);
                    throw no_crit_err("[Uncritical] Auth error");
                }

                // Send OK if authentication is successful
                if(send(work_sock, "OK", 2, 0) < 0) {
                    close(work_sock);
                    throw no_crit_err("[Uncritical] Error sending OK response");
                }
                l1->writelog("Authentication successful. Sent OK to client.");

                // Process vectors from client
                uint32_t num_vectors;
                rc = recv(work_sock, &num_vectors, sizeof(num_vectors), 0);
                if(rc <= 0) {
                    close(work_sock);
                    throw no_crit_err("[Uncritical error] Count of vectors not received");
                }
                l1->writelog("Vector count received");

                for(uint32_t i = 0; i < num_vectors; ++i) {
                    uint32_t vector_len;
                    rc = recv(work_sock, &vector_len, sizeof(vector_len), 0);
                    if(rc <= 0) {
                        close(work_sock);
                        throw no_crit_err("[Uncritical error] Length of vector not received");
                    }

                    // Check that the vector length is valid
                    if(vector_len == 0 || vector_len > (UINT32_MAX / sizeof(int64_t))) {
                        close(work_sock);
                        l1->writelog("[Uncritical error] Invalid vector length");
                        break; // Exit the loop processing vectors
                    }

                    std::unique_ptr<int64_t[]> vector_data(new int64_t[vector_len]);
                    rc = recv(work_sock, vector_data.get(), vector_len * sizeof(int64_t), 0);
                    if(rc <= 0) {
                        close(work_sock);
                        throw no_crit_err("[Uncritical error] Vector not received");
                    }

                    // Check that the received bytes match the vector size
                    if(static_cast<size_t>(rc) != vector_len * sizeof(int64_t)) {
                        close(work_sock);
                        l1->writelog("[Uncritical error] Received bytes do not match vector size");
                        break; // Exit the loop processing vectors
                    }

                    std::vector<int64_t> v(vector_data.get(), vector_data.get() + vector_len);
                    Calculator calc(v);
                    int64_t res = calc.send_res();
                    send(work_sock, &res, sizeof(res), 0);
                    l1->writelog("Result of calculating vector sent");
                }
            } catch(no_crit_err& e) {
                l1->writelog(e.what());
            }
        }
    } catch(crit_err& e) {
        l1->writelog(e.what());
    }
    return 0;
}
