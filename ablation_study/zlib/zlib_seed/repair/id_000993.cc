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
//<ID> 993
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence: deflate data, inspect pending, write via gzdopen, then cleanup.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int def_init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t crc0 = crc_table[0];
    strm.adler = (uLong)crc0;

    // step 2: Configure
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&strm, 0);
    unsigned int pending = 0;
    int bits = 0;
    int pend_ret = deflatePending(&strm, &pending, &bits);
    uLong comp_size = strm.total_out;
    gzFile gzf = gzdopen(1, "wb");
    int gz_write_ret = 0;
    gz_write_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_size);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&strm);
    int gz_close_ret = gzclose(gzf);
    free(comp_buf);
    (void)version;
    (void)def_init_ret;
    (void)crc_table;
    (void)crc0;
    (void)bound;
    (void)def_ret;
    (void)pending;
    (void)bits;
    (void)pend_ret;
    (void)comp_size;
    (void)gz_write_ret;
    (void)def_end_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}