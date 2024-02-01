#include ".\Database.hpp"

Database::Database(const char *file_path) {
  int return_code = sqlite3_open(file_path, &connection);
  log("open", return_code);
}

Database::~Database() {
  int return_code = sqlite3_close(connection);
  log("close", return_code);
}

Database Database::createDatabase(const char *directory_path, const char *file_name) {
  std::unique_ptr<char[]> file_path = createFilePath(directory_path, file_name);
  std::ofstream database_file(file_path.get());
  Database database(file_path.get());
  return database;
}

std::unique_ptr<char[]> Database::createFilePath(const char *directory_path, const char *name) {
  std::unique_ptr<char[]> new_path = std::make_unique<char[]>(getSize(directory_path)+getSize(name)+5);
  char *tail = new_path.get();
  tail = concatinate(tail, directory_path);
  *tail++ = '\\';
  tail = concatinate(tail, name);
  *tail++ = '.';
  *tail++ = 'd';
  *tail++ = 'b';
  *tail = '\0';
  return new_path;
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
