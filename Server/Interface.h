/** @file
 *  @brief Заголовок для класса Interface
 *  @details Определяет интерфейс для обработки коммуникации
 *  @version 1.0
 *	@author Пономарев А.А.
 *  @date 12.12.2024
 */

#pragma once
#include "Logger.h"
#include <string>

/** @class Interface
 *  @brief Управляет процессом коммуникации
 */
class Interface {
public:
    /** @brief Конструктор по умолчанию */
    Interface() = default;  

    /** @brief Обрабатывает аргументы командной строки
     *  @param argc Количество аргументов
     *  @param argv Вектор аргументов
     *  @return Код состояния обработки
     */
    int comm_proc(int argc, const char** argv);
};
