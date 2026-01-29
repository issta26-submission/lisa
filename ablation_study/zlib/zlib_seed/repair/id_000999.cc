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
//<ID> 999
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence: compress to buffer, inspect pending, write via gzdopen, then cleanup.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    (void)init_ret;

    // step 2: Configure
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl[0];
    (void)first_crc;

    // step 3: Operate
    int def_ret = deflate(&strm, 0);
    unsigned int pending = 0;
    int bits = 0;
    int pend_ret = deflatePending(&strm, &pending, &bits);
    uLong comp_size = strm.total_out;
    gzFile gzf = gzdopen(1, "wb");
    int gz_written = 0;
    if (gzf) gz_written = gzwrite(gzf, comp_buf, (unsigned int)comp_size);
    int gz_close_ret = 0;
    if (gzf) gz_close_ret = gzclose(gzf);
    (void)def_ret;
    (void)pend_ret;
    (void)pending;
    (void)bits;
    (void)gz_written;
    (void)gz_close_ret;

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&strm);
    free(comp_buf);
    (void)version;
    (void)source_len;
    (void)bound;
    (void)comp_size;
    (void)def_end_ret;
    return 66;
    // API sequence test completed successfully
}