#pragma once
#include <vector>
#include <limits>
#include <boost/numeric/conversion/cast.hpp>

class Calculator {
public:
    int64_t results;
    Calculator(std::vector<int64_t> input_data);
    int64_t send_res();
};
