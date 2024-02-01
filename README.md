# How to use
```cpp
#include "Database.hpp"

int main() {
  try {
    // Create a database file in the cwd named people, and open a connection to it
    Database people = Database::createDatabase(".", "people");

    // Open connection to an existing database named addresses in the cwd
    Database addresses(".\\addresses.db");

    // Execute a statement
    people.execute(R"(
      CREATE TABLE IF NOT EXISTS Person (
        id INTEGER PRIMARY KEY,
        first_name TEXT,
        last_name TEXT
      );
    )");

    // Create a reusable statement
    Statement insert_person = people.createStatement(R"(
      INSERT INTO Person (first_name, last_name)
      VALUES ($first_name, $last_name);
    )");

    // Use the statement to bind, execute, reset
    insert_person.bind("John", "Doe");
    insert_person.execute();
    insert_person.reset();

    // Bind then execute with different values
    insert_person.bind("Jane", "Doe");
    insert_person.execute();
    
    // Execute a statement with arguments, get table output
    Table people_named_john_jane_james = people.execute(R"(
      SELECT *
      FROM Person
      WHERE first_name IN ($a, $b, $c)
    ;)", "John", "Jane", "James");

    // Print table
    Database::print(people_named_john_jane_james);
    return EXIT_SUCCESS;
  }
  catch (int exception) {
    return exception;
  }
}
```

### Terminal output
```
Success: open returned OK
Success: open returned OK
Success: prepare returned OK
Success: step returned DONE
Success: finalise returned OK  // Statement is automatically finalised when the object goes out of scope
Success: prepare returned OK
Success: step returned DONE
Success: reset returned OK
Success: clear returned OK
Success: step returned DONE
Success: prepare returned OK
Success: step returned OK
Success: finalise returned OK
1|John|Doe
2|Jane|Doe
Success: finalise returned OK
Success: close returned OK  // Database is automatically closed when the object goes out of scope
Success: close returned OK
```
# Features
* Create a database file and open a connection in one line of code
* Prepare, bind, execute, and finalise a statement in one line of code
* Create a reusable statement object to repeatedly bind and execute the same statement
* Logging reports status of each operation
* Raw pointer objects from the SQLite3 C interface are encapsulated in classes
# Limitations
* Information extracted using a select statement is cast to text, which limits how you can interact with the data retrieved
