#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 919
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const z_crc_t *crc_table = get_crc_table();

    // step 2: Configure
    const char msg[] = "Zlib gzputs test payload";
    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int buf_set = gzbuffer(gzf, 8192);

    // step 3: Operate
    int put_ret = gzputs(gzf, msg);
    off_t current_offset = gzoffset(gzf);

    // step 4: Validate / Cleanup
    int close_ret = gzclose(gzf);
    z_crc_t sample_crc = crc_table ? crc_table[0] : (z_crc_t)0;
    (void)version;
    (void)buf_set;
    (void)put_ret;
    (void)current_offset;
    (void)close_ret;
    (void)sample_crc;
    // API sequence test completed successfully
    return 66;
}