#pragma once
#include <iostream>
#include <memory>
#include <sqlite3.h>
#include <variant>
#include <vector>
#include ".\string-functions.hpp"

#define ARGUMENT_TEXT 0
#define ARGUMENT_REAL 1
#define ARGUMENT_INTEGER 2

using Row = std::unique_ptr<char[]>;
using Table = std::vector<std::unique_ptr<char[]>>;

class Statement {
  private:
  sqlite3 *connection;
  sqlite3_stmt *statement;
  int getRowSize();
  void log(const char *function_name, const int &return_code);
  
  public:
  Statement(sqlite3 *connection, const char *sql);
  ~Statement();
  template<typename ...Type>
  void bind(Type &&...arguments);
  Table execute();
  void reset();
};

template<typename ...Type>
void Statement::bind(Type &&...arguments) {
  int index = 1, return_code;
  ([&](const std::variant<const char *, double, int> &argument) {
    switch (argument.index()) {
      case ARGUMENT_TEXT:
        return_code = sqlite3_bind_text(statement, index++, std::get<const char *>(argument), getSize(std::get<const char *>(argument))+1, SQLITE_STATIC);
        break;
      case ARGUMENT_REAL:
        return_code = sqlite3_bind_double(statement, index++, std::get<double>(argument));
        break;
      case ARGUMENT_INTEGER:
        return_code = sqlite3_bind_int(statement, index++, std::get<int>(argument));
        break;
      default:
        throw EXIT_FAILURE;
    }
  }(arguments), ...);
}
