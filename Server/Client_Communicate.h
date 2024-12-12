/** @file
 *  @brief Заголовок для класса Client_Communicate
 *  @details Этот файл содержит определения утилит для клиент-серверной коммуникации.
 *  @version 1.0
 *	@author Пономарев А.А.
 *  @date 12.12.2024
 */

#pragma once
#include <string>
#include <vector>
#include <map>
#include "Logger.h"
#include "Errors.h"
#include <openssl/evp.h>

/** @class Client_Communicate
 *  @brief Класс для управления коммуникацией клиента
 */
class Client_Communicate {
public:
    /** @brief Вычисляет SHA-256 хеш строки
     *  @param input_str Строка для хеширования
     *  @return SHA-256 хеш
     */
    static std::string sha256(std::string input_str);

    /** @brief Генерирует случайную соль
     *  @return Сгенерированная строка соли
     */
    static std::string generate_salt();

    /** @brief Управляет попыткой подключения
     *  @param port Порт для подключения
     *  @param database Карта, представляющая базу данных
     *  @param l1 Объект логгера для записи логов
     *  @return Код состояния операции подключения
     */
    int connection(int port, std::map<std::string, std::string> database, Logger* l1);
};
