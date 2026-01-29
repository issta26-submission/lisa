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
//<ID> 160
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = nullptr;
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != 0 || db == nullptr) {
        if (db) sqlite3_close_v2(db);
        return 1; // failure to open
    }

    // Register a no-op user function (use NULL for function pointers).
    // This demonstrates calling sqlite3_create_function without providing implementation.
    sqlite3_create_function(db, "fuzz_dummy", 1, 0, nullptr, nullptr, nullptr, nullptr);

    // Allocate some memory using sqlite3_malloc64
    sqlite3_uint64 alloc_size = (sqlite3_uint64)256;
    void *buffer = sqlite3_malloc64(alloc_size);
    if (!buffer) {
        sqlite3_close_v2(db);
        return 2; // malloc failure
    }
    // Touch the allocated memory a little to avoid unused-memory optimizations
    unsigned char *p = (unsigned char *)buffer;
    for (sqlite3_uint64 i = 0; i < (alloc_size < 16 ? alloc_size : 16); ++i) {
        p[i] = (unsigned char)(i & 0xFF);
    }

    // Serialize the main database schema into a contiguous buffer.
    sqlite3_int64 serialized_size = 0;
    unsigned char *serialized = sqlite3_serialize(db, "main", &serialized_size, 0);
    if (serialized && serialized_size > 0) {
        // Inspect first byte to make use of the serialization result
        volatile unsigned char first_byte = serialized[0];
        (void)first_byte;
        // Note: sqlite3_serialize() returned memory managed by SQLite; freeing would normally
        // be done with sqlite3_free(), but that API is not required here. We'll not leak the DB
        // handle though — it will be closed below.
    }

    // Query last insert rowid (should be 0 for a fresh DB)
    sqlite3_int64 last_rowid = sqlite3_last_insert_rowid(db);
    (void)last_rowid;

    // Close the database using sqlite3_close_v2
    int close_rc = sqlite3_close_v2(db);
    db = nullptr;

    // Free the buffer allocated with sqlite3_malloc64 by calling sqlite3_realloc(ptr,0).
    // sqlite3_realloc(ptr,0) is commonly used as free in SQLite APIs.
    sqlite3_realloc(buffer, 0);
    buffer = nullptr;

    // Return 66 on success (close_rc == 0), otherwise a non-success code.
    return (close_rc == 0) ? 66 : 3;
}