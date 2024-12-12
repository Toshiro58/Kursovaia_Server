/** @file
 *  @brief Содержит классы для обработки ошибок
 *  @details Определяет критические и некритические ошибки для приложения
 *  @version 1.0
 *	@author Пономарев А.А
 *  @date 12.12.2024
 */

#pragma once
#include <stdexcept>
#include <string>

/** @class crit_err
 *  @brief Представляет критическую ошибку
 */
class crit_err : public std::runtime_error {
public:
    /** @brief Конструктор объекта crit_err
     *  @param s Сообщение об ошибке
     */
    crit_err(const std::string& s) : std::runtime_error(s) {}
};

/** @class no_crit_err
 *  @brief Представляет некритическую ошибку
 */
class no_crit_err : public std::runtime_error {
public:
    /** @brief Конструктор объекта no_crit_err
     *  @param s Сообщение об ошибке
     */
    no_crit_err(const std::string& s) : std::runtime_error(s) {}
};
