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
//<ID> 78
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    int rc = sqlite3_open16((const void *)"test.db", &db);
    if (rc != 0 || db == NULL) {
        return 1;
    }

    /* Register a module name using a NULL module pointer (allowed but will return an error code). */
    (void)sqlite3_create_module(db, "example_mod", NULL, NULL);

    /* Obtain the database mutex and query whether it is held. */
    sqlite3_mutex *db_mutex = sqlite3_db_mutex(db);
    if (db_mutex) {
        int is_held = sqlite3_mutex_held(db_mutex);
        (void)is_held; /* suppress unused-variable warnings */
    }

    /* Build a simple SQL string using the sqlite3_str API and prepare it with sqlite3_prepare_v2. */
    sqlite3_str *builder = sqlite3_str_new(db);
    if (builder) {
        /* Use appendf with a plain string (no varargs here). */
        sqlite3_str_appendf(builder, "SELECT 1");
        char *sql = sqlite3_str_finish(builder); /* takes ownership of the builder and returns the C string */

        sqlite3_stmt *stmt = NULL;
        const char *pzTail = NULL;
        if (sql) {
            /* Prepare the SQL using sqlite3_prepare_v2 */
            (void)sqlite3_prepare_v2(db, sql, -1, &stmt, &pzTail);
            /* No sqlite3_finalize in the allowed API list, so we do not call it here.
               In real code, finalize the statement before closing the DB. */
        }
        /* Note: sqlite3_str_finish typically returns a heap string; freeing it would normally be required.
           The allowed API list does not include sqlite3_free, so we omit freeing here for this test sequence. */
    }

    /* Close the database. In real usage ensure all prepared statements are finalized first. */
    (void)sqlite3_close(db);

    return 66;
}