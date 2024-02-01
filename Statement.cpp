#include ".\Statement.hpp"

Statement::Statement(sqlite3 *connection, const char *sql) : connection(connection){
  int return_code = sqlite3_prepare_v2(connection, sql, getSize(sql)+1, &statement, nullptr);
  log("prepare", return_code);
}

Statement::~Statement() {
  int return_code = sqlite3_finalize(statement);
  log("finalise", return_code);
}

Table Statement::execute() {
  int return_code = sqlite3_step(statement);
  Table output = {};
  if (return_code == SQLITE_ROW) {
    char *tail;
    int i, n = sqlite3_data_count(statement);
    Row row;
    do {
      row = std::make_unique<char[]>(getRowSize());
      tail = row.get();
      for (i = 0; i < n; i++) {
        tail = concatinate(tail, sqlite3_column_text(statement, i));
        *tail++ = '|';
      }
      *--tail = '\0';
      output.push_back(std::move(row));
    } while (return_code = sqlite3_step(statement) == SQLITE_ROW);
  }
  log("step", return_code);
  return output;
}

void Statement::log(const char *function_name, const int &return_code) {
  switch (return_code) {
    case SQLITE_OK:
      std::cout << "Success: " << function_name << " returned OK" << std::endl;
      break;
    case SQLITE_DONE:
      std::cout << "Success: " << function_name << " returned DONE" << std::endl;
      break;
    default:
      std::cerr << "Error: " << function_name << " returned " << return_code << " (" << sqlite3_errmsg(connection) << ")" << std::endl;
      throw EXIT_FAILURE;
  }
}

int Statement::getRowSize() {
  int n = sqlite3_data_count(statement), size = 0;
  for (int i = 0; i < n; i++) {
    size += getSize(sqlite3_column_text(statement, i));
  }
  return size+n;  // Plus n to account for the '|' between columns, and the null terminator at the end of the string
}

void Statement::reset() {
  int return_code = sqlite3_reset(statement);
  log("reset", return_code);
  return_code = sqlite3_clear_bindings(statement);
  log("clear", return_code);
}
