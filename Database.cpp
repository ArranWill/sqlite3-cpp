#include ".\Database.hpp"

Database::Database(const char *file_path) {
  int return_code = sqlite3_open(file_path, &connection);
  log("open", return_code);
}

Database::~Database() {
  int return_code = sqlite3_close(connection);
  log("close", return_code);
}

Statement Database::createStatement(const char *sql) {
  Statement statement(connection, sql);
  return statement;
}

void Database::log(const char *function_name, const int &return_code) {
  switch (return_code) {
    case SQLITE_OK:
      std::cout << "Success: " << function_name << " returned OK" << std::endl;
      break;
    default:
      std::cerr << "Error: " << function_name << " returned " << return_code << " (" << sqlite3_errmsg(connection) << ")" << std::endl;
      throw EXIT_FAILURE;
  }
}

void Database::print(const Table &table) {
  for (int i = 0; i < table.size(); i++) {
    std::cout << table[i].get() << std::endl;
  }
}
