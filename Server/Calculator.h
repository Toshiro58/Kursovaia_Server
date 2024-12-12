/** @file
 *  @brief Заголовок для класса Calculator
 *  @details Этот файл содержит определение класса Calculator, который выполняет вычисления на входных данных.
 *  @version 1.0
 *  @date 12.12.2024
 *  @author Пономарев А.А
 *  @warning Убедитесь в корректной обработке больших наборов данных
 */

#pragma once
#include <vector>
#include <limits>
#include <boost/numeric/conversion/cast.hpp>

/** @class Calculator
 *  @brief Класс для выполнения вычислений
 */
class Calculator {
public:
    /** @brief Хранит результат вычислений */
    int64_t results;

    /** @brief Конструктор объекта Calculator
     *  @param input_data Вектор входных данных для вычислений
     */
    Calculator(std::vector<int64_t> input_data);

    /** @brief Получает результат вычислений
     *  @return Результат вычислений
     */
    int64_t send_res();
};
