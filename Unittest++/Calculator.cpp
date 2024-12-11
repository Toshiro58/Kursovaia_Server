#include "Calculator.h"
#include <limits>
#include <vector>
#include <stdexcept>

Calculator::Calculator(std::vector<int64_t> input_data) {
    int64_t sum = 0;
    int count = input_data.size();

    if (count == 0) {
        results = 0; 
        return;
    }

    for (auto elem : input_data) {
        // Проверка на переполнение
        if ((elem > 0) && (sum > std::numeric_limits<int64_t>::max() - elem)) {
            results = std::numeric_limits<int64_t>::max();
            return;
        }
        if ((elem < 0) && (sum < std::numeric_limits<int64_t>::min() - elem)) {
            results = std::numeric_limits<int64_t>::min();
            return;
        }
        sum += elem;
    }

    results = sum / count;
}

int64_t Calculator::send_res() {
    return results;
}
