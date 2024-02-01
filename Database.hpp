#pragma once
#include <fstream>
#include ".\Statement.hpp"

class Database {
  private:
  sqlite3 *connection;
  static std::unique_ptr<char[]> createFilePath(const char *directory_path, const char *name);
  void log(const char *function_name, const int &return_code);

  public:
  Database(const char *file_path);
  ~Database();
  static Database createDatabase(const char *directory_path, const char *name);
  Statement createStatement(const char *sql);
  template<typename ...Type>
  Table execute(const char *sql, Type &&...arguments);
  static void print(const Table &table);
};

template<typename ...Type>
Table Database::execute(const char *sql, Type &&...arguments) {
  Statement statement = createStatement(sql);
  if (sizeof...(arguments) > 0) {
    statement.bind(arguments...);
  }
  return statement.execute();
}
