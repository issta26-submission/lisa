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
//<ID> 527
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *insert_stmt = NULL;
    sqlite3_stmt *select_stmt = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_mutex *mutex = NULL;
    const char *create_sql = "CREATE TABLE t1(id INTEGER PRIMARY KEY, data TEXT);";
    const char *insert_sql = "INSERT INTO t1(data) VALUES(?1);";
    const char *select_sql = "SELECT rowid, data FROM t1 LIMIT 1;";
    const char *bound_text = "hello";
    const unsigned char encoding_utf8 = 1;
    sqlite3_int64 rowid = 1;
    sqlite3_uint64 text_len = (sqlite3_uint64)5;
    int rc_open = 0;
    int rc_exec = 0;
    int rc_prep_insert = 0;
    int rc_step_insert = 0;
    int rc_finalize_insert = 0;
    int rc_prep_select = 0;
    int rc_step_select = 0;
    int rc_blob_open = 0;
    int rc_blob_close = 0;
    int rc_finalize_select = 0;
    int rc_close = 0;
    int rc_testctrl = 0;
    int rc_mutexheld = 0;
    const unsigned char *selected_text = NULL;

    // step 2: Setup / Configure
    rc_open = sqlite3_open(":memory:", &db);
    rc_exec = sqlite3_exec(db, create_sql, NULL, NULL, NULL);

    // step 3: Operate / Validate
    rc_prep_insert = sqlite3_prepare_v2(db, insert_sql, -1, &insert_stmt, NULL);
    rc_prep_insert = rc_prep_insert; /* keep value used */
    sqlite3_bind_text64(insert_stmt, 1, bound_text, text_len, NULL, encoding_utf8);
    rc_step_insert = sqlite3_step(insert_stmt);
    rc_finalize_insert = sqlite3_finalize(insert_stmt);
    rc_prep_select = sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, NULL);
    rc_step_select = sqlite3_step(select_stmt);
    rowid = sqlite3_column_int64(select_stmt, 0);
    selected_text = sqlite3_column_text(select_stmt, 1);
    rc_blob_open = sqlite3_blob_open(db, "main", "t1", "data", rowid, 0, &blob);
    rc_testctrl = sqlite3_test_control(0);
    rc_mutexheld = sqlite3_mutex_held(mutex);

    // step 4: Cleanup
    rc_finalize_select = sqlite3_finalize(select_stmt);
    rc_blob_close = sqlite3_blob_close(blob);
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_exec;
    (void)rc_prep_insert;
    (void)rc_step_insert;
    (void)rc_finalize_insert;
    (void)rc_prep_select;
    (void)rc_step_select;
    (void)rowid;
    (void)selected_text;
    (void)rc_blob_open;
    (void)rc_testctrl;
    (void)rc_mutexheld;
    (void)rc_finalize_select;
    (void)rc_blob_close;
    (void)rc_close;
    (void)bound_text;
    (void)text_len;
    (void)encoding_utf8;
    (void)mutex;

    // API sequence test completed successfully
    return 66;
}