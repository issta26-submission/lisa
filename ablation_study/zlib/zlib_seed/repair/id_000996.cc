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
//<ID> 996
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
    const Bytef source[] = "zlib API sequence: deflate, inspect pending, write to gz via gzdopen, then cleanup.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    (void)def_init_ret;

    // step 2: Configure
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 0);
    unsigned int pending = 0;
    int pending_bits = 0;
    int pending_ret = deflatePending(&def_strm, &pending, &pending_bits);
    gzFile gzf = gzdopen(1, "wb");
    int gz_write_ret = gzwrite(gzf, (void *)source, (unsigned int)source_len);
    int gz_close_ret = gzclose(gzf);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    (void)version;
    (void)crc_table;
    (void)source_len;
    (void)bound;
    (void)def_ret;
    (void)pending;
    (void)pending_bits;
    (void)pending_ret;
    (void)gz_write_ret;
    (void)gz_close_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}