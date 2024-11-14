#include "Calculator.h"
#include <boost/numeric/conversion/cast.hpp>
#include <limits>
#include <vector>

Calculator::Calculator(std::vector<int64_t> input_data) {
    try {
        int64_t sum = 0;
        int count = input_data.size();

        if (count == 0) {
            results = 0; 
            return;
        }

        for (auto elem : input_data) {
            sum = boost::numeric_cast<int64_t>(sum + elem);
        }

        results = sum / count;
    } catch (const boost::numeric::negative_overflow& e) {
        results = std::numeric_limits<int64_t>::min();
    } catch (const boost::numeric::positive_overflow& e) {
        results = std::numeric_limits<int64_t>::max();
    }
}

int64_t Calculator::send_res() {
    return results;
}
