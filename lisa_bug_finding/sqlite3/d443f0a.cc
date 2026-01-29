#include <cstddef>
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
#include <cassert>

// Header
int test_api_sequence() {
// step 1: Declarations (initialize handles and sizes)
    sqlite3 *db = 0;
    sqlite3_stmt *stmt = 0;
    const char *tail = 0;
    int col_type = 0;
    int data_count = 0;
    int busy = 0;
    sqlite3 *db_handle = 0;  
    int rc_open = sqlite3_open(":memory:", &db);
    int rc_prepare = sqlite3_prepare_v2(db, "SELECT 0 OR 2", -1, &stmt, &tail);
    int rc_step = sqlite3_step(stmt);
    assert(rc_open == SQLITE_OK);
    assert(db != 0);
    assert(rc_prepare == SQLITE_OK);
    assert(stmt != 0);
    assert(tail != 0);
    assert(*tail == '\0');
    assert(sqlite3_db_handle(stmt) == db);
    assert(rc_step == SQLITE_ROW);
// step 2: Setup
    col_type = sqlite3_column_type(stmt, 0);
    assert(col_type == SQLITE_INTEGER);
    int val = sqlite3_column_int(stmt, 0);
    assert(val == 1);
// step 3: Operate
    data_count = sqlite3_data_count(stmt);
    assert(data_count == 1);
    busy = sqlite3_stmt_busy(stmt);
    assert(busy != 0); 
    db_handle = sqlite3_db_handle(stmt);
    assert(db_handle == db);
    int is_readonly = sqlite3_stmt_readonly(stmt);
    assert(is_readonly != 0);
// step 4: Cleanup
    int rc_finalize = sqlite3_finalize(stmt);
    assert(rc_finalize == SQLITE_OK);
    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)col_type;
    (void)val;
    (void)data_count;
    (void)busy;
    (void)db_handle;
    (void)is_readonly;
    (void)rc_finalize;
    return 66;
}

int main() {
    for (int i = 0; i < 10; i++) {
        int result = test_api_sequence();
        if (result != 66) {
            std::cerr << "API sequence test failed with error code: " << result << std::endl;
            exit(100);
        }
    }
    printf("Test completed successfully.\n");
    return 0;
}