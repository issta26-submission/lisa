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
//<ID> 997
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & Configure
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence: deflate into buffer, inspect pending, write original to gz via gzdopen, then cleanup.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl ? crc_tbl[0] : (z_crc_t)0;
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 2: Operate
    int def_ret = deflate(&def_strm, 0);
    uLong comp_size = def_strm.total_out;
    unsigned int pending = 0;
    int bits = 0;
    int pending_ret = deflatePending(&def_strm, &pending, &bits);

    // step 3: Validate
    gzFile gzf = gzdopen(1, "wb");
    int gz_write_ret = gzwrite(gzf, source, (unsigned int)source_len);
    int gz_flush_ret = gzflush(gzf, 2); // 2 commonly corresponds to Z_FINISH/ZONE flush; value use is illustrative
    int gz_close_ret = gzclose(gzf);
    int def_end_ret = deflateEnd(&def_strm);

    // step 4: Cleanup
    free(comp_buf);
    (void)version;
    (void)first_crc;
    (void)def_init_ret;
    (void)def_ret;
    (void)comp_size;
    (void)pending;
    (void)bits;
    (void)pending_ret;
    (void)gz_write_ret;
    (void)gz_flush_ret;
    (void)gz_close_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}