/** @file
 *  @brief Заголовок для класса Logger
 *  @details Обеспечивает функциональность логирования для приложения
 *  @version 1.0
 *	@author Пономарев А.А
 *  @date 12.12.2024
 */

#pragma once
#include <string>

/** @class Logger
 *  @brief Простой класс логирования
 */
class Logger {
    /** @brief Получает текущие дату и время в виде строки
     *  @param s Формат строки
     *  @return Текущие дата и время
     */
    static std::string getCurrentDateTime(std::string s);

    std::string path_to_logfile;

public:
    /** @brief Записывает лог
     *  @param s Сообщение для логирования
     *  @return Код состояния операции записи
     */
    int writelog(std::string s);

    /** @brief Устанавливает путь к файлу логирования
     *  @param path_file Путь к файлу логов
     *  @return Код состояния установки пути
     */
    int set_path(std::string path_file);

    /** @brief Конструктор по умолчанию */
    Logger() { path_to_logfile = " "; };

    /** @brief Конструктор класса Logger с заданным путем к файлу логов
     *  @param s Путь к файлу логов
     */
    Logger(std::string s) { path_to_logfile = s; };
};
