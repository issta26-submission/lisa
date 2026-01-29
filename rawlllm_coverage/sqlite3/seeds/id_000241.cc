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
//<ID> 241
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = nullptr;
    int rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        if (db) sqlite3_close_v2(db);
        return 1;
    }

    /* create a simple table */
    rc = sqlite3_exec(db, "CREATE TABLE test(id INTEGER PRIMARY KEY, val TEXT);", nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close_v2(db);
        return 1;
    }

    /* prepare an INSERT with two parameters */
    sqlite3_stmt *insert = nullptr;
    rc = sqlite3_prepare_v3(db, "INSERT INTO test(id, val) VALUES(?1, ?2);", -1, 0, &insert, nullptr);
    if (rc != SQLITE_OK || insert == nullptr) {
        if (insert) sqlite3_finalize(insert);
        sqlite3_close_v2(db);
        return 1;
    }

    /* bind id (int) */
    rc = sqlite3_bind_int64(insert, 1, 1);

    /* bind a pointer as the second parameter using sqlite3_bind_pointer */
    const char *payload = "hello-pointer";
    auto bind_destructor = [](void *p) { /* no-op destructor */ };
    void (*bind_destructor_ptr)(void *) = bind_destructor; /* convert non-capturing lambda */
    /* call sqlite3_bind_pointer as requested */
    sqlite3_bind_pointer(insert, 2, (void *)payload, "my.ptr.type", bind_destructor_ptr);

    /* execute the insert */
    rc = sqlite3_step(insert);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(insert);
        sqlite3_close_v2(db);
        return 1;
    }
    sqlite3_finalize(insert);

    /* prepare a SELECT to retrieve the inserted value */
    sqlite3_stmt *select = nullptr;
    rc = sqlite3_prepare_v3(db, "SELECT val FROM test WHERE id = ?1;", -1, 0, &select, nullptr);
    if (rc != SQLITE_OK || select == nullptr) {
        if (select) sqlite3_finalize(select);
        sqlite3_close_v2(db);
        return 1;
    }
    rc = sqlite3_bind_int64(select, 1, 1);
    rc = sqlite3_step(select);

    /* use sqlite3_column_value on the resulting row (if any) */
    if (rc == SQLITE_ROW) {
        sqlite3_value *v = sqlite3_column_value(select, 0);
        (void)v; /* we don't need to do anything further with v for this test */
    }
    sqlite3_finalize(select);

    /* create a custom collation using sqlite3_create_collation_v2 */
    auto cmp_lambda = [](void * /*pArg*/, int len1, const void *s1, int len2, const void *s2) -> int {
        const unsigned char *a = reinterpret_cast<const unsigned char *>(s1);
        const unsigned char *b = reinterpret_cast<const unsigned char *>(s2);
        int minl = (len1 < len2) ? len1 : len2;
        for (int i = 0; i < minl; ++i) {
            if (a[i] < b[i]) return -1;
            if (a[i] > b[i]) return 1;
        }
        if (len1 < len2) return -1;
        if (len1 > len2) return 1;
        return 0;
    };
    int (*cmp_ptr)(void *, int, const void *, int, const void *) = cmp_lambda;
    /* reuse the same simple destructor pointer for collation cleanup */
    sqlite3_create_collation_v2(db, "my_custom_collation", SQLITE_UTF8, nullptr, cmp_ptr, bind_destructor_ptr);

    /* close DB with sqlite3_close_v2 as requested */
    sqlite3_close_v2(db);

    return 66;
}