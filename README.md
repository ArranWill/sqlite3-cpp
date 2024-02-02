# How to use
```cpp
#include ".\Database.hpp"

int main() {
  try {
    // Open/create a database specified by path
    Database employee(".\\employee.db");

    // Execute a one-off statement
    employee.execute(R"(
      CREATE TABLE Employee (
        id INTEGER PRIMARY KEY,
        first_name TEXT,
        last_name TEXT,
        position TEXT,
        salary REAL
      );
    )");

    // Create a reusable statement
    Statement insert_employee = employee.createStatement(R"(
      INSERT INTO Employee (first_name, last_name, position, salary)
      VALUES ($first_name, $last_name, $position, $salary);
    )");

    // Use the statement to bind, execute, reset
    insert_employee.bind("John", "Smith", "Assistant", 70000.0);
    insert_employee.execute();
    insert_employee.reset();

    // Bind then execute with different values
    insert_employee.bind("Jane", "Doe", "Manager", 100000.0);
    insert_employee.execute();
    
    // Execute a one-off statement with arguments, get table output
    Table managers = employee.execute(R"(
      SELECT *
      FROM Employee
      WHERE position = $position
      AND salary > $salary;
    )", "Manager", 75000.0);

    // Print table
    Database::print(managers);
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
Success: prepare returned OK
Success: step returned DONE
Success: finalise returned OK
Success: prepare returned OK
Success: bind returned OK
Success: step returned DONE
Success: reset returned OK
Success: clear returned OK
Success: bind returned OK
Success: step returned DONE
Success: prepare returned OK
Success: bind returned OK
Success: step returned OK
Success: finalise returned OK
2|Jane|Doe|Manager|100000.0
Success: finalise returned OK
Success: close returned OK
```
# Features
* Open an SQLite database connection in one line of code
* Use the execute method to prepare, bind, execute, and finalise a statement in one line of code
* Create a statement object to reuse the same prepared statement object multiple times
* Function templates allow zero or more arguments of variable type to be passed to the execute and bind methods
* Raw pointer objects from the SQLite3 C interface are encapsulated in classes
* Logging reports status of each operation
# Limitations
* Information extracted using a select statement is cast to text, which limits how you can interact with the data retrieved
* BLOB data type not currently supported
* Execute method always returns a table, even if the statement passed isn't a select query
