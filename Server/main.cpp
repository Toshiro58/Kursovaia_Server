/** @file
 *  @brief Основная точка входа для серверного приложения
 *  @details Инициализирует сервер и запускает процесс коммуникации.
 *  @version 1.0
 *	@author Пономарев А.А
 *  @date 12.12.2024
 */

#include <iostream>
#include "Connector_to_base.h"
#include "Interface.h"
#include "Client_Communicate.h"
#include "Calculator.h"
#include "Errors.h"
#include "Logger.h"

/** @brief Основная функция для запуска сервера
 *  @param argc Количество аргументов
 *  @param argv Вектор аргументов
 *  @return Код состояния работы сервера
 */
int main(int argc, const char** argv) {
    std::cout << "Сервер запущен..." << std::endl;
    Interface interface;
    return interface.comm_proc(argc, argv);
}
