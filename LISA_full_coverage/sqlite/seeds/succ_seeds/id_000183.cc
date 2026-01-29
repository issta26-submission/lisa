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
//<ID> 183
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    sqlite3_stmt *stmt3 = NULL;
    sqlite3_snapshot snap1;
    sqlite3_snapshot snap2;
    const void *pzTail = NULL;
    const char *select_sql = "SELECT 1 AS id, 'hello' AS val;";
    const char *col0_origin = NULL;

    // UTF-16 (native endian) encoded SQL statements as arrays of 16-bit code units.
    static const unsigned short create_sql16[] = {
        'C','R','E','A','T','E',' ','T','A','B','L','E',' ','t','(','i','d',' ',
        'I','N','T','E','G','E','R',' ','P','R','I','M','A','R','Y',',',' ','v','a','l',' ',
        'T','E','X','T',')',';','\0'
    };
    static const unsigned short insert_sql16[] = {
        'I','N','S','E','R','T',' ','I','N','T','O',' ','t','(','i','d',',','v','a','l',')',' ',
        'V','A','L','U','E','S','(','1',',','\'','h','e','l','l','o','\'',')',';','\0'
    };
    static const unsigned short select_sql16[] = {
        'S','E','L','E','C','T',' ','1',' ','A','S',' ','i','d',',',' ','\'','h','e','l','l','o','\'',
        ' ','A','S',' ','v','a','l',';','\0'
    };

    // step 2: Configure / Prepare statements (using UTF-16 prepare)
    int rc_init = sqlite3_initialize();
    int rc_open = sqlite3_open(":memory:", &db);
    int rc_prepare1 = sqlite3_prepare16_v2(db, (const void*)create_sql16, -1, &stmt1, &pzTail);
    int rc_prepare2 = sqlite3_prepare16_v2(db, (const void*)insert_sql16, -1, &stmt2, &pzTail);
    int rc_prepare3 = sqlite3_prepare16_v2(db, (const void*)select_sql16, -1, &stmt3, &pzTail);

    // step 3: Operate - inspect column origin (for the prepared SELECT), check SQL completeness, compare snapshots
    // sqlite3_column_origin_name can legally be called on a prepared statement to obtain origin metadata (may be NULL).
    col0_origin = sqlite3_column_origin_name(stmt3, 0);
    int rc_complete = sqlite3_complete(select_sql);
    int rc_snapcmp = sqlite3_snapshot_cmp(&snap1, &snap2);

    // step 4: Validate / Cleanup-ish (release DB memory; not closing to avoid using unavailable APIs)
    int rc_release = sqlite3_db_release_memory(db);

    // silence unused-variable warnings for return-oriented testing
    (void)rc_init;
    (void)rc_open;
    (void)rc_prepare1;
    (void)rc_prepare2;
    (void)rc_prepare3;
    (void)col0_origin;
    (void)rc_complete;
    (void)rc_snapcmp;
    (void)rc_release;
    (void)stmt1;
    (void)stmt2;
    (void)stmt3;
    (void)db;

    // API sequence test completed successfully
    return 66;
}