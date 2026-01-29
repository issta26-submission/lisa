#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 104
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // Open an in-memory database.
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != 0 || db == nullptr) {
        return -1;
    }

    // Prepare a simple statement that returns a single integer column.
    sqlite3_stmt *stmt = nullptr;
    rc = sqlite3_prepare(db, "SELECT 1", -1, &stmt, nullptr);
    if (rc != 0 || stmt == nullptr) {
        return -2;
    }

    // Step the statement to produce a row.
    sqlite3_step(stmt);

    // Retrieve the value for column 0 and check its type.
    sqlite3_value *val = sqlite3_column_value(stmt, 0);
    int vtype = sqlite3_value_type(val);

    // Use a glob pattern match on a literal string.
    int glob_match = sqlite3_strglob("1", "1"); // expected to indicate a match

    // Reset the prepared statement so it can be re-executed.
    int reset_rc = sqlite3_reset(stmt);

    // Attempt a try-lock on a mutex pointer; pass nullptr as a placeholder.
    // Some sqlite3 builds handle nullptr by returning an error code instead of crashing.
    int try_rc = sqlite3_mutex_try(nullptr);

    // Silence unused-variable warnings and treat above calls as the required API usage.
    (void)vtype;
    (void)glob_match;
    (void)reset_rc;
    (void)try_rc;

    // Success sentinel required by the task.
    return 66;
}