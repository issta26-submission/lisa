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
//<ID> 185
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_snapshot snapshot;
    const char *sql = "CREATE TABLE IF NOT EXISTS t(id INTEGER);";
    int rc_open = 0;
    int rc_complete = 0;
    int rc_snapshot = 0;
    int rc_close = 0;
    const void *col_origin = NULL;
    memset(&snapshot, 0, sizeof(snapshot));

    // step 2: Open database (Configure)
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Operate / Validate
    rc_complete = sqlite3_complete(sql);
    rc_snapshot = sqlite3_snapshot_open(db, "main", &snapshot);
    col_origin = sqlite3_column_origin_name16((sqlite3_stmt *)NULL, 0);

    // step 4: Cleanup
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_complete;
    (void)rc_snapshot;
    (void)rc_close;
    (void)col_origin;
    (void)sql;
    // API sequence test completed successfully
    return 66;
}