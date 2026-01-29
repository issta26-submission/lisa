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
//<ID> 217
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = nullptr;
    if (sqlite3_open(":memory:", &db) != 0) {
        return 1;
    }

    const char *sql = "SELECT 123 AS thecol";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != 0) {
        return 2;
    }

    // Use sqlite3_column_name to obtain the name of the first column.
    const char *colname = sqlite3_column_name(stmt, 0);

    // Clear any bindings on the prepared statement (no-op here but valid usage).
    sqlite3_clear_bindings(stmt);

    // Create a sqlite3_str and append formatted text including the column name.
    sqlite3_str *str = sqlite3_str_new(db);
    if (str) {
        sqlite3_str_appendf(str, "col=%s", colname ? colname : "NULL");
        char *valstr = sqlite3_str_value(str);
        if (valstr) {
            // Log the composed string (sqlite3_log accepts a format string).
            sqlite3_log(0, valstr);
        }
    }

    // Obtain a sqlite3_value* for the first column (from the prepared statement).
    // Note: in a normal usage sqlite3_column_value is meaningful after sqlite3_step()
    // but it's valid to call this API as a demonstration here.
    sqlite3_value *colval = sqlite3_column_value(stmt, 0);
    sqlite3_int64 intval = 0;
    if (colval) {
        // Use sqlite3_value_int64 to extract a 64-bit integer.
        intval = sqlite3_value_int64(colval);
        // Log the integer value as well (format string only, integer appended manually not supported,
        // so we just log a static message to demonstrate usage).
        sqlite3_log(0, "extracted int64 from sqlite3_value");
        (void)intval; // silence unused-variable warnings if any
    }

    // Demonstrate sqlite3_vtab_collation by calling it with a stack index_info.
    sqlite3_index_info idxinfo = {};
    const char *coll = sqlite3_vtab_collation(&idxinfo, 0);
    if (str) {
        sqlite3_str_appendf(str, " coll=%s", coll ? coll : "NULL");
    }

    // Success
    return 66;
}