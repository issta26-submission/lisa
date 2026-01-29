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
//<ID> 28
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char sqlbuf[128];
    unsigned char write_buf[16];
    unsigned char read_buf[16];
    int rc_open = 0;
    char *snprintf_ret = NULL;
    void *temp_alloc = NULL;
    unsigned long long alloc_size = 0;
    const char *ver = NULL;
    sqlite3_int64 mem_before = 0;

    // step 2: Setup - open an in-memory database and prepare a SQL string
    rc_open = sqlite3_open16((const void*)":memory:", &db);
    snprintf_ret = sqlite3_snprintf((int)sizeof(sqlbuf), sqlbuf, "CREATE TABLE t(a BLOB);");
    sqlite3_prepare(db, sqlbuf, -1, &stmt, NULL);

    // step 3: Operate - simulate work, allocate a fake blob-like buffer and query its size
    memset(write_buf, 0xA5, sizeof(write_buf));
    memset(read_buf, 0, sizeof(read_buf));
    sqlite3_sleep(1);
    ver = sqlite3_libversion();
    mem_before = sqlite3_memory_used();
    temp_alloc = sqlite3_malloc64((sqlite3_uint64)32);
    alloc_size = (unsigned long long)sqlite3_msize(temp_alloc);

    // store alloc_size into read_buf in a simple, straight-line way (no branches)
    read_buf[0] = (unsigned char)(alloc_size & 0xFF);
    read_buf[1] = (unsigned char)((alloc_size >> 8) & 0xFF);
    read_buf[2] = (unsigned char)((alloc_size >> 16) & 0xFF);
    read_buf[3] = (unsigned char)((alloc_size >> 24) & 0xFF);

    // step 4: Cleanup - free allocated memory and leave DB (no sqlite3_close available in provided API list)
    sqlite3_free(temp_alloc);

    (void)rc_open;
    (void)snprintf_ret;
    (void)stmt;
    (void)db;
    (void)write_buf;
    (void)read_buf;
    (void)ver;
    (void)mem_before;
    (void)alloc_size;

    // API sequence test completed successfully
    return 66;
}