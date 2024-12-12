/** @file
 *  @brief Заголовок для класса Connector_to_base
 *  @details Этот файл включает класс для управления подключениями к базе данных.
 *  @version 1.0
 *	@author Пономарев А.А
 *  @date 12.12.2024
 */

#pragma once
#include <map>
#include <string>

/** @class Connector_to_base
 *  @brief Управляет подключениями к базе данных
 */
class Connector_to_base {
private:
    std::map<std::string, std::string> data_base;

public:
    /** @brief Подключается к базе данных
     *  @param base_file Путь к файлу базы данных
     *  @return Код состояния подключения
     */
    int connect_to_base(std::string base_file = "/home/stud/local_git/Kursovaia_Server/test/base.txt");

    /** @brief Получает данные из базы данных
     *  @return Карта, содержащая записи базы данных
     */
    std::map<std::string, std::string> get_data(); 
};
